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

AUTOMAKE_OPTIONS = subdir-objects color-tests
ACLOCAL_AMFLAGS = -I m4

###############################################################################
# Library: name
###############################################################################

lib_LTLIBRARIES  = libpaho-mqttpp3.la

###############################################################################
# Library: source code
###############################################################################

libpaho_mqttpp3_la_SOURCES  = src/async_client.cpp
libpaho_mqttpp3_la_SOURCES += src/client.cpp
libpaho_mqttpp3_la_SOURCES += src/connect_options.cpp
libpaho_mqttpp3_la_SOURCES += src/disconnect_options.cpp
libpaho_mqttpp3_la_SOURCES += src/iclient_persistence.cpp
libpaho_mqttpp3_la_SOURCES += src/message.cpp
libpaho_mqttpp3_la_SOURCES += src/response_options.cpp
libpaho_mqttpp3_la_SOURCES += src/string_collection.cpp
libpaho_mqttpp3_la_SOURCES += src/token.cpp
libpaho_mqttpp3_la_SOURCES += src/topic.cpp
libpaho_mqttpp3_la_SOURCES += src/will_options.cpp
libpaho_mqttpp3_la_SOURCES += src/ssl_options.cpp


###############################################################################
# Library: proprocessor flags
###############################################################################

COMMONCPPFLAGS  = $(AM_CPPFLAGS)
if PAHO_WITH_SSL
COMMONCPPFLAGS += -DOPENSSL
endif

libpaho_mqttpp3_la_CPPFLAGS  = $(COMMONCPPFLAGS)

###############################################################################
# Library: compiler flags
###############################################################################

libpaho_mqttpp3_la_CXXFLAGS  = $(CXXFLAGS)
libpaho_mqttpp3_la_CXXFLAGS += -fPIC
libpaho_mqttpp3_la_CXXFLAGS += -I $(srcdir)/src -I src

###############################################################################
# Library: linker flags
###############################################################################

# Create SONAME entry. Same as -Wl,-soname,libname.so.major
VERSION_INFO = `echo '$(PACKAGE_VERSION)' | $(SED) -e "s/\(.*\)\.\(.*\\)\.\(.*\)/\1:\3:\2/g" `
libpaho_mqttpp3_la_LDFLAGS  = -version-info $(VERSION_INFO)

libpaho_mqttpp3_la_LDFLAGS += $(LDFLAGS)
libpaho_mqttpp3_la_LDFLAGS += $(LIBS)


###############################################################################
# Library: headers to install
###############################################################################

pkgincludedir = $(includedir)/mqtt

pkginclude_HEADERS  = src/mqtt/async_client.h
pkginclude_HEADERS += src/mqtt/buffer_ref.h
pkginclude_HEADERS += src/mqtt/buffer_view.h
pkginclude_HEADERS += src/mqtt/callback.h
pkginclude_HEADERS += src/mqtt/client.h
pkginclude_HEADERS += src/mqtt/connect_options.h
pkginclude_HEADERS += src/mqtt/delivery_token.h
pkginclude_HEADERS += src/mqtt/disconnect_options.h
pkginclude_HEADERS += src/mqtt/exception.h
pkginclude_HEADERS += src/mqtt/iaction_listener.h
pkginclude_HEADERS += src/mqtt/iasync_client.h
pkginclude_HEADERS += src/mqtt/iclient_persistence.h
pkginclude_HEADERS += src/mqtt/message.h
pkginclude_HEADERS += src/mqtt/response_options.h
pkginclude_HEADERS += src/mqtt/thread_queue.h
pkginclude_HEADERS += src/mqtt/token.h
pkginclude_HEADERS += src/mqtt/topic.h
pkginclude_HEADERS += src/mqtt/string_collection.h
pkginclude_HEADERS += src/mqtt/types.h
pkginclude_HEADERS += src/mqtt/will_options.h
pkginclude_HEADERS += src/mqtt/ssl_options.h


###############################################################################
# Samples
###############################################################################

if PAHO_BUILD_SAMPLES

###############################################################################
# Samples: names
###############################################################################

bin_PROGRAMS  = async_publish
bin_PROGRAMS += async_subscribe
bin_PROGRAMS += async_consume
bin_PROGRAMS += sync_publish
bin_PROGRAMS += sync_consume
bin_PROGRAMS += data_publish
bin_PROGRAMS += pub_speed_test

