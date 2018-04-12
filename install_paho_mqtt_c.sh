#!/bin/bash
#
# Installs the matching version of Paho MQTT C library required by the C++ lib.
#

git clone https://github.com/eclipse/paho.mqtt.c.git
pushd paho.mqtt.c
#git checkout develop
mkdir build_cmake && cd build_cmake
cmake -DPAHO_WITH_SSL=ON ..
make
sudo make install
sudo ldconfig
popd

exit 0


