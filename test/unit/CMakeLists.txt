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

find_package(PkgConfig REQUIRED)

pkg_check_modules(CppUnit REQUIRED cppunit)

include_directories(BEFORE
    "${PROJECT_SOURCE_DIR}/src"
    ${CppUnit_INCLUDE_DIRS}
)

add_executable(testprg test)
set_target_properties(testprg
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_link_libraries(testprg
    mqttpp
    ${CppUnit_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_test(UnitTest testprg)
