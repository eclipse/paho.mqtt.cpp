# Deprecated Build Systems 
# for the Paho MQTT C++ Library

The _autotools_ and _GNU Make_ build systems are now marked as deprecated, and will disappear from the library in a future release. Until then, they should continue to work as they always have, but no effort will be made to add any new features to them.

_CMake_ is now the only supported build system for the library.

## Autotools (on Linux and Unix)

### Build requirements

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

### Build instructions (current release on systems with Autotools)

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
$ sudo make install
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
$ sudo make install
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
$ sudo make install
```

This will use the latest Paho MQTT C headers and libraries from the local workspace (PAHO_MQTT_C_PATH).

#### Build instructions (on systems without Autotools)

The *dist* target generates a tarball to build on systems where Autotools isn't installed. You must generate a distribution package on a system with Autotools installed:

```
$ make dist
```

The command above will create the package *paho-mqtt-cpp-1.0.1.tar.gz*. This package can be compiled on systems without Autotools. Use the following commands:

```
$ tar xzf paho-mqtt-cpp-1.0.1.tar.gz
$ cd paho-mqtt-cpp-1.0.1
$ ./configure --with-paho-mqtt-c=$PAHO_MQTT_C_PATH
$ make
$ sudo make install
```

### Cross compilation

The Autotools cross compilation is performed through *--host* option. For example, to build the library for one of the ARM platforms:

```
$ $PAHO_DIR/configure --host=arm-linux-gnueabi
```


