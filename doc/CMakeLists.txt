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

### documentation settings
find_package(Doxygen REQUIRED)

configure_file(Doxyfile.in Doxyfile @ONLY)

add_custom_target(doc ALL
    COMMAND ${DOXYGEN_EXECUTABLE}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
)
install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html
        DESTINATION share/doc/mqttpp
        COMPONENT docs
)
install(
    FILES ${PROJECT_SOURCE_DIR}/about.html ${PROJECT_SOURCE_DIR}/notice.html
        DESTINATION share/doc/mqttpp/html
        COMPONENT docs
)
install(
    FILES ${PROJECT_SOURCE_DIR}/CONTRIBUTING.md ${PROJECT_SOURCE_DIR}/README.md
        DESTINATION share/doc/mqttpp
        COMPONENT docs
)
