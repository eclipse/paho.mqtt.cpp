# Makefile for the paho-mqttpp (C++) sample applications

# Check if paho mqtt C libs are installed into the system, and use them.
ifneq (,$(wildcard /usr/local/lib/libpaho-mqtt*))
PAHO_C_LIB = /usr/local/lib
LDLIBS += -L$(PAHO_C_LIB) -lpaho-mqttpp3 -lpaho-mqtt3a
else
PAHO_C_LIB ?= $(abspath ../../../paho.mqtt.c)
LDLIBS += -L../../lib -L$(PAHO_C_LIB)/build/output -lpaho-mqttpp3 -lpaho-mqtt3a
endif

all: async_publish async_subscribe sync_publish

CXXFLAGS += -Wall -std=c++0x
CPPFLAGS += -I.. -I$(PAHO_C_LIB)/src

ifdef DEBUG
  CPPFLAGS += -DDEBUG
  CXXFLAGS += -g -O0
else
  CPPFLAGS += -D_NDEBUG
  CXXFLAGS += -O2
endif


async_publish: async_publish.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)

async_subscribe: async_subscribe.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)

sync_publish: sync_publish.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)

.PHONY: clean distclean

clean:
	rm -f async_publish async_subscribe sync_publish

distclean: clean

