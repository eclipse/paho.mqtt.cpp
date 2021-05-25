# CMakeLists.txt
#
# CMake file for the Catch2 unit tests in the Eclipse Paho C++ library.
#

#*******************************************************************************
# Copyright (c) 2019 Frank Pagliughi <fpagliughi@mindspring.com>
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
#     Frank Pagliughi - Initial implementation
#*******************************************************************************/

find_package(Catch2 REQUIRED)

# --- For apps that use threads ---

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

# --- Executables ---

add_executable(unit_tests unit_tests.cpp
  test_async_client.cpp
  test_buffer_ref.cpp
  test_client.cpp
  test_connect_options.cpp
  test_create_options.cpp
  test_disconnect_options.cpp
	test_exception.cpp
  test_message.cpp
  test_persistence.cpp
  test_properties.cpp
  test_response_options.cpp
  test_string_collection.cpp
  test_token.cpp
  test_topic.cpp
  test_will_options.cpp
)

if(PAHO_WITH_SSL)
  target_sources(unit_tests PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ssl_options.cpp
  )
endif()

# --- Link for executables ---

message(STATUS "Using library for unit tests: ${PAHO_CPP_LIB}")

# .. Threads::Threads)
target_link_libraries(unit_tests ${PAHO_CPP_LIB} Catch2::Catch2)

include(CTest)
include(Catch)

catch_discover_tests(unit_tests)

