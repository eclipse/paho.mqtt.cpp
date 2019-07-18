# CMakeLists.txt
#
# CMake file for the Paho C++ library's unit tests.
#
# This is part of the Paho MQTT C++ client library.
#

#*******************************************************************************
# Copyright (c) 2018, Frank Pagliughi
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
#*******************************************************************************/

# --- Executable ---

add_executable(paho-mqttpp-test test.cpp)

target_link_libraries(paho-mqttpp-test ${PAHO_CPP_LIB} cppunit)

## install binaries
install(TARGETS paho-mqttpp-test EXPORT PahoMqttCppTests
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
