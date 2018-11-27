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

find_package(PahoMqttC REQUIRED)

add_subdirectory(mqtt)

if(PAHO_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

## --- Library dependencies ---

find_package(Threads)
set(LIBS_SYSTEM Threads::Threads)
if(WIN32)
    string(APPEND LIBS_SYSTEM " ws2_32")
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

# Automatically add the current source- and build directories to the include path
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE TRUE)

add_library(OBJS OBJECT ${SOURCES})
# Object libraries can't use target_link_libraries in order to take advantage
# of transitive usage requirements until CMake 3.12. This is a workaround:
target_include_directories(OBJS PRIVATE ${PAHO_MQTT_C_INCLUDE_DIRS})

## --- Build the shared library, if requested ---

if(PAHO_BUILD_SHARED)
    ## set position independent flag (-fPIC on Unix)
    set_property(TARGET OBJS PROPERTY POSITION_INDEPENDENT_CODE ON)

    add_library(${PAHO_MQTT_CPP} SHARED $<TARGET_OBJECTS:OBJS>)

    ## add dependencies to the shared library
    target_link_libraries(${PAHO_MQTT_CPP}
        PRIVATE ${LIBS_SYSTEM}
        PUBLIC PahoMqttC::PahoMqttC)
    target_include_directories(${PAHO_MQTT_CPP}
        PUBLIC $<INSTALL_INTERFACE:include>)

    ## set the shared library soname
    set_target_properties(${PAHO_MQTT_CPP} PROPERTIES
        VERSION ${CLIENT_VERSION}
        SOVERSION ${PAHO_VERSION_MAJOR})

    ## install the shared library
    install(TARGETS ${PAHO_MQTT_CPP} EXPORT PahoMqttCpp
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

## --- Build static version of the library, if requested ---

if(PAHO_BUILD_STATIC)
    ## create the static library
    add_library(${PAHO_MQTT_CPP_STATIC} STATIC $<TARGET_OBJECTS:OBJS>)

    ## add dependencies to the shared library
    target_link_libraries(${PAHO_MQTT_CPP}
        PRIVATE ${LIBS_SYSTEM}
        PUBLIC PahoMqttC::PahoMqttC)
    target_include_directories(${PAHO_MQTT_CPP}
        PUBLIC $<INSTALL_INTERFACE:include>)

    ## install the static library
    install(TARGETS ${PAHO_MQTT_CPP_STATIC} EXPORT PahoMqttCpp
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

    ## Static lib has same name as shared lib
    set_target_properties(${PAHO_MQTT_CPP_STATIC} PROPERTIES OUTPUT_NAME ${PAHO_MQTT_CPP})
endif()
