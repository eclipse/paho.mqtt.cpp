#!/bin/bash
#
# Installs the matching version of Paho MQTT C library required by the C++ lib.
#

# Install Paho MQTT C without SLL support on system directory
git clone https://github.com/eclipse/paho.mqtt.c.git
pushd paho.mqtt.c
git checkout develop
mkdir build_cmake && cd build_cmake
cmake -DPAHO_WITH_SSL=OFF ..
make
sudo make install
sudo ldconfig
popd

# Install Paho MQTT C with SLL support on a temporary directory
pushd paho.mqtt.c
mkdir build_cmake_no_ssl && cd build_cmake_no_ssl
cmake -DCMAKE_INSTALL_PREFIX=/tmp/paho-c-with-ssl -DPAHO_WITH_SSL=ON ..
make
make install
popd

exit 0


