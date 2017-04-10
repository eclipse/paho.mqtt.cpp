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
#     Guilherme Maciel Ferreira - initial version
#*******************************************************************************/

## Note: on OS X you should install XCode and the associated command-line tools

## Paho MQTT C include directory
get_filename_component(PAHO_MQTT_C_DEV_INC_DIR ${PAHO_MQTT_C_PATH}/src ABSOLUTE)
get_filename_component(PAHO_MQTT_C_STD_INC_DIR ${PAHO_MQTT_C_PATH}/include ABSOLUTE)
set(PAHO_MQTT_C_INC_DIR
    ${PAHO_MQTT_C_DEV_INC_DIR}
    ${PAHO_MQTT_C_STD_INC_DIR})

## Paho MQTT C++ include directory
get_filename_component(PAHO_MQTT_CPP_INC_DIR ${CMAKE_SOURCE_DIR}/src ABSOLUTE)

## include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PAHO_MQTT_C_INC_DIR})
include_directories(${PAHO_MQTT_CPP_INC_DIR})

## Paho MQTT C library directory
get_filename_component(PAHO_MQTT_C_LIB_DIR ${PAHO_MQTT_C_LIB} DIRECTORY)

## Paho MQTT C++ library directory
get_filename_component(PAHO_MQTT_CPP_LIB_DIR ${CMAKE_BINARY_DIR}/src ABSOLUTE)

## link directories
link_directories(${PAHO_MQTT_C_LIB_DIR})
link_directories(${PAHO_MQTT_CPP_LIB_DIR})

## binary files
add_executable(async_publish async_publish.cpp)
add_executable(async_subscribe async_subscribe.cpp)
add_executable(sync_publish sync_publish.cpp)

## link binaries
target_link_libraries(async_publish
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP})
target_link_libraries(async_subscribe
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP})
target_link_libraries(sync_publish
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP})

## install binaries
install(TARGETS async_publish
                async_subscribe
                sync_publish
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
