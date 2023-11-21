# CMakeLists.txt
#
# CMake file for the Paho C++ sample applications.
#
#*******************************************************************************
# This is part of the Paho MQTT C++ client library.
#
# Copyright (c) 2016-2023
# 
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# and Eclipse Distribution License v1.0 which accompany this distribution.
# 
# The Eclipse Public License is available at
#   http://www.eclipse.org/legal/epl-v20.html
# and the Eclipse Distribution License is available at
#   http://www.eclipse.org/org/documents/edl-v10.php.
# 
# Contributors:
#   Guilherme Maciel Ferreira - initial version
#   Frank Pagliughi - Updates for new samples
#*******************************************************************************/

## Note: on OS X you should install XCode and the associated command-line tools

## --- Library dependencies ---

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

## include directories
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# The sample applications
set(EXECUTABLES
  async_publish
  async_publish_time
  async_subscribe
  async_consume
  async_consume_v5
  sync_publish
  sync_consume
  sync_consume_v5
  sync_reconnect
  data_publish
  rpc_math_cli
  rpc_math_srvr
  mqttpp_chat
  topic_publish
  multithr_pub_sub
  ws_publish
  pub_speed_test
)

# These will only be built if SSL selected
set(SSL_EXECUTABLES
  ssl_publish
)

## Build the samples
foreach(EXECUTABLE ${EXECUTABLES})
  add_executable(${EXECUTABLE} ${EXECUTABLE}.cpp)
  target_link_libraries(${EXECUTABLE} ${PAHO_CPP_LIB})
  if(PAHO_BUILD_SHARED)
    target_compile_definitions(${EXECUTABLE} PUBLIC PAHO_MQTTPP_IMPORTS)
    if(MSVC AND PAHO_BUILD_STATIC)
      target_link_libraries(${EXECUTABLE} ${LIBS_SYSTEM})
    endif()
  endif()
endforeach()

## install binaries
install(TARGETS ${EXECUTABLES} EXPORT PahoMqttCppSamples
  RUNTIME DESTINATION bin
)

## Build the SSL/TLS samples, if selected
if(PAHO_WITH_SSL)
  foreach(EXECUTABLE ${SSL_EXECUTABLES})
    add_executable(${EXECUTABLE} ${EXECUTABLE}.cpp)
    target_link_libraries(${EXECUTABLE} ${PAHO_CPP_LIB})
    if(PAHO_BUILD_SHARED)
      target_compile_definitions(${EXECUTABLE} PUBLIC PAHO_MQTTPP_IMPORTS)
      if(MSVC AND PAHO_BUILD_STATIC)
        target_link_libraries(${EXECUTABLE} ${LIBS_SYSTEM})
      endif()
    endif()
    target_compile_definitions(${EXECUTABLE} PUBLIC OPENSSL)
  endforeach()

  install(TARGETS ${SSL_EXECUTABLES} EXPORT PahoMqttCppSamples
    RUNTIME DESTINATION bin
  )
endif()


