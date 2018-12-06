#!/bin/bash
#
# Installs the matching version of Paho MQTT C library required by the C++ lib.
#

set -e

git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
git checkout v1.2.1
cmake -Bbuild -H. -DPAHO_WITH_SSL=ON
sudo cmake --build build/ --target install
sudo ldconfig
exit 0


