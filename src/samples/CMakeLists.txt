# CMakeLists.txt
#
# CMake file for the Paho C++ sample applications.
#
#*******************************************************************************
# This is part of the Paho MQTT C++ client library.
#
# Copyright (c) 2016
# 
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# and Eclipse Distribution License v1.0 which accompany this distribution.
# 
# The Eclipse Public License is available at
#   http://www.eclipse.org/legal/epl-v10.html
# and the Eclipse Distribution License is available at
#   http://www.eclipse.org/org/documents/edl-v10.php.
# 
# Contributors:
#   Guilherme Maciel Ferreira - initial version
#*******************************************************************************/

## Note: on OS X you should install XCode and the associated command-line tools

## --- Library dependencies ---

if(NOT WIN32)
    set(LIBS_THREADING pthread)
endif()


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
add_executable(async_consume async_consume.cpp)
add_executable(sync_publish sync_publish.cpp)
add_executable(sync_consume sync_consume.cpp)
add_executable(data_publish data_publish.cpp)
add_executable(pub_speed_test pub_speed_test.cpp)

## link binaries
target_link_libraries(async_publish
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP})
target_link_libraries(async_subscribe
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP})
target_link_libraries(async_consume
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP})
target_link_libraries(sync_publish
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP})
target_link_libraries(sync_consume
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP})
target_link_libraries(data_publish
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP})
target_link_libraries(pub_speed_test
    ${PAHO_MQTT_C}
    ${PAHO_MQTT_CPP}
    ${LIBS_THREADING})

set(INSTALL_TARGETS
    async_publish
    async_subscribe
    async_consume
    sync_publish
    sync_consume
    data_publish
    pub_speed_test)

if(PAHO_WITH_SSL)
    ## SSL binary files
    add_executable(ssl_publish ssl_publish.cpp)

    ## link SSL binaries
    target_link_libraries(ssl_publish
        ${PAHO_MQTT_C}
        ${PAHO_MQTT_CPP})

    set(INSTALL_TARGETS
        ${INSTALL_TARGETS}
        ssl_publish)

    add_definitions(-DOPENSSL)
endif()

## install binaries
install(TARGETS ${INSTALL_TARGETS}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

