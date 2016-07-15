#! /bin/bash

###############################################################################
# This script installs Ninja build system and Eclipse Paho MQTT C Client as a
# prerequisites in order to compile and install Eclipse Paho MQTT C++ CLient.
#
# - Ninja is required by Eclipse Paho MQTT C Client for building it.
#
# - Eclipse Paho MQTT C Client is required by the C++ Client for building it.
###############################################################################

REQ_PKGS="build-essential gcc make cmake cmake-gui cmake-curses-gui re2c"
MQTT_C_CLIENT_VERSION="1.1.0"
BASE_URL="http://build.eclipse.org/technology/paho/C/"

function install_ninja() {
    git clone https://github.com/ninja-build/ninja
    cd ninja
    git checkout release
    ./configure.py --bootstrap
    sudo ln -s ${PWD}/ninja /sbin/ninja
}

# Preferred method for installing Eclipse Paho MQTT C Client
function install_mqtt_c_client_from_github() {
    git clone https://github.com/eclipse/paho.mqtt.c.git
    mkdir -p paho.mqtt.c/build/output
    cd paho.mqtt.c/build/output
    cmake -GNinja -DPAHO_WITH_SSL=TRUE \
        -DPAHO_BUILD_DOCUMENTATION=FALSE \
        -DPAHO_BUILD_SAMPLES=TRUE ../../
    ninja
    cd ../../
    sudo make install
}

# Alternative method for installing Eclipse Paho MQTT C Client
function install_mqtt_c_client_from_eclipse_src() {
    mqtt_c_src="eclipse-paho-mqtt-c-src-${MQTT_C_CLIENT_VERISON}"
    tarball="${mqtt_c_src}.tar.gz"
    base_path="~/Downloads"
    path_mqtt_c="${base_path}/${mqtt_c_src}"
    mkdir -p $path_mqtt_c
    wget -O $base_path "${BASE_URL}/${MQTT_C_CLIENT_VERISON}/${tarball}"
    cd $base_path
    tar xzvf $tarball -C $mqtt_c_src
    cd $path_mqtt_c
    mkdir build
    cd build
    cmake -GNinja -DPAHO_WITH_SSL=TRUE \
        -DPAHO_BUILD_DOCUMENTATION=FALSE \
        -DPAHO_BUILD_SAMPLES=TRUE $path_mqtt_c/src/
    ninja
    cd ../
    sudo make install
}

# By default use the preferred method for installing Eclipse Paho MQTT C Client
function install_mqtt_c_client() {
    install_mqtt_c_client_from_github
}

sudo apt-get install -y $REQ_PKGS
install_ninja
install_mqtt_c_client

exit