if PAHO_WITH_SSL
bin_PROGRAMS += ssl_publish
endif

###############################################################################
# Samples: source code
###############################################################################

async_publish_SOURCES = src/samples/async_publish.cpp

async_subscribe_SOURCES = src/samples/async_subscribe.cpp

async_consume_SOURCES = src/samples/async_consume.cpp

sync_publish_SOURCES = src/samples/sync_publish.cpp

sync_consume_SOURCES = src/samples/sync_consume.cpp

data_publish_SOURCES = src/samples/data_publish.cpp

pub_speed_test_SOURCES = src/samples/pub_speed_test.cpp

if PAHO_WITH_SSL
ssl_publish_SOURCES = src/samples/ssl_publish.cpp
endif

###############################################################################
# Samples: proprocessor flags
###############################################################################

async_publish_CPPFLAGS = $(COMMONCPPFLAGS)

async_subscribe_CPPFLAGS = $(COMMONCPPFLAGS)

async_consume_CPPFLAGS = $(COMMONCPPFLAGS)

sync_publish_CPPFLAGS = $(COMMONCPPFLAGS)

sync_consume_CPPFLAGS = $(COMMONCPPFLAGS)

data_publish_CPPFLAGS = $(COMMONCPPFLAGS)

pub_speed_test_CPPFLAGS = $(COMMONCPPFLAGS)

if PAHO_WITH_SSL
ssl_publish_CPPFLAGS = $(COMMONCPPFLAGS)
endif

###############################################################################
# Samples: compiler flags
###############################################################################

COMMONCXXFLAGS  = $(CXXFLAGS)
COMMONCXXFLAGS += -I$(top_srcdir)/src/

async_publish_CXXFLAGS = $(COMMONCXXFLAGS)

async_subscribe_CXXFLAGS = $(COMMONCXXFLAGS)

async_consume_CXXFLAGS = $(COMMONCXXFLAGS)

sync_publish_CXXFLAGS = $(COMMONCXXFLAGS)

sync_consume_CXXFLAGS = $(COMMONCXXFLAGS)

data_publish_CXXFLAGS = $(COMMONCXXFLAGS)

pub_speed_test_CXXFLAGS = $(COMMONCXXFLAGS)

if PAHO_WITH_SSL
ssl_publish_CXXFLAGS = $(COMMONCXXFLAGS)
endif

###############################################################################
# Samples: linker flags
###############################################################################

COMMONLDFLAGS  = $(LDFLAGS)
COMMONLDFLAGS += $(LIBS)
COMMONLDFLAGS += libpaho-mqttpp3.la

async_publish_LDADD = $(COMMONLDFLAGS)

async_subscribe_LDADD = $(COMMONLDFLAGS)

async_consume_LDADD = $(COMMONLDFLAGS)

sync_publish_LDADD = $(COMMONLDFLAGS)

sync_consume_LDADD = $(COMMONLDFLAGS)

pub_speed_test_LDADD = $(COMMONLDFLAGS)

data_publish_LDADD = $(COMMONLDFLAGS)

if PAHO_WITH_SSL
ssl_publish_LDADD = $(COMMONLDFLAGS)
endif

endif

###############################################################################
# Test: names
###############################################################################

check_PROGRAMS  = mqttpp-unittest

TESTS = $(check_PROGRAMS)

###############################################################################
# Test: source code
###############################################################################

mqttpp_unittest_SOURCES = test/unit/test.cpp

###############################################################################
# Test: preprocessor flags
###############################################################################

mqttpp_unittest_CPPFLAGS  = $(COMMONCPPFLAGS)

###############################################################################
# Test: compiler flags
###############################################################################

mqttpp_unittest_CXXFLAGS  = $(CXXFLAGS)
mqttpp_unittest_CXXFLAGS += -I $(srcdir)/src

###############################################################################
# Test: linker flags
###############################################################################

mqttpp_unittest_LDFLAGS  = $(LDFLAGS)
mqttpp_unittest_LDFLAGS += $(LIBS)

mqttpp_unittest_LDADD  = libpaho-mqttpp3.la
mqttpp_unittest_LDADD += -lcppunit

###############################################################################
# Documentation
###############################################################################

if PAHO_BUILD_DOC
doxyfile.stamp:
	$(MKDIR_P) @top_builddir@/doc
	cd doc && $(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp
clean-local:
	rm -rf $(top_srcdir)/docs/man
endif

