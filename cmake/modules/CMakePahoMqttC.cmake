#*******************************************************************************
#  Copyright (c) 2016-2017 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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

## Paho MQTT C include directory
get_filename_component(PAHO_MQTT_C_DEV_INC_DIR ${PAHO_MQTT_C_PATH}/src ABSOLUTE)
get_filename_component(PAHO_MQTT_C_STD_INC_DIR ${PAHO_MQTT_C_PATH}/include ABSOLUTE)
set(PAHO_MQTT_C_INC_DIR
    ${PAHO_MQTT_C_DEV_INC_DIR}
    ${PAHO_MQTT_C_STD_INC_DIR})

## Paho MQTT C library directory
get_filename_component(PAHO_MQTT_C_DEV_LIB_DIR ${PAHO_MQTT_C_PATH}/build/output ABSOLUTE)
get_filename_component(PAHO_MQTT_C_STD_LIB_DIR ${PAHO_MQTT_C_PATH}/lib ABSOLUTE)
get_filename_component(PAHO_MQTT_C_STD64_LIB_DIR ${PAHO_MQTT_C_PATH}/lib64 ABSOLUTE)
set(PAHO_MQTT_C_LIB_DIR
    ${PAHO_MQTT_C_DEV_LIB_DIR}
    ${PAHO_MQTT_C_STD_LIB_DIR}
    ${PAHO_MQTT_C_STD64_LIB_DIR})

## Paho MQTT C binary directory (Windows may place libraries there)
get_filename_component(PAHO_MQTT_C_BIN_DIR ${PAHO_MQTT_C_PATH}/bin ABSOLUTE)

if(PAHO_WITH_SSL)
    ## find the Paho MQTT C SSL library
    find_library(PAHO_MQTT_C_LIB
        NAMES paho-mqtt3as
              mqtt3as
        PATHS ${PAHO_MQTT_C_LIB_DIR}
              ${PAHO_MQTT_C_BIN_DIR})

    find_package(OpenSSL REQUIRED)
else()
    ## find the Paho MQTT C library
    find_library(PAHO_MQTT_C_LIB
        NAMES paho-mqtt3a
              mqtt3a
        PATHS ${PAHO_MQTT_C_LIB_DIR}
              ${PAHO_MQTT_C_BIN_DIR})
endif()

## use the Paho MQTT C library if found. Otherwise terminate the compilation
if(${PAHO_MQTT_C_LIB} STREQUAL "PAHO_MQTT_C_LIB-NOTFOUND")
    message(FATAL_ERROR "Could not find Paho MQTT C library")
else()
    ## include directories
    include_directories(${PAHO_MQTT_C_INC_DIR})

    ## link directories
    link_directories(${PAHO_MQTT_C_LIB_DIR})
    link_directories(${PAHO_MQTT_C_BIN_DIR})
endif()
