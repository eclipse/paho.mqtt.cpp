# Eclipse Paho MQTT C++ client library


This repository contains the source code for the [Eclipse Paho](http://eclipse.org/paho) MQTT C++ client library on memory-managed operating systems such as Linux/Posix and Windows.

This code builds a library which enables C++11 applications to connect to an [MQTT](http://mqtt.org) broker to publish messages, and to subscribe to topics and receive published messages.

Both synchronous and asynchronous modes of operation are supported.

This code requires the [Paho C library](https://github.com/eclipse/paho.mqtt.c) by Ian Craggs, et al., specifically version 1.2.0 (Paho Release 1.3) or possibly later.

## Building from source

Paho MQTT C++ supports the following building systems: 
 * CMake
 * Autotools
 * GNU Make 

### CMake

CMake is a cross-platform building system suitable for Unix and non-Unix platforms, like Microsoft Windows.

#### Unix and GNU/Linux

On Unix systems CMake creates Makefiles.

##### Build requirements

The build process currently supports a number of Unix and Linux flavors. The build process requires the following tools:
  * CMake (cmake.org)
  * GNU Make (www.gnu.org/software/make)
  * GCC (gcc.gnu.org)

On Debian based systems this would mean that the following packages have to be installed:

```
apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui
```

The documentation requires doxygen and optionally graphviz:

```
apt-get install doxygen graphviz
```

##### Build instructions

Before compiling, determine the value of some variables in order to configure features, library locations, and other options:

Variable | Default Value | Description
------------ | ------------- | -------------
PAHO_MQTT_C_PATH | "" | Add a path paho.mqtt.c library and headers
PAHO_BUILD_SHARED | TRUE (Linux), FALSE (Win32) | Whether to build the shared library
PAHO_BUILD_STATIC | FALSE (Linux), TRUE (Win32) | Whether to build the static library
PAHO_BUILD_DOCUMENTATION | FALSE | Create and install the HTML based API documentation (requires Doxygen)
PAHO_BUILD_SAMPLES | FALSE | Build sample programs
PAHO_WITH_SSL | TRUE (Linux), FALSE (Win32) | Flag that defines whether to build ssl-enabled binaries too

Using these variables CMake can be used to generate your Makefiles. The out-of-source build is the default on CMake. Therefore it is recommended to invoke all build commands inside your chosen build directory.

An example build session targeting the build platform could look like this:

```
$ git clone https://github.com/eclipse/paho.mqtt.cpp
$ cd paho.mqtt.cpp
$ mkdir build
$ cd build
$ cmake -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE -DPAHO_MQTT_C_PATH=../../paho.mqtt.c ..
$ make
```

or

```
$ cmake -DCMAKE_INSTALL_PREFIX=/tmp/paho-cpp -DPAHO_MQTT_C_PATH=/tmp/paho-c \
  -DPAHO_BUILD_SAMPLES:BOOL=ON -DPAHO_BUILD_STATIC:BOOL=ON \
  -DPAHO_BUILD_DOCUMENTATION:BOOL=ON
$ make
```

Invoking cmake and specifying build options can also be performed using cmake-gui or ccmake (see https://cmake.org/runningcmake/). For example:

```
$ ccmake ..
$ make
```

To use another compiler:

```
$ cmake -DCMAKE_CXX_COMPILER=clang++
$ make
```

#### Windows

On Windows systems CMake creates Visual Studio project files.

##### Build requirements

The build process currently supports a number Windows versions. The build process requires the following tools:
  * CMake GUI (cmake.org)
  * Visual Studio (www.visualstudio.com)

##### Build instructions

First install and open the cmake-gui application. This tutorial is based on cmake-gui 3.5.2.

Second, select the path to the Paho MQTT C library (PAHO_MQTT_C_PATH). Remember that the Paho MQTT C must be installed on the system. Next, choose if it is supposed to build the documentation (PAHO_BUILD_DOCUMENTATION) and/or the sample applications (PAHO_BUILD_SAMPLES).

Once the configuration is done, click on the Configure button, select the version of the Visual Studio, and then click on Generate button.

At the end of this process you have a Visual Studio solution.

#### Updating CMake on Ubuntu 14.04 / Mint 17 (LTS)

The version of cmake on Ubuntu 14.04 LTS is pretty old and has some problems with Paho C++ library. A newer version can be added by downloading the source and building it. If the older cmake can be removed from the system using the package manager, or it can be kept, using the Ububtu alternatives to chose between the versions. For example:

```
$ wget http://www.cmake.org/files/v3.6/cmake-3.6.3.tar.gz 
$ tar -xvzf cmake-3.6.3.tar.gz 
$ cd cmake-3.6.3/
$ ./configure
$ make
$ sudo make install
$ sudo mv /usr/bin/cmake /usr/bin/cmake-2.8
$ sudo update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 100
$ sudo update-alternatives --install /usr/bin/cmake cmake /usr/bin/cmake-2.8 200
$ cmake --version
cmake version 3.6.3
```

You can speed up the build on multi-core systems, by specifying parallel buid jobs for the configure and make steps, above, such as the following for a 4-core system:
```
$ ./configure --parallel=4
$ make -j4
```


### Autotools (on Linux and Unix)

The GNU Build System is the preferred building system on POSIX-compliant systems.

#### Build requirements

The build process supports any POSIX-compliant system. The following tools must be installed:
  * Autoconf (www.gnu.org/software/autoconf)
  * Automake (www.gnu.org/software/automake)
  * Libtool (www.gnu.org/software/libtool)
  * GNU Make (www.gnu.org/software/make)
  * GCC (gcc.gnu.org) or Clang/LLVM (clang.llvm.org).

You can instruct Autotools to use another compiler besides the one default by the system:

```
$ $PAHO_DIR/configure CC=clang CXX=clang++
```

The library uses C++11 features, and thus requires a conforming compiler such as g++ 4.8 or above.
The Paho MQTT C++ library can be built against the current release version of the Paho MQTT C library or the latest development tree.

#### Build instructions (current release on systems with Autotools)

In order to build against the release version of the Paho MQTT C library, you must download, build, and install the library as described [here](https://github.com/eclipse/paho.mqtt.c)

After the Paho MQTT C library is installed on your system, clone the Paho MQTT C++ library:

```
$ git clone https://github.com/eclipse/paho.mqtt.cpp
$ cd paho.mqtt.cpp
```

Then run the bootstrap script to create Autotools' scripts:

```
$ ./bootstrap
```

To avoid problems with the existing Makefile, build in a seperate directory
(known as VPATH build or out-of-tree build):

```
$ mkdir mybuild && cd mybuild
$ export PAHO_DIR=$PWD
```

Next, configure the features you want to be available in the library and build:

```
$ ./configure [Options]
$ make
```

Option | Default Value | Description
------------ | ------------- | -------------
 --[en/dis]able-shared | yes | Build as shared library
 --[en/dis]able-static | yes | Build as static library
 --[en/dis]able-samples | no | Build sample programs
 --[en/dis]able-doc | no | Build documentation
 --[en/dis]able-peak-warnings | no | Compile with peak warnings level
 --with-paho-mqtt-c |  | Path to a non-standard Paho MQTT C library
 --with[out]-ssl | with | Build with OpenSSL support

For example, in order to build only the static library:
(under the assumption that "$PAHO_DIR" points to the directory which contains the paho.mqtt.c source tree)

```
$ $PAHO_DIR/configure --disable-shared --enable-static
$ make
```

#### Build instructions (development tree on systems with Autotools)

In order to build against the latest Paho MQTT C development branch, first clone the C library repository. Then checkout the *develop* branch and build in-place (you don't need to install).

```
$ git clone https://github.com/eclipse/paho.mqtt.c
$ cd paho.mqtt.c
$ git checkout -t origin/develop
$ make
$ export PAHO_MQTT_C_PATH=$PWD
$ export LD_LIBRARY_PATH=$PWD/build/output
$ cd ..
```

Then clone the Paho MQTT C++ library and build it, passing the path to the Paho MQTT C library:

```
$ git clone https://github.com/eclipse/paho.mqtt.cpp
$ cd paho.mqtt.cpp
$ ./bootstrap
$ $PAHO_DIR/configure --with-paho-mqtt-c=$PAHO_MQTT_C_PATH
$ make
```

This will use the latest Paho MQTT C headers and libraries from the local workspace (PAHO_MQTT_C_PATH).

#### Build instructions (on systems without Autotools)

The *dist* target generates a tarball to build on systems where Autotools isn't installed. You must generate a distribution package on a system with Autotools installed:

```
$ make dist
```

The command above will create the package *paho-mqtt-cpp-0.0.2.tar.gz*. This package can be compiled on systems without Autotools. Use the following commands:

```
$ tar xzf paho-mqtt-cpp-0.0.2.tar.gz
$ cd paho-mqtt-cpp-0.0.2
$ ./configure --with-paho-mqtt-c=$PAHO_MQTT_C_PATH
$ make
```

#### Cross compilation

The Autotools cross compilation is performed through *--host* option. For example, to build the library for one of the ARM platforms:

```
$ $PAHO_DIR/configure --host=arm-linux-gnueabi
```


## Example

Sample applications can be found in src/samples.

```
int main(int argc, char* argv[])
{
    sample_mem_persistence persist;
    mqtt::client client(ADDRESS, CLIENTID, &persist);

    callback cb;
    client.set_callback(cb);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try {
        client.connect(connOpts);

        // First use a message pointer.

        mqtt::message_ptr pubmsg = mqtt::make_message(PAYLOAD1);
        pubmsg->set_qos(QOS);
        client.publish(TOPIC, pubmsg);

        // Now try with itemized publish.

        client.publish(TOPIC, PAYLOAD2, strlen(PAYLOAD2)+1, 0, false);

        // Disconnect
        client.disconnect();
    }
    catch (const mqtt::persistence_exception& exc) {
        cerr << "Persistence Error: " << exc.what() << " ["
            << exc.get_reason_code() << "]" << endl;
        return 1;
    }
    catch (const mqtt::exception& exc) {
        cerr << "Error: " << exc.what() << " ["
            << exc.get_reason_code() << "]" << endl;
        return 1;
    }

    return 0;
}
```

-----------

The API organization and documentation were adapted from:

The Paho Java library
by Dave Locke.
Copyright (c) 2012, IBM Corp

 All rights reserved. This program and the accompanying materials
 are made available under the terms of the Eclipse Public License v1.0
 which accompanies this distribution, and is available at
 http://www.eclipse.org/legal/epl-v10.html

-----------

This code requires:

The Paho C library by Ian Craggs
Copyright (c) 2013 IBM Corp.

 All rights reserved. This program and the accompanying materials
 are made available under the terms of the Eclipse Public License v1.0
 and Eclipse Distribution License v1.0 which accompany this distribution.

 The Eclipse Public License is available at
    http://www.eclipse.org/legal/epl-v10.html
 and the Eclipse Distribution License is available at
   http://www.eclipse.org/org/documents/edl-v10.php.

