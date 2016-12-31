# - Try to find mqtt
# Once done this will define
#  MQTT_FOUND - System has mqtt
#  MQTT_INCLUDE_DIRS - The mqtt include directories
#  MQTT_LIBRARIES - The libraries needed to use mqtt
#  MQTT_DEFINITIONS - Compiler switches required for using mqtt

find_package(PkgConfig)
pkg_check_modules(PC_MQTT QUIET mqtt)
set(MQTT_DEFINITIONS ${PC_MQTT_CFLAGS_OTHER})

find_path(
    MQTT_INCLUDE_DIR MQTTAsync.h
    HINTS ${PC_MQTT_INCLUDEDIR} ${PC_MQTT_INCLUDE_DIRS}
    PATH_SUFFIXES mqtt
)

find_library(
    MQTT_LIBRARY
    NAMES paho-mqtt3as libpaho-mqtt3as
    HINTS ${PC_MQTT_LIBDIR} ${PC_MQTT_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set MQTTPP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(MQTT  DEFAULT_MSG
                                  MQTT_LIBRARY MQTT_INCLUDE_DIR)

mark_as_advanced(MQTT_INCLUDE_DIR MQTT_LIBRARY)

set(MQTT_LIBRARIES ${MQTT_LIBRARY})
set(MQTT_INCLUDE_DIRS ${MQTT_INCLUDE_DIR})
