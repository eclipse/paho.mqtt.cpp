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

## extract Paho MQTT C include directory
get_filename_component(PAHO_MQTT_C_INC_DIR ${PAHO_MQTT_C_PATH}/include ABSOLUTE)

## extract Paho MQTT C library directory
get_filename_component(PAHO_MQTT_C_LIB_DIR ${PAHO_MQTT_C_PATH}/lib ABSOLUTE)

## extract Paho MQTT C binary directory (Windows may place libraries there)
get_filename_component(PAHO_MQTT_C_BIN_DIR ${PAHO_MQTT_C_PATH}/bin ABSOLUTE)

## add library suffixes so Windows can find Paho DLLs
set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES} ".dll" ".lib")

## find the Paho MQTT C library
find_library(PAHO_MQTT_C_LIB
    NAMES paho-mqtt3a
          mqtt
          paho-mqtt
          mqtt3
          paho-mqtt3
          mqtt3a
    PATHS ${PAHO_MQTT_C_LIB_DIR}
          ${PAHO_MQTT_C_BIN_DIR})

## use the Paho MQTT C library if found. Otherwise terminate the compilation
if(${PAHO_MQTT_C_LIB} STREQUAL "PAHO_MQTT_C_LIB-NOTFOUND")
    message(FATAL_ERROR "Could not find Paho MQTT C library")
else()
    include_directories(${PAHO_MQTT_C_INC_DIR})
    link_directories(${PAHO_MQTT_C_LIB_DIR})
    target_link_libraries(${PAHO_MQTT_CPP}
        ${PAHO_MQTT_C_LIB})
endif()
