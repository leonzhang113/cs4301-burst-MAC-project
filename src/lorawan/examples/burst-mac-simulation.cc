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

// Part 5: Collision resolution structures
struct VCScheduleInfo
{
    double slotLength;
    double superframeLength;
    uint32_t groupSize;
};
std::map<uint8_t, VCScheduleInfo> sfScheduleInfo; // SF -> schedule info
std::map<uint32_t, double> nodeSlotOffsets;       // nodeId -> assigned slot offset in seconds

// Part 6: Mode switching and beaconing
static bool g_inBurstMode = false;
static bool g_beaconingEnabled = false;
static double g_beaconPeriodSeconds = 60.0;
static double g_burstModeEndTime = 300.0;
static bool g_anyClassAReported = false;
static uint32_t g_numReassignedNodes = 0;

// Part 6: Gateway beacon function
static void SendGatewayBeacon()
{
    if (!g_beaconingEnabled || !g_inBurstMode)
    {
        return;
    }

    std::cout << "[t=" << Simulator::Now().GetSeconds() 
              << "s] Gateway Beacon (Class B synchronization)" << std::endl;

    // Schedule next beacon
    Simulator::Schedule(Seconds(g_beaconPeriodSeconds), &SendGatewayBeacon);
}

// Part 6: Switch back to Class A
static void SwitchToClassA()
{
    if (!g_inBurstMode)
    {
        return;
    }

    g_inBurstMode = false;
    g_beaconingEnabled = false;

    std::cout << "[t=" << Simulator::Now().GetSeconds() 
              << "s] Gateway switching back to Class A (stopping beacons)" << std::endl;
}

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
        g_inBurstMode = true; // Part 6: Node signals burst mode
        std::cout << "Burst bit detected at t="
                  << Simulator::Now().GetSeconds() << "s" << std::endl;
    }
    else
    {
        // Part 6: Node reports back to normal (Class A)
        g_anyClassAReported = true;
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
    double simulationTimeSeconds = 600.0;
    Time appStopTime = Seconds(simulationTimeSeconds);

    // Burst scenario parameters (configurable via CLI)
    double burstFraction = 0.5;   // [0..1] fraction of nodes that will burst
    double burstStart = 300.0;     // set a time for burst signal to start
    double normalPeriod = 10;   // set length of normal signal period
    double burstPeriod = 0.1;     // length of burst signal period
    double burstThreshold = 1.0;  // threshold for packets/sec that nodes need to set to burst
    double baseSlotL = 0.2;       // base slot duration (seconds) for hash scheduling
    
    // Part 6: Mode switching parameters
    double beaconPeriod = 60.0;   // seconds between gateway beacons
    double burstDuration = 450.0; // time when nodes return to Class A
    bool enableBurstMac = true;   // enable/disable Burst-MAC scheduling and collision resolution

    CommandLine cmd(__FILE__);
    cmd.AddValue("nNodes", "Number of end devices", nNodes);
    cmd.AddValue("x", "Number of gateways", nGateways);
    cmd.AddValue("radius", "Radius of the deployment area in meters", radiusMeters);
    cmd.AddValue("burstFraction", "Fraction of nodes that will burst (0.0-1.0)", burstFraction);
    cmd.AddValue("baseSlotLength", "Base slot length L (seconds) for SF7", baseSlotL);
    cmd.AddValue("enableBurstMac", "Enable Burst-MAC scheduling and collision resolution", enableBurstMac);
    cmd.AddValue("beaconPeriod", "Gateway beacon period in seconds", beaconPeriod);
    cmd.AddValue("burstDuration", "Time when nodes return to Class A", burstDuration);
    cmd.Parse(argc, argv);
    
    // Part 6: Initialize mode switching globals
    g_beaconPeriodSeconds = beaconPeriod;
    g_burstModeEndTime = burstDuration;
    g_inBurstMode = enableBurstMac;
    g_beaconingEnabled = enableBurstMac;

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

    // Part 4 + Part 5: Hash-based scheduling with collision resolution
    if (enableBurstMac)
    {
        std::cout << "\n=== Part 5: Hash-based Scheduling with Collision Resolution ===" << std::endl;
        
        for (const auto &entry : sfToNodeIds)
        {
            uint8_t sf = entry.first;
            const auto &nodes = entry.second;
            if (nodes.empty()) continue;

            uint32_t groupSize = static_cast<uint32_t>(nodes.size());
            double slotLen = baseSlotL * sfMultiplier(sf);
            double superframe = slotLen * groupSize;

            // Store schedule info
            VCScheduleInfo info;
            info.slotLength = slotLen;
            info.superframeLength = superframe;
            info.groupSize = groupSize;
            sfScheduleInfo[sf] = info;

            // Part 5.1: Gateway pre-computes schedule using hash function
            // slot = nodeId % groupSize
            std::map<uint32_t, std::vector<uint32_t>> slotToNodes;
            for (uint32_t nodeId : nodes)
            {
                uint32_t slotIndex = nodeId % groupSize;
                slotToNodes[slotIndex].push_back(nodeId);
            }

            // Part 5.2: Identify unused slots in the superframe
            std::vector<uint32_t> unusedSlots;
            for (uint32_t s = 0; s < groupSize; ++s)
            {
                if (slotToNodes.find(s) == slotToNodes.end())
                {
                    unusedSlots.push_back(s);
                }
            }

            std::cout << "SF" << unsigned(sf) << ": groupSize=" << groupSize 
                      << ", unusedSlots=" << unusedSlots.size() << std::endl;

            // Part 5.3: Resolve collisions by reassigning to unused slots
            for (auto &p : slotToNodes)
            {
                uint32_t slotIndex = p.first;
                std::vector<uint32_t> &slotNodes = p.second;

                if (slotNodes.size() > 1)
                {
                    std::cout << "  Collision detected in slot " << slotIndex 
                              << " (SF" << unsigned(sf) << "): " 
                              << slotNodes.size() << " nodes" << std::endl;
                }

                // First node stays in original slot
                // Remaining nodes are reassigned to unused slots
                for (size_t k = 1; k < slotNodes.size() && !unusedSlots.empty(); ++k)
                {
                    uint32_t nodeId = slotNodes[k];
                    uint32_t newSlot = unusedSlots.back();
                    unusedSlots.pop_back();

                    double newOffset = static_cast<double>(newSlot) * slotLen;
                    nodeSlotOffsets[nodeId] = newOffset;
                    g_numReassignedNodes++;

                    // Part 5.4: Conceptually, this reassignment would be piggybacked in ACK
                    std::cout << "    Node " << nodeId << " reassigned: slot " 
                              << slotIndex << " -> slot " << newSlot 
                              << " (ACK would carry new assignment)" << std::endl;
                }
            }

            // Apply slot assignments to all nodes in this SF group
            for (uint32_t nodeId : nodes)
            {
                double offset;
                if (nodeSlotOffsets.find(nodeId) != nodeSlotOffsets.end())
                {
                    // Node was reassigned
                    offset = nodeSlotOffsets[nodeId];
                }
                else
                {
                    // Node uses original hash slot
                    uint32_t slotIndex = nodeId % groupSize;
                    offset = static_cast<double>(slotIndex) * slotLen;
                    nodeSlotOffsets[nodeId] = offset;
                }

                // Find application and set schedule
                for (uint32_t i = 0; i < endDevices.GetN(); ++i)
                {
                    if (endDevices.Get(i)->GetId() == nodeId)
                    {
                        Ptr<Application> app = appContainer.Get(i);
                        Ptr<lorawan::PeriodicSender> ps = app->GetObject<lorawan::PeriodicSender>();
                        if (ps)
                        {
                            Simulator::Schedule(Seconds(offset), 
                                              &lorawan::PeriodicSender::SetInterval, 
                                              ps, Seconds(superframe));
                        }
                        break;
                    }
                }
            }
        }
        
        std::cout << "Total nodes reassigned: " << g_numReassignedNodes << std::endl;
        std::cout << "============================================\n" << std::endl;
    }
    else
    {
        // Baseline: simple hash scheduling without collision resolution
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
                    Simulator::Schedule(Seconds(slot * slotLen), 
                                      &lorawan::PeriodicSender::SetInterval, 
                                      ps, Seconds(superframe));
                }
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

    // Part 6: Schedule gateway beaconing and mode switching
    if (enableBurstMac)
    {
        // Start beaconing at simulation start
        Simulator::Schedule(Seconds(0.0), &SendGatewayBeacon);
        
        // Schedule switch back to Class A at burstDuration
        Simulator::Schedule(Seconds(g_burstModeEndTime), &SwitchToClassA);
        
        std::cout << "\n=== Part 6: Mode Switching ===" << std::endl;
        std::cout << "Gateway beaconing enabled (period: " << beaconPeriod << "s)" << std::endl;
        std::cout << "Will switch to Class A at t=" << burstDuration << "s" << std::endl;
        std::cout << "================================\n" << std::endl;
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
    
    // Part 5: Collision resolution summary
    if (enableBurstMac)
    {
        std::cout << "\n--- Part 5: Collision Resolution Summary ---" << std::endl;
        std::cout << "Base slot length L (SF7): " << baseSlotL << " s" << std::endl;
        std::cout << "Nodes reassigned to avoid collisions: " << g_numReassignedNodes << std::endl;
        
        if (!sfScheduleInfo.empty())
        {
            std::cout << "\nSchedule per SF:" << std::endl;
            for (const auto &entry : sfScheduleInfo)
            {
                uint8_t sf = entry.first;
                const VCScheduleInfo &info = entry.second;
                std::cout << "  SF" << unsigned(sf) 
                          << ": groupSize=" << info.groupSize
                          << ", slotLength=" << info.slotLength << "s"
                          << ", superframeLength=" << info.superframeLength << "s" << std::endl;
            }
        }
        std::cout << "-------------------------------------------" << std::endl;
    }
    
    // Part 6: Mode switching summary
    std::cout << "\n--- Part 6: Mode Switching Summary ---" << std::endl;
    std::cout << "Burst-MAC enabled: " << (enableBurstMac ? "YES" : "NO") << std::endl;
    std::cout << "Beacon period: " << g_beaconPeriodSeconds << " s" << std::endl;
    std::cout << "Configured burst end time: " << g_burstModeEndTime << " s" << std::endl;
    std::cout << "Any node reported Class A (burst bit=0): " 
              << (g_anyClassAReported ? "YES" : "NO") << std::endl;
    std::cout << "Final burst mode state: " 
              << (g_inBurstMode ? "ACTIVE" : "INACTIVE") << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    
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