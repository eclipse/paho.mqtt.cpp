# Makefile for the paho-mqttpp (C++) unit tests

ifdef DEVELOP
  PAHO_C_DIR ?= $(abspath ../../../paho.mqtt.c)
  PAHO_C_LIB_DIR ?= $(PAHO_C_DIR)/build/output
  PAHO_C_INC_DIR ?= $(PAHO_C_DIR)/src
else
  PAHO_C_LIB_DIR ?= /usr/local/lib
  PAHO_C_INC_DIR ?= /usr/local/include
endif

TGT = mqttpp-unittest

.PHONY: all
all: $(TGT)

ifneq ($(CROSS_COMPILE),)
  CC  = $(CROSS_COMPILE)gcc
  CXX = $(CROSS_COMPILE)g++
  AR  = $(CROSS_COMPILE)ar
  LD  = $(CROSS_COMPILE)ld
endif

HDR_TESTS = $(wildcard *.h)

CXXFLAGS += -Wall -std=c++11
CPPFLAGS += -I../../src -I$(PAHO_C_INC_DIR)

ifdef DEBUG
  CPPFLAGS += -DDEBUG
  CXXFLAGS += -g -O0
else
  CPPFLAGS += -D_NDEBUG
  CXXFLAGS += -O2
endif

ifdef SSL
  CPPFLAGS += -DOPENSSL
  PAHO_C_LIB ?= paho-mqtt3as
else
  PAHO_C_LIB ?= paho-mqtt3a
endif

LDLIBS += -L../../lib -L$(PAHO_C_LIB_DIR) -lpaho-mqttpp3 -l$(PAHO_C_LIB) -lcppunit -ldl

$(TGT): test.cpp $(HDR_TESTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)


.PHONY: clean distclean dump

clean:
	rm -f $(TGT)

distclean: clean

dump:
	@echo CXX: $(CXX)
	@echo Tests: $(HDR_TESTS)



