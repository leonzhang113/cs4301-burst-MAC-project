# Install script for directory: /Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "default")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/cmake-cache/src/lorawan/examples/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/build/lib/libns3.46.1-lorawan-default.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.46.1-lorawan-default.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.46.1-lorawan-default.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/build/lib"
      -add_rpath "/usr/local/lib:$ORIGIN/:$ORIGIN/../lib:/usr/local/lib64:$ORIGIN/:$ORIGIN/../lib64"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.46.1-lorawan-default.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.46.1-lorawan-default.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-net-device.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lorawan-mac.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-phy.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/building-penetration-loss.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/correlated-shadowing-propagation-loss-model.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-channel.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-interference-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/gateway-lorawan-mac.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/end-device-lorawan-mac.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/class-a-end-device-lorawan-mac.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/gateway-lora-phy.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/end-device-lora-phy.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/simple-end-device-lora-phy.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/simple-gateway-lora-phy.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/sub-band.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/logical-lora-channel.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/logical-lora-channel-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/periodic-sender.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/one-shot-sender.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/forwarder.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lorawan-mac-header.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-frame-header.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/mac-command.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-device-address.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-device-address-generator.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-tag.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/network-server.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/network-status.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/network-controller.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/network-controller-components.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/network-scheduler.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/end-device-status.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/gateway-status.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-radio-energy-model.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-tx-current-model.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/lora-utils.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/adr-component.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/model/hex-grid-position-allocator.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/helper/lora-radio-energy-model-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/helper/lora-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/helper/lora-phy-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/helper/lorawan-mac-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/helper/periodic-sender-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/helper/one-shot-sender-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/helper/forwarder-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/helper/network-server-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/helper/lora-packet-tracker.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/lorawan/test/utilities.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/build/include/ns3/lorawan-module.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/cmake-cache/src/lorawan/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
