#!/bin/bash
#
# devenv.sh
#
# Sets up a development environment for working on the the Paho C++ Library on Linux,
# using the development tree of the Paho C library. 
# This is _not_ necessary for building applications against the library once it has 
# been installed - it's just for library developers.
#
# Source it into the current shell as:
#     $ source devenv.sh
#

PAHO_C_DIR=$(readlink -e ../paho.mqtt.c)

export DEVELOP=1
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PAHO_C_DIR}/build/output:${PWD}/lib

