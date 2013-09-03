# Makefile for the mqttpp sample applications

PAHO_C_LIB ?= /home/fmp/static/opensrc/mqtt/paho/org.eclipse.paho.mqtt.c

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

LDLIBS += -L../lib -L$(PAHO_C_LIB)/src/linux_ia64 -lmqttpp -lmqttv3a

async_publish: async_publish.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)

async_subscribe: async_subscribe.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)

sync_publish: sync_publish.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(LDLIBS)

.PHONY: clean
clean:
	rm -f async_publish async_subscribe sync_publish

.PHONY: distclean
distclean: clean


