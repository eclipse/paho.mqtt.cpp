# CMakeLists.txt
#
# CMake file for the Paho C++ core library.
#
# This is part of the Paho MQTT C++ client library.
#

#*******************************************************************************
# Copyright (c) 2016-2017, Guilherme Maciel Ferreira
# Copyright (c) 2017, Frank Pagliughi
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
#     Frank Pagliughi - made the shared library optional
#*******************************************************************************/

## Note: on OS X you should install XCode and the associated command-line tools

## include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

## --- Library dependencies ---

if(WIN32)
    set(LIBS_SYSTEM ws2_32)
elseif(UNIX)
    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        set(LIB_DL dl)
    endif()
    set(LIBS_SYSTEM ${LIB_DL} c stdc++ pthread)
endif()

## --- use Object Library to optimize compilation ---

set(SOURCES
    async_client.cpp
    client.cpp
    disconnect_options.cpp
    iclient_persistence.cpp
    message.cpp
    response_options.cpp
    ssl_options.cpp
    string_collection.cpp
    token.cpp
    topic.cpp
    connect_options.cpp
    will_options.cpp)

if(PAHO_WITH_SSL)
    add_definitions(-DOPENSSL)
endif()

add_library(OBJS OBJECT ${SOURCES})

## --- Build the shared library, if requested ---

if(PAHO_BUILD_SHARED)
    ## set position independent flag (-fPIC on Unix)
    set_property(TARGET OBJS PROPERTY POSITION_INDEPENDENT_CODE ON)

    add_library(${PAHO_MQTT_CPP} SHARED $<TARGET_OBJECTS:OBJS>)

    ## add dependencies to the shared library
    target_link_libraries(${PAHO_MQTT_CPP} ${LIBS_SYSTEM})

    ## set the shared library soname
    set_target_properties(${PAHO_MQTT_CPP} PROPERTIES
        VERSION ${CLIENT_VERSION}
        SOVERSION ${PAHO_VERSION_MAJOR})

    ## install the shared library
    install(TARGETS ${PAHO_MQTT_CPP}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
endif()

## --- Build static version of the library, if requested ---

if(PAHO_BUILD_STATIC)
    ## create the static library
    add_library(${PAHO_MQTT_CPP_STATIC} STATIC $<TARGET_OBJECTS:OBJS>)

    ## add dependencies to the static library
    target_link_libraries(${PAHO_MQTT_CPP_STATIC} ${LIBS_SYSTEM})

    ## install the static library
    install(TARGETS ${PAHO_MQTT_CPP_STATIC}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

    ## Static lib has same name as shared lib
    set_target_properties(${PAHO_MQTT_CPP_STATIC} PROPERTIES OUTPUT_NAME ${PAHO_MQTT_CPP})
endif()

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

## add library suffixes so Windows can find Paho DLLs
set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES} ".dll" ".lib")

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
              mqtt
              paho-mqtt
              mqtt3
              paho-mqtt3
              mqtt3a
        PATHS ${PAHO_MQTT_C_LIB_DIR}
              ${PAHO_MQTT_C_BIN_DIR})
endif()

## use the Paho MQTT C library if found. Otherwise terminate the compilation
if(${PAHO_MQTT_C_LIB} STREQUAL "PAHO_MQTT_C_LIB-NOTFOUND")
    message(FATAL_ERROR "Could not find Paho MQTT C library")
else()
    include_directories(${PAHO_MQTT_C_INC_DIR})
    link_directories(${PAHO_MQTT_C_LIB_DIR})
    if(PAHO_BUILD_SHARED)
        target_link_libraries(${PAHO_MQTT_CPP} ${PAHO_MQTT_C_LIB})
    endif()
    if(PAHO_BUILD_STATIC)
        target_link_libraries(${PAHO_MQTT_CPP_STATIC} ${PAHO_MQTT_C_LIB})
    endif()
endif()
