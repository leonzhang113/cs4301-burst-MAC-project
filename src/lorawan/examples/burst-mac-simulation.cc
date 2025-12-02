#include "ns3/point-to-point-helper.h"
#include "ns3/lora-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/node-container.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/command-line.h"
#include "ns3/random-variable-stream.h"
#include "ns3/network-server-helper.h"
#include "ns3/forwarder-helper.h"
#include "ns3/periodic-sender-helper.h"
#include "ns3/periodic-sender.h"
#include "ns3/lora-tag.h"

#include <iostream>
#include <vector>

using namespace ns3;
using namespace lorawan;

NS_LOG_COMPONENT_DEFINE("LoraPdrSimulation");

uint64_t packetsSent = 0;
uint64_t packetsReceived = 0;
uint64_t packetsWithBurstBit = 0;

// Virtual Channels, channel, SF pair
using VCKey = std::pair<uint32_t, uint8_t>;
std::map<VCKey, std::deque<Time>> vcPacketTimes;
std::map<VCKey, uint64_t> vcReceivedCounts;
double collisionWindow = 1e-3; 
double collisionThreshold = 0.5; // make threshold for collisions 50%, can set to any value between 0 and 1
uint64_t totalReceived = 0;
uint64_t totalCollisions = 0;

void OnTransmissionCallback(Ptr<const Packet> packet, uint32_t senderNodeId)
{
    //NS_LOG_INFO("Packet sent by node " << senderNodeId);
    packetsSent++;
}

void OnPacketRecptionCallback(std::string context, Ptr<const Packet> packet)
{
    //NS_LOG_INFO("Packet received at MAC context: " << context);
    packetsReceived++;
    totalReceived++;

    Ptr<Packet> copy = packet->Copy();
    LorawanMacHeader hdr;
    copy->PeekHeader(hdr);

    // burst detection
    if (hdr.GetBurst())
    {
        packetsWithBurstBit++;
        std::cout << "Burst bit detected at t="
                  << Simulator::Now().GetSeconds() << "s" << std::endl;
    }

    // Extract freq, sf from LoraTag
    VCKey vc{0u, 0u};
    LoraTag rxTag;
    Ptr<Packet> tagProbe = packet->Copy();
    if (tagProbe->PeekPacketTag(rxTag))
    {
        vc.first = rxTag.GetFrequency();
        vc.second = rxTag.GetSpreadingFactor();
    }

    vcReceivedCounts[vc]++;
    Time now = Simulator::Now();
    auto &times = vcPacketTimes[vc];

    // Remove outdated entries
    while (!times.empty() && (now - times.front()) > Seconds(collisionWindow))
    {
        times.pop_front();
    }

    // Count collisions only within same VC window
    if (!times.empty())
    {
        totalCollisions++;
        NS_LOG_WARN("Collision in VC(f=" << vc.first << ",sf=" << unsigned(vc.second)
                                         << ") at gateway (MAC)");
    }

    times.push_back(now);

    // Trigger Burst-MAC if collision ratio exceeds threshold
    if (totalReceived > 0 && (double)totalCollisions / totalReceived > collisionThreshold)
    {
        NS_LOG_WARN("Burst-MAC triggered by collision ratio > threshold at gateway");
    }
}

