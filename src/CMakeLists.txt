#*******************************************************************************
#  Copyright (c) 2016
#
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the Eclipse Public License v1.0
#  and Eclipse Distribution License v1.0 which accompany this distribution.
#
#  The Eclipse Public License is available at
#     http://www.eclipse.org/legal/epl-v10.html
#  and the Eclipse Distribution License is available at
#    http://www.eclipse.org/org/documents/edl-v10.php.
#
#  Contributors:
#     Luis Castedo - initial version
#*******************************************************************************/

# compilation/linkage settings
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(version.cpp.in version.cpp @ONLY)

set(COMMON_SOURCE_FILES
  version
  topic
  message
  iclient_persistence
  client
  async_client
  token
)

set(ASYNC_SOURCE_FILES
  ssl_options
  connect_options
  disconnect_options
  response_options
  will_options
)

set(MQTT_PUBLIC_HEADERS)
list(APPEND MQTT_PUBLIC_HEADERS
    mqtt/version.h
    mqtt/token.h
    mqtt/delivery_token.h
    mqtt/iclient_persistence.h
    mqtt/iaction_listener.h
    mqtt/ipersistable.h
    mqtt/exception.h
    mqtt/message.h
    mqtt/callback.h
    mqtt/iasync_client.h
    mqtt/async_client.h
    mqtt/client.h
    mqtt/connect_options.h
    mqtt/will_options.h
    mqtt/ssl_options.h
    mqtt/topic.h
)

add_library(mqttpp SHARED ${COMMON_SOURCE_FILES} ${ASYNC_SOURCE_FILES})
set_target_properties(mqttpp PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    VERSION ${MQTTPP_VERSION}
    SOVERSION ${MQTTPP_VERSION_MAJOR}
    PUBLIC_HEADER "${MQTT_PUBLIC_HEADERS}"
)
target_include_directories(mqttpp BEFORE PUBLIC ${MQTT_INCLUDE_DIRS})
target_link_libraries(mqttpp ${MQTT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS mqttpp EXPORT mqttppTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib COMPONENT devel
    PUBLIC_HEADER DESTINATION include/mqtt COMPONENT devel
)
