# Makefile for the paho-mqttpp (C++) sample applications

ifdef DEVELOP
  PAHO_C_LIB_DIR ?= $(abspath ../../../paho.mqtt.c)/build/output
  PAHO_C_INC_DIR ?= $(abspath ../../../paho.mqtt.c)/src
else
  PAHO_C_LIB_DIR ?= /usr/local/lib
  PAHO_C_INC_DIR ?= /usr/local/include
endif


all: async_publish async_subscribe sync_publish ssl_publish

ifneq ($(CROSS_COMPILE),)
  CC  = $(CROSS_COMPILE)gcc
  CXX = $(CROSS_COMPILE)g++
  AR  = $(CROSS_COMPILE)ar
  LD  = $(CROSS_COMPILE)ld
endif

CXXFLAGS += -Wall -std=c++11
CPPFLAGS += -I.. -I$(PAHO_C_INC_DIR) -DOPENSSL

ifdef DEBUG
  CPPFLAGS += -DDEBUG
  CXXFLAGS += -g -O0
else
  CPPFLAGS += -D_NDEBUG
  CXXFLAGS += -O2
endif

LDLIBS += -L../../lib -L$(PAHO_C_LIB_DIR) -lpaho-mqttpp3 -lpaho-mqtt3a
LDLIBS_SSL += -L../../lib -L$(PAHO_C_LIB_DIR) -lpaho-mqttpp3 -lpaho-mqtt3as

async_publish: async_publish.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)

async_subscribe: async_subscribe.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)

sync_publish: sync_publish.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)

ssl_publish: ssl_publish.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS_SSL)

.PHONY: clean distclean

clean:
	rm -f async_publish async_subscribe sync_publish ssl_publish

distclean: clean


