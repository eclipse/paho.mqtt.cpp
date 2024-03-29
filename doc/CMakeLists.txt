#*******************************************************************************
#  Copyright (c) 2016
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
#*******************************************************************************/

## documentation settings
find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
endif()

set(DOXYTARGETS)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc)

set(DOXYFILE_SRC Doxyfile.cmake)
set(DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE_SRC})

configure_file(${DOXYFILE_SRC} ${DOXYFILE} @ONLY)
add_custom_target(
    ${DOXYFILE_SRC}.target
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)
set(DOXYTARGETS ${DOXYTARGETS} ${DOXYFILE_SRC}.target)

add_custom_target(doc ALL DEPENDS ${DOXYTARGETS})

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc DESTINATION share)

