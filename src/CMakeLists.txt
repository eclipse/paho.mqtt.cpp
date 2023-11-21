# CMakeLists.txt
#
# CMake file for the Paho C++ core library.
#
# This is part of the Paho MQTT C++ client library.
#

#*******************************************************************************
# Copyright (c) 2016-2017, Guilherme Maciel Ferreira
# Copyright (c) 2017-2023, Frank Pagliughi
#
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the Eclipse Public License v2.0
#  and Eclipse Distribution License v1.0 which accompany this distribution. 
# 
#  The Eclipse Public License is available at 
#     http://www.eclipse.org/legal/epl-v20.html
#  and the Eclipse Distribution License is available at 
#    http://www.eclipse.org/org/documents/edl-v10.php.
# 
#  Contributors:
#     Guilherme Maciel Ferreira - initial version
#     Frank Pagliughi - made the shared library optional
#*******************************************************************************/

# --- Find the Paho C library ---

if (NOT TARGET PahoMqttC::PahoMqttC)
    find_package(PahoMqttC REQUIRED)
endif()

if(PAHO_WITH_MQTT_C)
    message(STATUS "Paho C: Bundled")
else()
    message(STATUS "Paho C Headers: ${PAHO_MQTT_C_INCLUDE_DIRS}")
    message(STATUS "Paho C Library: ${PAHO_MQTT_C_LIBRARIES}")
endif()

# --- The headers ---

add_subdirectory(mqtt)

include(GenerateExportHeader)
#generate_export_header(mylibrary)

## --- Library dependencies ---

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

## --- Use object library to optimize compilation ---

add_library(paho-cpp-objs OBJECT
  async_client.cpp
  client.cpp
  connect_options.cpp
  create_options.cpp    
  disconnect_options.cpp
  iclient_persistence.cpp
  message.cpp
  properties.cpp
  response_options.cpp
  ssl_options.cpp
  string_collection.cpp
  subscribe_options.cpp
  token.cpp
  topic.cpp
  will_options.cpp
)

## install the shared library
#install(TARGETS paho-cpp-objs EXPORT PahoMqttCpp
#    OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
#)

# Object libraries can't use target_link_libraries in order to take advantage
# of transitive usage requirements until CMake 3.12. This is a workaround:
#target_include_directories(OBJS PRIVATE ${PAHO_MQTT_C_INCLUDE_DIRS})

target_include_directories(paho-cpp-objs 
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
  PRIVATE 
    ${PAHO_MQTT_C_INCLUDE_DIRS}
    src
)

target_compile_definitions(paho-cpp-objs PRIVATE PAHO_MQTTPP_EXPORTS)

# --- Warnings ---

# Maybe set '-Werror' for Release builds?
target_compile_options(paho-cpp-objs PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W3>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wdocumentation>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC,Clang>>:-Wall -Wextra>
)

## --- Build the shared library, if requested ---

if(PAHO_BUILD_SHARED)
    ## create the shared library
    add_library(paho-mqttpp3 SHARED $<TARGET_OBJECTS:paho-cpp-objs>)

    ## add dependencies to the shared library
    target_link_libraries(paho-mqttpp3
        PRIVATE ${LIBS_SYSTEM}
        PUBLIC PahoMqttC::PahoMqttC Threads::Threads)

    # It would be nice to export the include paths from the obj lib, but we
    # get an export error. Perhaps in a future version?
    # 
    # CMake Error: install(EXPORT "PahoMqttCpp" ...) includes target "paho-mqttpp3"
    #   which requires target "paho-cpp-objs" that is not in the export set.

    #target_include_directories(paho-mqttpp3 PUBLIC 
    #    $<TARGET_PROPERTY:paho-cpp-objs,INCLUDE_DIRECTORIES>
    #)

    target_include_directories(paho-mqttpp3 PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    )

    ## set the shared library soname
    set_target_properties(paho-mqttpp3 PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})

    ## install the shared library
    install(TARGETS paho-mqttpp3 EXPORT PahoMqttCpp
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

## --- Build static version of the library, if requested ---

if(PAHO_BUILD_STATIC)
    ## create the static library
    add_library(paho-mqttpp3-static STATIC $<TARGET_OBJECTS:paho-cpp-objs>)

    ## add dependencies to the shared library
    target_link_libraries(paho-mqttpp3-static
        PRIVATE ${LIBS_SYSTEM}
        PUBLIC PahoMqttC::PahoMqttC Threads::Threads)

    target_include_directories(paho-mqttpp3-static PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
    )

    ## install the static library
    install(TARGETS paho-mqttpp3-static EXPORT PahoMqttCpp
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

    ## Let the archive use the same base name as the shared lib on *nix systems
    if(UNIX)
        set_target_properties(paho-mqttpp3-static PROPERTIES OUTPUT_NAME paho-mqttpp3)
    endif()
endif()


