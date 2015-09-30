# Make include file for the mqttpp library
#
# This will compile all the .cpp files in the directory into the library.
# Any files to be excluded should be placed in the variable SRC_IGNORE, like:
# SRC_IGNORE = this.c that.cpp
#

MODULE = mqttpp

# ----- Tools -----

ifndef VERBOSE
  QUIET := @
endif

# ----- Directories -----

LIB_DIR ?= lib
OBJ_DIR ?= obj
SRC_DIR ?= src
INC_DIR ?= $(SRC_DIR)

PAHO_C_HEADERS ?= /usr/local/include

INC_DIRS += $(INC_DIR) $(PAHO_C_HEADERS)

_MK_OBJ_DIR := $(shell mkdir -p $(OBJ_DIR))
_MK_LIB_DIR := $(shell mkdir -p $(LIB_DIR))

# ----- Definitions for the shared library -----

LIB_BASE  ?= $(MODULE)
LIB_MAJOR ?= 0
LIB_MINOR ?= 1

LIB_LINK = lib$(LIB_BASE).so
LIB_MAJOR_LINK = $(LIB_LINK).$(LIB_MAJOR)

LIB = $(LIB_MAJOR_LINK).$(LIB_MINOR)

TGT = $(LIB_DIR)/$(LIB)

# ----- Sources -----

SRCS += $(wildcard $(SRC_DIR)/*.cpp)

ifdef SRC_IGNORE
  SRCS := $(filter-out $(SRC_IGNORE),$(SRCS))
endif

OBJS := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS:.cpp=.o))
DEPS := $(OBJS:.o=.dep)

# ----- Compiler flags, etc -----

CXXFLAGS += -std=c++0x
CPPFLAGS += -Wall -fPIC

ifdef DEBUG
  DEFS += DEBUG
  CPPFLAGS += -g -O0
else
  DEFS += _NDEBUG
  CPPFLAGS += -O2 -Wno-unused-result -Werror
endif

CPPFLAGS += $(addprefix -D,$(DEFS)) $(addprefix -I,$(INC_DIRS))

LIB_DEPS += c stdc++ pthread 

LIB_DEP_FLAGS += $(addprefix -l,$(LIB_DEPS))

LDFLAGS := -g -shared -Wl,-soname,$(LIB_MAJOR_LINK) -L$(LIB_DIR)

# ----- Compiler directives -----

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo $(CXX) $<
	$(QUIET) $(COMPILE.cpp) $(OUTPUT_OPTION) $<

# ----- Build targets -----

.PHONY: all
all: $(TGT) $(LIB_DIR)/$(LIB_LINK) $(LIB_DIR)/$(LIB_MAJOR_LINK)

# This might work for a static library
#$(TGT): $(OBJS)
#	@echo Creating library: $@
#	$(QUIET) $(AR) $(ARFLAGS) $@ $^

$(TGT): $(OBJS)
	@echo Creating library: $@
	$(QUIET) $(CC) $(LDFLAGS) -o $@ $^ $(LIB_DEP_FLAGS)

$(LIB_DIR)/$(LIB_LINK): $(LIB_DIR)/$(LIB_MAJOR_LINK)
	$(QUIET) cd $(LIB_DIR) ; $(RM) $(LIB_LINK) ; ln -s $(LIB_MAJOR_LINK) $(LIB_LINK)

$(LIB_DIR)/$(LIB_MAJOR_LINK): $(TGT)
	$(QUIET) cd $(LIB_DIR) ; $(RM) $(LIB_MAJOR_LINK) ; ln -s $(LIB) $(LIB_MAJOR_LINK)

.PHONY: dump
dump:
	@echo LIB=$(LIB)
	@echo TGT=$(TGT)
	@echo LIB_DIR=$(LIB_DIR)
	@echo OBJ_DIR=$(OBJ_DIR)
	@echo CFLAGS=$(CFLAGS)
	@echo CPPFLAGS=$(CPPFLAGS)
	@echo CXX=$(CXX)
	@echo COMPILE.cpp=$(COMPILE.cpp)
	@echo SRCS=$(SRCS)
	@echo OBJS=$(OBJS)
	@echo DEPS:$(DEPS)
	@echo LIB_DEPS=$(LIB_DEPS)

.PHONY: clean
clean:
	$(QUIET) $(RM) $(TGT) $(LIB_DIR)/$(LIB_LINK) $(LIB_DIR)/$(LIB_MAJOR_LINK) \
	    $(OBJS)

.PHONY: distclean
distclean: clean
	$(QUIET) rm -rf $(OBJ_DIR) $(LIB_DIR)

# ----- Header dependencies -----

MKG := $(findstring $(MAKECMDGOALS),"clean distclean dump")
ifeq "$(MKG)" ""
  -include $(DEPS)
endif

$(OBJ_DIR)/%.dep: %.cpp
	@echo DEP $<
	$(QUIET) $(CXX) -M $(CPPFLAGS) $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$$(OBJ_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

