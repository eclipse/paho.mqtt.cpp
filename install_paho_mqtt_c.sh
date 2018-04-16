#!/bin/bash
#
# Installs the matching version of Paho MQTT C library required by the C++ lib.
#

git clone https://github.com/eclipse/paho.mqtt.c.git

pushd paho.mqtt.c
git checkout v1.2.1
mkdir build_cmake && cd build_cmake
cmake -DPAHO_WITH_SSL=ON ..
make
sudo make install
sudo ldconfig
popd

exit 0


