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
#     Guilherme Maciel Ferreira - initial contribuition
#*******************************************************************************/

###############################################################################
# Library name
###############################################################################

lib_LTLIBRARIES  = libpaho-mqttpp3.la

###############################################################################
# Source code
###############################################################################

libpaho_mqttpp3_la_SOURCES  = async_client.cpp
libpaho_mqttpp3_la_SOURCES += client.cpp
libpaho_mqttpp3_la_SOURCES += disconnect_options.cpp
libpaho_mqttpp3_la_SOURCES += iclient_persistence.cpp
libpaho_mqttpp3_la_SOURCES += message.cpp
libpaho_mqttpp3_la_SOURCES += response_options.cpp
libpaho_mqttpp3_la_SOURCES += token.cpp
libpaho_mqttpp3_la_SOURCES += topic.cpp
libpaho_mqttpp3_la_SOURCES += connect_options.cpp
libpaho_mqttpp3_la_SOURCES += will_options.cpp
libpaho_mqttpp3_la_SOURCES += ssl_options.cpp

###############################################################################
# Compiler flags
###############################################################################

libpaho_mqttpp3_la_CXXFLAGS  = $(CXXFLAGS)
libpaho_mqttpp3_la_CXXFLAGS += -fPIC

###############################################################################
# Linker flags
###############################################################################

# Create SONAME entry. Same as -Wl,-soname,libname.so.major
VERSION_INFO = `echo '$(PACKAGE_VERSION)' | $(SED) -e "s/\(.*\)\.\(.*\\)\.\(.*\)/\1:\3:\2/g" `
libpaho_mqttpp3_la_LDFLAGS  = -version-info $(VERSION_INFO)

libpaho_mqttpp3_la_LDFLAGS += $(LDFLAGS)
libpaho_mqttpp3_la_LDFLAGS += $(LIBS)
