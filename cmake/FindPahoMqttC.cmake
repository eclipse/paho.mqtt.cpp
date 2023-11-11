# find the Paho MQTT C library
if(PAHO_WITH_SSL)
    set(_PAHO_MQTT_C_LIB_NAME paho-mqtt3as)
    find_package(OpenSSL REQUIRED)
else()
    set(_PAHO_MQTT_C_LIB_NAME paho-mqtt3a)
endif()

# add suffix when using static Paho MQTT C library variant on Windows
if(WIN32)
    if(PAHO_BUILD_STATIC)
        set(_PAHO_MQTT_C_LIB_NAME ${_PAHO_MQTT_C_LIB_NAME}-static)
    endif()
endif()

if(PAHO_WITH_MQTT_C)
    # Build the paho.mqtt.c library from the submodule
    add_subdirectory(externals/paho-mqtt-c)
    add_library(PahoMqttC::PahoMqttC ALIAS ${_PAHO_MQTT_C_LIB_NAME})

    ## install paho.mqtt.c library (appending to PahoMqttCpp export)
    install(TARGETS ${_PAHO_MQTT_C_LIB_NAME}
        EXPORT PahoMqttCpp
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    find_path(PAHO_MQTT_C_INCLUDE_DIRS
        NAMES MQTTAsync.h
        HINTS ${CMAKE_CURRENT_SOURCE_DIR}/externals/paho-mqtt-c/src
    )
else()
    find_library(PAHO_MQTT_C_LIBRARIES NAMES ${_PAHO_MQTT_C_LIB_NAME})
    unset(_PAHO_MQTT_C_LIB_NAME)
    find_path(PAHO_MQTT_C_INCLUDE_DIRS NAMES MQTTAsync.h)

    add_library(PahoMqttC::PahoMqttC UNKNOWN IMPORTED)

    set_target_properties(PahoMqttC::PahoMqttC PROPERTIES
        IMPORTED_LOCATION "${PAHO_MQTT_C_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${PAHO_MQTT_C_INCLUDE_DIRS}"
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    )

    if(PAHO_WITH_SSL)
        set_target_properties(PahoMqttC::PahoMqttC PROPERTIES
            INTERFACE_COMPILE_DEFINITIONS "OPENSSL=1"
            INTERFACE_LINK_LIBRARIES "OpenSSL::SSL;OpenSSL::Crypto")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PahoMqttC REQUIRED_VARS PAHO_MQTT_C_INCLUDE_DIRS)
