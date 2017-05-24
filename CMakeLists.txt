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

## cmake flags
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

## project name
project("paho-mqtt-cpp" LANGUAGES CXX)

## library name
set(PAHO_MQTT_CPP paho-mqttpp3)

## build settings
set(PAHO_VERSION_MAJOR 0)
set(PAHO_VERSION_MINOR 9)
set(PAHO_VERSION_PATCH 0)

set(CLIENT_VERSION ${PAHO_VERSION_MAJOR}.${PAHO_VERSION_MINOR}.${PAHO_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_MAJOR ${PAHO_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PAHO_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PAHO_VERSION_PATCH})

## build options
set(PAHO_BUILD_STATIC FALSE CACHE BOOL "Build static library")
set(PAHO_BUILD_SAMPLES FALSE CACHE BOOL "Build sample programs")
set(PAHO_BUILD_DOCUMENTATION FALSE CACHE BOOL "Create and install the HTML based API documentation (requires Doxygen)")
set(PAHO_MQTT_C_PATH "" CACHE PATH "Add a path to paho.mqtt.c library and headers")
set(PAHO_MQTT_C paho-mqtt3a)

## extract Paho MQTT C include directory
get_filename_component(PAHO_MQTT_C_DEV_INC_DIR ${PAHO_MQTT_C_PATH}/src ABSOLUTE)
get_filename_component(PAHO_MQTT_C_STD_INC_DIR ${PAHO_MQTT_C_PATH}/include ABSOLUTE)
set(PAHO_MQTT_C_INC_DIR
    ${PAHO_MQTT_C_DEV_INC_DIR}
    ${PAHO_MQTT_C_STD_INC_DIR})

## extract Paho MQTT C library directory
get_filename_component(PAHO_MQTT_C_DEV_LIB_DIR ${PAHO_MQTT_C_PATH}/build/output ABSOLUTE)
get_filename_component(PAHO_MQTT_C_STD_LIB_DIR ${PAHO_MQTT_C_PATH}/lib ABSOLUTE)
get_filename_component(PAHO_MQTT_C_STD64_LIB_DIR ${PAHO_MQTT_C_PATH}/lib64 ABSOLUTE)
set(PAHO_MQTT_C_LIB_DIR
    ${PAHO_MQTT_C_DEV_LIB_DIR}
    ${PAHO_MQTT_C_STD_LIB_DIR}
    ${PAHO_MQTT_C_STD64_LIB_DIR})

## extract Paho MQTT C binary directory (Windows may place libraries there)
get_filename_component(PAHO_MQTT_C_BIN_DIR ${PAHO_MQTT_C_PATH}/bin ABSOLUTE)

## try to find the Paho MQTT C library with SSL support
find_library(PAHO_MQTT_C_LIB
    NAMES paho-mqtt3as
          mqtt3as
    PATHS ${PAHO_MQTT_C_LIB_DIR}
          ${PAHO_MQTT_C_BIN_DIR})

if(${PAHO_MQTT_C_LIB} STREQUAL "PAHO_MQTT_C_LIB-NOTFOUND")
    ## try to find the Paho MQTT C library without SSL support
    find_library(PAHO_MQTT_C_LIB
        NAMES paho-mqtt3a
              mqtt
              paho-mqtt
              mqtt3
              paho-mqtt3
              mqtt3a
        PATHS ${PAHO_MQTT_C_LIB_DIR}
              ${PAHO_MQTT_C_BIN_DIR})

    if(${PAHO_MQTT_C_LIB} STREQUAL "PAHO_MQTT_C_LIB-NOTFOUND")
        message(FATAL_ERROR "Could not find Paho MQTT C library")
    else()
        set(PAHO_WITH_SSL FALSE CACHE BOOL "Flag that defines whether to build ssl-enabled binaries too. ")
    endif()
else()
    find_package(OpenSSL REQUIRED)
    set(PAHO_WITH_SSL TRUE CACHE BOOL "Flag that defines whether to build ssl-enabled binaries too. ")
endif()

## build flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## build directories

add_subdirectory(src)
add_subdirectory(src/mqtt)

if(PAHO_BUILD_SAMPLES)
    add_subdirectory(src/samples)
endif()

if(PAHO_BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()

## packaging settings
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif(UNIX)
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
