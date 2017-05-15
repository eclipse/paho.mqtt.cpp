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
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

## project name
project("paho-mqtt-cpp")

## project language
enable_language(CXX)

## library name
set(PAHO_MQTT_CPP paho-mqttpp3)

## build settings
set(PAHO_VERSION_MAJOR 0)
set(PAHO_VERSION_MINOR 5)
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
set(PAHO_WITH_SSL TRUE CACHE BOOL "Flag that defines whether to build ssl-enabled binaries too. ")

## build flags
## for GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC versions older than 4.8 do not compile the code
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
        message(FATAL_ERROR "Minimum GCC version required is 4.8")
    # GCC versions prior to 6.1 require -std=c++11
    elseif (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.1)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
# for Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang versions older than 3.3 do not compile the code
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)
        message(FATAL_ERROR "Minimum Clang version required is 3.3")
    # Clang versions prior to 5.0 require -std=c++11
    elseif (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
endif()

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
