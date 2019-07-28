# Makefile for the paho-mqttpp (C++) unit tests

ifndef VERBOSE
  QUIET := @
endif

ifdef DEVELOP
  PAHO_CPP_DIR ?= $(abspath ../..)
  PAHO_CPP_INC_DIR ?= $(PAHO_CPP_DIR)/src
  PAHO_CPP_LIB_DIR ?= $(PAHO_CPP_DIR)/lib

  PAHO_C_DIR ?= $(abspath ../../../paho.mqtt.c)
  PAHO_C_LIB_DIR ?= $(PAHO_C_DIR)/build/output
  PAHO_C_INC_DIR ?= $(PAHO_C_DIR)/src
else
  PAHO_CPP_DIR ?= /usr/local
  PAHO_CPP_INC_DIR ?= $(PAHO_CPP_DIR)/include
  PAHO_CPP_LIB_DIR ?= $(PAHO_CPP_DIR)/lib

  PAHO_C_DIR ?= /usr/local
  PAHO_C_INC_DIR ?= $(PAHO_C_DIR)/include
  PAHO_C_LIB_DIR ?= $(PAHO_C_DIR)/lib
endif


TGT = mqttpp-unittest

.PHONY: all tests
all: $(TGT)

ifneq ($(CROSS_COMPILE),)
  CC  = $(CROSS_COMPILE)gcc
  CXX = $(CROSS_COMPILE)g++
  AR  = $(CROSS_COMPILE)ar
  LD  = $(CROSS_COMPILE)ld
endif

HDR_TESTS = $(wildcard *.h)
HDRS = $(wildcard $(PAHO_CPP_INC_DIR)/mqtt/*.h)

CXXFLAGS += -Wall -std=c++11
CPPFLAGS += -I$(PAHO_CPP_INC_DIR) -I$(PAHO_C_INC_DIR)

ifdef DEBUG
  CPPFLAGS += -DDEBUG
  CXXFLAGS += -g -O0
else
  CPPFLAGS += -D_NDEBUG
  CXXFLAGS += -O2
endif

SSL ?= 1
ifneq ($(SSL),0)
  CPPFLAGS += -DOPENSSL
  PAHO_C_LIB ?= paho-mqtt3as
else
  PAHO_C_LIB ?= paho-mqtt3a
endif

ifdef COVERAGE
  CXXFLAGS += -fprofile-arcs -ftest-coverage
  LDFLAGS += -fprofile-arcs -pg -lgcov
endif

LDLIBS += -L$(PAHO_CPP_LIB_DIR) -lpaho-mqttpp3
LDLIBS += -L$(PAHO_C_LIB_DIR) -l$(PAHO_C_LIB)
LDLIBS += -lcppunit -ldl

$(TGT): test.cpp $(HDR_TESTS) $(HDRS)
	@echo "Building unit tests."
	$(QUIET) $(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS) $(LDFLAGS)

.PHONY: run
run: $(TGT)
	@echo "Running unit tests..."
	$(QUIET) LD_LIBRARY_PATH=$(PAHO_C_LIB_DIR):$(PAHO_CPP_LIB_DIR) ./$(TGT)

.PHONY: clean distclean dump

clean:
	rm -f $(TGT)

distclean: clean

dump:
	@echo CXX: $(CXX)
	@echo Tests: $(HDR_TESTS)
	@echo Headers: $(HDRS)
	@echo CXXFLAGS: $(CXXFLAGS)
	@echo LDFLAGS: $(LDFLAGS)
	@echo LDLIBS: $(LDLIBS)

