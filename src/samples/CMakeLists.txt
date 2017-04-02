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
#     Luis Castedo - initial version
#*******************************************************************************/

include_directories(BEFORE "${PROJECT_SOURCE_DIR}/src")

add_executable(sync_publish sync_publish)
add_executable(async_publish async_publish)
add_executable(async_subscribe async_subscribe)
set_target_properties(async_publish sync_publish async_subscribe
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_link_libraries(sync_publish mqttpp ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(async_publish mqttpp ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(async_subscribe mqttpp ${CMAKE_THREAD_LIBS_INIT})

install(
  TARGETS sync_publish async_publish async_subscribe
  RUNTIME DESTINATION bin COMPONENT samples
)
