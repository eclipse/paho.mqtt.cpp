# CMakeLists.txt
#
# CMake file for the Paho C++ sample applications.
#
#*******************************************************************************
# This is part of the Paho MQTT C++ client library.
#
# Copyright (c) 2016-2019
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
#   Frank Pagliughi - Updates for new samples
#*******************************************************************************/

## Note: on OS X you should install XCode and the associated command-line tools

## --- Library dependencies ---

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

## include directories
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

## binary files
add_executable(async_publish async_publish.cpp)
add_executable(async_publish_time async_publish_time.cpp)
add_executable(async_subscribe async_subscribe.cpp)
add_executable(async_consume async_consume.cpp)
add_executable(sync_publish sync_publish.cpp)
add_executable(sync_consume sync_consume.cpp)
add_executable(data_publish data_publish.cpp)
add_executable(rpc_math_cli rpc_math_cli.cpp)
add_executable(rpc_math_srvr rpc_math_srvr.cpp)
add_executable(pub_speed_test pub_speed_test.cpp)
add_executable(mqttpp_chat mqttpp_chat.cpp)
add_executable(topic_publish topic_publish.cpp)

## link binaries
target_link_libraries(async_publish ${PAHO_CPP_LIB})
target_link_libraries(async_publish_time ${PAHO_CPP_LIB})
target_link_libraries(async_subscribe ${PAHO_CPP_LIB})
target_link_libraries(async_consume ${PAHO_CPP_LIB})
target_link_libraries(sync_publish ${PAHO_CPP_LIB})
target_link_libraries(sync_consume ${PAHO_CPP_LIB})
target_link_libraries(data_publish ${PAHO_CPP_LIB})
target_link_libraries(rpc_math_cli ${PAHO_CPP_LIB})
target_link_libraries(rpc_math_srvr ${PAHO_CPP_LIB})
target_link_libraries(mqttpp_chat ${PAHO_CPP_LIB})
target_link_libraries(topic_publish ${PAHO_CPP_LIB})
target_link_libraries(pub_speed_test ${PAHO_CPP_LIB})

set(INSTALL_TARGETS
	async_publish
	async_publish_time
	async_subscribe
	async_consume
	sync_publish
	sync_consume
	data_publish
	rpc_math_cli
	rpc_math_srvr
	mqttpp_chat
	topic_publish
	pub_speed_test
)

if(PAHO_WITH_SSL)
	## SSL binary files
	add_executable(ssl_publish ssl_publish.cpp)

	## link SSL binaries
	target_link_libraries(ssl_publish ${PAHO_CPP_LIB})

	set(INSTALL_TARGETS
		${INSTALL_TARGETS}
		ssl_publish
	)

	add_definitions(-DOPENSSL)
endif()

## install binaries
install(TARGETS ${INSTALL_TARGETS} EXPORT PahoMqttCppSamples
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)

