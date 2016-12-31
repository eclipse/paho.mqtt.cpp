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

# Note: on OS X you should install XCode and the associated command-line tools

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT("mqttpp" CXX)

## build settings
set(MQTTPP_VERSION_MAJOR 0)
set(MQTTPP_VERSION_MINOR 3)
set(MQTTPP_VERSION_PATCH 0)
set(MQTTPP_VERSION ${MQTTPP_VERSION_MAJOR}.${MQTTPP_VERSION_MINOR}.${MQTTPP_VERSION_PATCH})

## build options
set(MQTTPP_BUILD_DOCUMENTATION FALSE CACHE BOOL "Create and install the HTML based API documentation (requires Doxygen)")
set(MQTTPP_BUILD_SAMPLES FALSE CACHE BOOL "Build sample programs")
set(MQTTPP_BUILD_TESTS FALSE CACHE BOOL "Build test programs")

# packaging settings
set(CPACK_PACKAGE_VERSION_MAJOR ${MQTTPP_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${MQTTPP_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${MQTTPP_VERSION_PATCH})

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

include(CPack)


# search dependencies
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(MQTT REQUIRED)

find_package(Threads REQUIRED)

add_subdirectory(src)

if(MQTTPP_BUILD_SAMPLES)
    add_subdirectory(src/samples)
endif()

if(MQTTPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test/unit)
endif()

if(MQTTPP_BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()
