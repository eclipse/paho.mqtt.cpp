# CMakeLists.txt
#
# CMake file for the Paho C++ core library.
#
# This is part of the Paho MQTT C++ client library.
#

#*******************************************************************************
# Copyright (c) 2016-2017, Guilherme Maciel Ferreira
# Copyright (c) 2017-2023, Frank Pagliughi
#
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the Eclipse Public License v2.0
#  and Eclipse Distribution License v1.0 which accompany this distribution. 
# 
#  The Eclipse Public License is available at 
#     http://www.eclipse.org/legal/epl-v20.html
#  and the Eclipse Distribution License is available at 
#    http://www.eclipse.org/org/documents/edl-v10.php.
# 
#  Contributors:
#     Guilherme Maciel Ferreira - initial version
#     Frank Pagliughi - made the shared library optional
#*******************************************************************************/

# --- The headers ---

add_subdirectory(mqtt)

## --- Library dependencies ---

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

## --- Use object library to optimize compilation ---

set(COMMON_SRC
  async_client.cpp
  client.cpp
  connect_options.cpp
  create_options.cpp    
  disconnect_options.cpp
  iclient_persistence.cpp
  message.cpp
  properties.cpp
  response_options.cpp
  ssl_options.cpp
  string_collection.cpp
  subscribe_options.cpp
  token.cpp
  topic.cpp
  will_options.cpp
)

## --- Build the shared library, if requested ---

if(PAHO_BUILD_SHARED)
    ## Create the shared library
    add_library(paho-mqttpp3 SHARED ${COMMON_SRC})
    list(APPEND PAHO_CPP_TARGETS paho-mqttpp3)

    ## Add dependencies to the shared library
    target_link_libraries(paho-mqttpp3 PUBLIC
      eclipse-paho-mqtt-c::paho-mqtt3as
      ${PAHO_MQTT_C_LIB}
      Threads::Threads
    )

    ## set the shared library soname
    set_target_properties(paho-mqttpp3 PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )
endif()

## --- Build static version of the library, if requested ---

if(PAHO_BUILD_STATIC)
    ## create the static library
    add_library(paho-mqttpp3-static STATIC ${COMMON_SRC})
    list(APPEND PAHO_CPP_TARGETS paho-mqttpp3-static)

    ## add dependencies to the shared library
    target_link_libraries(paho-mqttpp3-static PUBLIC 
      ${PAHO_MQTT_C_LIB}
      Threads::Threads
    )

    ## Let the archive use the same base name as the shared lib on *nix systems
    if(UNIX)
        set_target_properties(paho-mqttpp3-static PROPERTIES OUTPUT_NAME paho-mqttpp3)
    endif()
endif()

## --- Set common properties, etc ---

include(GNUInstallDirs)

foreach(TARGET ${PAHO_CPP_TARGETS})
  ## Warnings
  target_compile_options(${TARGET} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wdocumentation>
    $<$<NOT:$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:Clang>>>:-Wall -Wextra>
  )

  target_include_directories(${TARGET} PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>
  )

  ## install the shared library
  install(TARGETS ${TARGET} EXPORT PahoMqttCpp
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endforeach()
