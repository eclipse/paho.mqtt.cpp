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
# Binaries to build
###############################################################################

bin_PROGRAMS  = async_publish
bin_PROGRAMS += async_subscribe
bin_PROGRAMS += sync_publish

###############################################################################
# Source code
###############################################################################

async_publish_SOURCES = async_publish.cpp

async_subscribe_SOURCES = async_subscribe.cpp

sync_publish_SOURCES = sync_publish.cpp

###############################################################################
# Compiler flags
###############################################################################

COMMONCXXFLAGS  = $(CXXFLAGS)
COMMONCXXFLAGS += -I$(top_srcdir)/src/

async_publish_CXXFLAGS = $(COMMONCXXFLAGS)

async_subscribe_CXXFLAGS = $(COMMONCXXFLAGS)

sync_publish_CXXFLAGS = $(COMMONCXXFLAGS)

###############################################################################
# Linker flags
###############################################################################

COMMONLDFLAGS  = $(LDFLAGS)
COMMONLDFLAGS += $(LIBS)
COMMONLDFLAGS += $(top_srcdir)/src/libpaho-mqttpp3.la

async_publish_LDADD = $(COMMONLDFLAGS)

async_subscribe_LDADD = $(COMMONLDFLAGS)

sync_publish_LDADD = $(COMMONLDFLAGS)
