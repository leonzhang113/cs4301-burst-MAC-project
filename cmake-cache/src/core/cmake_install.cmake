# Install script for directory: /Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core

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
  include("/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/cmake-cache/src/core/examples/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/build/lib/libns3.46.1-core-default.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.46.1-core-default.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.46.1-core-default.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/usr/local/lib:$ORIGIN/:$ORIGIN/../lib:/usr/local/lib64:$ORIGIN/:$ORIGIN/../lib64"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.46.1-core-default.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.46.1-core-default.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/build/include/ns3/core-config.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/int64x64-128.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/example-as-test.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/helper/csv-reader.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/helper/event-garbage-collector.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/helper/random-variable-stream-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/abort.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/ascii-file.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/ascii-test.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/assert.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/attribute-accessor-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/attribute-construction-list.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/attribute-container.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/attribute-helper.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/attribute.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/boolean.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/breakpoint.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/build-profile.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/calendar-scheduler.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/callback.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/command-line.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/config.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/default-deleter.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/default-simulator-impl.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/demangle.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/deprecated.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/des-metrics.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/double.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/enum.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/event-id.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/event-impl.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/fatal-error.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/fatal-impl.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/fd-reader.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/environment-variable.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/global-value.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/hash-fnv.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/hash-function.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/hash-murmur3.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/hash.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/heap-scheduler.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/int64x64-double.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/int64x64.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/integer.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/length.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/list-scheduler.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/log-macros-disabled.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/log-macros-enabled.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/log.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/make-event.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/map-scheduler.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/math.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/names.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/node-printer.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/nstime.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/object-base.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/object-factory.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/object-map.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/object-ptr-container.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/object-vector.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/object.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/pair.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/pointer.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/priority-queue-scheduler.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/ptr.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/random-variable-stream.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/rng-seed-manager.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/rng-stream.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/scheduler.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/show-progress.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/shuffle.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/simple-ref-count.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/simulation-singleton.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/simulator-impl.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/simulator.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/singleton.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/string.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/synchronizer.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/system-path.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/system-wall-clock-ms.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/system-wall-clock-timestamp.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/test.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/time-printer.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/timer-impl.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/timer.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/trace-source-accessor.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/traced-callback.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/traced-value.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/trickle-timer.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/tuple.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/type-id.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/type-name.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/type-traits.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/uinteger.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/uniform-random-bit-generator.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/valgrind.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/vector.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/warnings.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/watchdog.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/realtime-simulator-impl.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/wall-clock-synchronizer.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/val-array.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/src/core/model/matrix-array.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/build/include/ns3/core-module.h"
    "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/build/include/ns3/core-export.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/leonzhang/Documents/school/cs4301/project/cs4301-burst-MAC-project/ns3-temp/cmake-cache/src/core/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
