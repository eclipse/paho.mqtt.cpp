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
cmake_minimum_required(VERSION 3.5)

## project name
project("paho-mqtt-cpp" 
    VERSION "1.2.0"
    LANGUAGES CXX
)

## --- Build options ---

if(WIN32)
  option(PAHO_BUILD_STATIC "Build static library" TRUE)
  option(PAHO_BUILD_SHARED "Build shared library (DLL)" FALSE)
  option(PAHO_WITH_SSL "Build SSL-enabled library" FALSE)
else()
  option(PAHO_BUILD_STATIC "Build static library" FALSE)
  option(PAHO_BUILD_SHARED "Build shared library" TRUE)
  option(PAHO_WITH_SSL "Build SSL-enabled library" TRUE)
  option(PAHO_BUILD_DEB_PACKAGE "Build debian package" FALSE)
endif()

option(PAHO_BUILD_SAMPLES "Build sample programs" FALSE)
option(PAHO_BUILD_TESTS "Build tests" FALSE)
option(PAHO_BUILD_DOCUMENTATION "Create and install the API documentation (requires Doxygen)" FALSE)

## --- C++11 build flags ---

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate position-independent code (-fPIC on UNIX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --- System Libraries ---

include(GNUInstallDirs)

if(WIN32)
	  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(LIBS_SYSTEM ws2_32)
elseif(UNIX)
    set(LIBS_SYSTEM c stdc++)
endif()

## --- Build directories ---

# For the paho_mqtt_c module
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
add_subdirectory(src)

# --- Documentation ---

if(PAHO_BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()

# --- Default library for samples and unit tests ---

if(PAHO_BUILD_SHARED)
    set(PAHO_CPP_LIB paho-mqttpp3)
else()
    set(PAHO_CPP_LIB paho-mqttpp3-static)
endif()

# --- Sample Apps ---

if(PAHO_BUILD_SAMPLES)
    add_subdirectory(src/samples)
endif()

# --- Unit Tests ---

if(PAHO_BUILD_TESTS)
    add_subdirectory(test/unit)
endif()

## --- Packaging settings ---

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
elseif(UNIX)
    if(PAHO_BUILD_DEB_PACKAGE)
        set(CPACK_GENERATOR "DEB")
        include(cmake/CPackDebConfig.cmake)
    else()
        set(CPACK_GENERATOR "TGZ")
	endif()
endif()

include(CPack)
add_subdirectory(cmake)