int main(int argc, char* argv[])
{
    int nNodes = 500;
    int nGateways = 1;
    double radiusMeters = 6000;
    double simulationTimeSeconds = 70.0;
    Time appStopTime = Seconds(simulationTimeSeconds);

    // Burst scenario parameters (configurable via CLI)
    double burstFraction = 50.0;   // [0..1] fraction of nodes that will burst
    double burstStart = 0.0;     // set a time for burst signal to start
    double normalPeriod = 1;   // set length of normal signal period
    double burstPeriod = 0.1;     // length of burst signal period
    double burstThreshold = 0.0;  // threshold for packets/sec that nodes need to set to burst
    double baseSlotL = 0.2;       // base slot duration (seconds) for hash scheduling

    CommandLine cmd(__FILE__);
    cmd.AddValue("nNodes", "Number of end devices", nNodes);
    cmd.AddValue("x", "Number of gateways", nGateways);
    cmd.AddValue("radius", "Radius of the deployment area in meters", radiusMeters);
    cmd.Parse(argc, argv);

    LogComponentEnable("LoraPdrSimulation", LOG_LEVEL_INFO);

    LoraPhyHelper phyHelper = LoraPhyHelper();
    LorawanMacHelper macHelper = LorawanMacHelper();
    LoraHelper helper = LoraHelper();
    MobilityHelper mobility;

    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel>();
    loss->SetPathLossExponent(3.76);
    loss->SetReference(1, 7.7);

    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel>();
    Ptr<LoraChannel> channel = CreateObject<LoraChannel>(loss, delay);
    
    phyHelper.SetChannel(channel);
    helper.EnablePacketTracking();

    NodeContainer endDevices;
    endDevices.Create(nNodes);

    mobility.SetPositionAllocator("ns3::UniformDiscPositionAllocator",
                                  "rho", DoubleValue(radiusMeters),
                                  "X", DoubleValue(0.0),
                                  "Y", DoubleValue(0.0));
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(endDevices);

    phyHelper.SetDeviceType(LoraPhyHelper::ED);
    macHelper.SetDeviceType(LorawanMacHelper::ED_A);
    helper.Install(phyHelper, macHelper, endDevices);

    NodeContainer gateways;
    gateways.Create(nGateways);

    Ptr<ListPositionAllocator> gwAllocator = CreateObject<ListPositionAllocator>();
    gwAllocator->Add(Vector(0.0, 1000.0, 15.0));
    gwAllocator->Add(Vector(0.0, -1000.0, 15.0));
    mobility.SetPositionAllocator(gwAllocator);
    mobility.Install(gateways);

    phyHelper.SetDeviceType(LoraPhyHelper::GW);
    macHelper.SetDeviceType(LorawanMacHelper::GW);
    helper.Install(phyHelper, macHelper, gateways);

    LorawanMacHelper::SetSpreadingFactorsUp(endDevices, gateways, channel);

    NetworkServerHelper nsHelper = NetworkServerHelper();
    ForwarderHelper forHelper = ForwarderHelper();
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));
    
    Ptr<Node> networkServer = CreateObject<Node>();
    P2PGwRegistration_t gwRegistration;
    for (uint32_t i = 0; i < gateways.GetN(); ++i)
    {
        auto container = p2p.Install(networkServer, gateways.Get(i));
        auto serverP2PNetDev = DynamicCast<PointToPointNetDevice>(container.Get(0));
        gwRegistration.emplace_back(serverP2PNetDev, gateways.Get(i));
    }

    nsHelper.SetGatewaysP2P(gwRegistration);
    nsHelper.SetEndDevices(endDevices);
    nsHelper.Install(networkServer);
    forHelper.Install(gateways);

    PeriodicSenderHelper appHelper = PeriodicSenderHelper();
    appHelper.SetPeriod(Seconds(normalPeriod));
    appHelper.SetPacketSize(24);

    ApplicationContainer appContainer = appHelper.Install(endDevices);
    appContainer.Start(Time(0));
    appContainer.Stop(appStopTime);

    // hash task scheduling
    std::map<uint8_t, std::vector<uint32_t>> sfToNodeIds;
    for (uint32_t i = 0; i < endDevices.GetN(); ++i)
    {
        auto dev = DynamicCast<LoraNetDevice>(endDevices.Get(i)->GetDevice(0));
        auto mac = DynamicCast<EndDeviceLorawanMac>(dev->GetMac());
        uint8_t dr = mac->GetDataRate();
        uint8_t sf = mac->GetSfFromDataRate(dr);

        //hash nodes based on sf
        sfToNodeIds[sf].push_back(endDevices.Get(i)->GetId());
    }

    //get the time slot based on sf
    auto sfMultiplier = [&](uint8_t sf) -> uint32_t {
        switch (sf)
        {
            case 7: return 1;
            case 8: return 2;
            case 9: return 3;
            case 10: return 4;
            default: return 4;
        }
    };

    
    for (const auto &entry : sfToNodeIds)
    {
        uint8_t sf = entry.first;
        const auto &nodes = entry.second;
        if (nodes.empty()) continue;

        uint32_t groupSize = static_cast<uint32_t>(nodes.size());
        double slotLen = baseSlotL * sfMultiplier(sf);
        double superframe = slotLen * groupSize;

        for (uint32_t nid : nodes)
        {
            uint32_t slot = nid % groupSize;
            Ptr<Application> app = appContainer.Get(nid);
            Ptr<lorawan::PeriodicSender> ps = app->GetObject<lorawan::PeriodicSender>();
            if (ps)
            {
                Simulator::Schedule(Seconds(slot * slotLen), &lorawan::PeriodicSender::SetInterval, ps, Seconds(superframe));
            }
        }
    }

    // apply the burst threshold to all end devices
    for (uint32_t i = 0; i < endDevices.GetN(); ++i)
    {
        auto dev = DynamicCast<LoraNetDevice>(endDevices.Get(i)->GetDevice(0));
        auto mac = DynamicCast<EndDeviceLorawanMac>(dev->GetMac());
        if (mac)
        {
            mac->SetBurstThreshold(burstThreshold);
        }
    }

    // Select a subset of nodes to become burst nodes based on the percentage set
    uint32_t nBurst = std::max<uint32_t>(1, std::min<uint32_t>(endDevices.GetN(), std::lround(burstFraction * endDevices.GetN())));
    for (uint32_t i = 0; i < endDevices.GetN(); ++i)
    {
        if (i < nBurst)
        {
            Ptr<Application> app = appContainer.Get(i);
            Ptr<lorawan::PeriodicSender> ps = app->GetObject<lorawan::PeriodicSender>();
            if (ps)
            {
                Simulator::Schedule(Seconds(burstStart), &lorawan::PeriodicSender::SetInterval, ps, Seconds(burstPeriod));
            }
        }
    }

    for (auto node = endDevices.Begin(); node != endDevices.End(); node++)
    {
        DynamicCast<LoraNetDevice>((*node)->GetDevice(0))
            ->GetPhy()
            ->TraceConnectWithoutContext("StartSending", MakeCallback(OnTransmissionCallback));
    }

    // Connect to MAC-layer ReceivedPacket
    for (auto node = gateways.Begin(); node != gateways.End(); node++)
    {
        auto mac = DynamicCast<LoraNetDevice>((*node)->GetDevice(0))->GetMac();
        std::string ctx = std::string("GW-") + std::to_string((*node)->GetId());
        mac->TraceConnect("ReceivedPacket", ctx, MakeCallback(OnPacketRecptionCallback));
    }

    Simulator::Stop(appStopTime + Hours(1));
    NS_LOG_INFO("Running simulation...");
    Simulator::Run();
    Simulator::Destroy();

    double pdr = 0.0;
    if (packetsSent > 0)
    {
        pdr = (double)packetsReceived / packetsSent;
    }

    std::cout << "\n--- Simulation Results ---" << std::endl;
    std::cout << "Total packets sent: " << packetsSent << std::endl;
    std::cout << "Total packets received: " << packetsReceived << std::endl;
    std::cout << "Packet Delivery Ratio (PDR): " << pdr * 100.0 << "%" << std::endl;
    std::cout << "Packets with burst bit: " << packetsWithBurstBit << std::endl;
    std::cout << "--------------------------" << std::endl;

    // track vc
    if (!vcReceivedCounts.empty())
    {
        std::cout << "VC groups observed (frequency Hz, SF) -> packet count" << std::endl;
        for (const auto &kv : vcReceivedCounts)
        {
            std::cout << "  (" << kv.first.first << ", SF" << unsigned(kv.first.second)
                      << ") -> " << kv.second << std::endl;
        }
    }

    LoraPacketTracker& tracker = helper.GetPacketTracker(); 
    std::cout << tracker.CountMacPacketsGlobally(Seconds(0), appStopTime + Hours(1)) << std::endl;

    //improvement:
    //1. Gateway will not drop packet
    //2. Assigning SF based on signal quality
    //3. Error correction

    //LogComponentEnable("LoraChannel", LOG_LEVEL_INFO);
    LogComponentEnable("LoraPhy", LOG_LEVEL_INFO);
    //LogComponentEnable("EndDeviceLoraPhy", LOG_LEVEL_ALL);
    //LogComponentEnable("GatewayLoraPhy", LOG_LEVEL_ALL);
    //LogComponentEnable("LoraInterferenceHelper", LOG_LEVEL_ALL);
    //LogComponentEnable("LorawanMac", LOG_LEVEL_ALL);
    //LogComponentEnable("EndDeviceLorawanMac", LOG_LEVEL_ALL);
    //LogComponentEnable("ClassAEndDeviceLorawanMac", LOG_LEVEL_ALL);
    //LogComponentEnable("GatewayLorawanMac", LOG_LEVEL_ALL);
    //LogComponentEnable("LogicalLoraChannelHelper", LOG_LEVEL_ALL);
    //LogComponentEnable("LogicalLoraChannel", LOG_LEVEL_ALL);
    //LogComponentEnable("LoraHelper", LOG_LEVEL_ALL);
    //LogComponentEnable("LoraPhyHelper", LOG_LEVEL_ALL);
    //LogComponentEnable("LorawanMacHelper", LOG_LEVEL_ALL);
    //LogComponentEnable("OneShotSenderHelper", LOG_LEVEL_ALL);
    //LogComponentEnable("OneShotSender", LOG_LEVEL_ALL);
    LogComponentEnable("LorawanMacHeader", LOG_LEVEL_ALL);
    //LogComponentEnable("LoraFrameHeader", LOG_LEVEL_ALL);

    //LogComponentEnableAll(LOG_PREFIX_FUNC);
    //LogComponentEnableAll(LOG_PREFIX_NODE);
    //LogComponentEnableAll(LOG_PREFIX_TIME);

    return 0;
}