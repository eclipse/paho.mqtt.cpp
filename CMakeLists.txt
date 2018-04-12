# CMakeLists.txt
#
# Top-level CMake file for the Paho C++ library.
#
#*******************************************************************************
# This is part of the Paho MQTT C++ client library.
#
# Copyright (c) 2016-2017, Guilherme Maciel Ferreira
# Copyright (c) 2017, Frank Pagliughi
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
#   Frank Pagliughi
#*******************************************************************************/

## Note: on OS X you should install XCode and the associated command-line tools

## cmake flags
cmake_minimum_required(VERSION 3.1)

## project name
project("paho-mqtt-cpp" LANGUAGES CXX)

## library name
set(PAHO_MQTT_CPP paho-mqttpp3)
set(PAHO_MQTT_CPP_STATIC ${PAHO_MQTT_CPP}-static)

## build settings
set(PAHO_VERSION_MAJOR 1)
set(PAHO_VERSION_MINOR 0)
set(PAHO_VERSION_PATCH 0)

set(CLIENT_VERSION ${PAHO_VERSION_MAJOR}.${PAHO_VERSION_MINOR}.${PAHO_VERSION_PATCH})

set(CPACK_PACKAGE_VERSION_MAJOR ${PAHO_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PAHO_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PAHO_VERSION_PATCH})

## --- Build options ---

if(WIN32)
  set(PAHO_BUILD_STATIC TRUE  CACHE BOOL "Build static library")
  set(PAHO_BUILD_SHARED FALSE CACHE BOOL "Build shared library (DLL)")
  set(PAHO_WITH_SSL FALSE CACHE BOOL "Build SSL-enabled library")
else()
  set(PAHO_BUILD_STATIC FALSE CACHE BOOL "Build static library")
  set(PAHO_BUILD_SHARED TRUE CACHE BOOL "Build shared library")
  set(PAHO_WITH_SSL TRUE CACHE BOOL "Build SSL-enabled library")
endif()

set(PAHO_BUILD_SAMPLES FALSE CACHE BOOL "Build sample programs")
set(PAHO_BUILD_DOCUMENTATION FALSE CACHE BOOL "Create and install the API documentation (requires Doxygen)")
set(PAHO_MQTT_C_PATH "" CACHE PATH "Add a path to paho.mqtt.c library and headers")

set(PAHO_MQTT_C paho-mqtt3a)

## --- C++11 build flags ---

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## --- Build directories ---

add_subdirectory(src)
add_subdirectory(src/mqtt)

if(PAHO_BUILD_SAMPLES)
    add_subdirectory(src/samples)
endif()

if(PAHO_BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()

## --- Packaging settings ---

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif(UNIX)
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)
