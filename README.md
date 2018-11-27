# Eclipse Paho MQTT C++ Client Library


This repository contains the source code for the [Eclipse Paho](http://eclipse.org/paho) MQTT C++ client library on memory-managed operating systems such as Linux/Posix and Windows.

This code builds a library which enables C++11 applications to connect to an [MQTT](http://mqtt.org) broker to publish messages, and to subscribe to topics and receive published messages.

Both synchronous and asynchronous modes of operation are supported.

This code requires the [Paho C library](https://github.com/eclipse/paho.mqtt.c) by Ian Craggs, et al., specifically version 1.2.1 or possibly later.

## Contributing

Contributions to this project are gladly welcomed. Before submitting a Pull Request, please keep two things in mind:

 - This is an official Eclipse project, so it is required that all contributors sign an [Eclipse Contributor Agreement (ECA)](https://www.eclipse.org/legal/ECA.php)
 - Please submit all Pull Requests against the _develop_ branch (not master).
 
 For full details, see [CONTRIBUTING.md](https://github.com/eclipse/paho.mqtt.cpp/blob/master/CONTRIBUTING.md).
 
## Building from source

_CMake_ is now the only supported build system. For information about _autotools_, see  [DEPRECATED_BUILD.md](https://github.com/eclipse/paho.mqtt.cpp/blob/master/CONTRIBUTING.md).

### CMake

CMake is a cross-platform build system suitable for Unix and non-Unix platforms like Microsoft Windows.

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
PAHO_BUILD_SHARED | TRUE (Linux), FALSE (Win32) | Whether to build the shared library
PAHO_BUILD_STATIC | FALSE (Linux), TRUE (Win32) | Whether to build the static library
PAHO_BUILD_DOCUMENTATION | FALSE | Create and install the HTML based API documentation (requires Doxygen)
PAHO_BUILD_SAMPLES | FALSE | Build sample programs
PAHO_WITH_SSL | TRUE (Linux), FALSE (Win32) | Flag that defines whether to build ssl-enabled binaries too

If you installed the C library on a non-standard path, you might want to pass it as value to the
`CMAKE_PREFIX_PATH` option.

Using these variables CMake can be used to generate your Makefiles. The out-of-source build is the default on CMake. Therefore it is recommended to invoke all build commands inside your chosen build directory.

An example build session targeting the build platform could look like this:

```
$ git clone https://github.com/eclipse/paho.mqtt.cpp
$ cd paho.mqtt.cpp
$ mkdir build
$ cd build
$ cmake -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE -DCMAKE_PREFIX_PATH=../../paho.mqtt.c ..
$ make
$ sudo make install
```

or

```
$ cmake -DCMAKE_INSTALL_PREFIX=/tmp/paho-cpp -DCMAKE_PREFIX_PATH=/tmp/paho-c \
  -DPAHO_BUILD_SAMPLES:BOOL=ON -DPAHO_BUILD_STATIC:BOOL=ON \
  -DPAHO_BUILD_DOCUMENTATION:BOOL=ON
$ make
$ sudo make install
```

Invoking cmake and specifying build options can also be performed using cmake-gui or ccmake (see https://cmake.org/runningcmake/). For example:

```
$ cmake ..
$ make
$ sudo make install
```

To use another compiler:

```
$ cmake -DCMAKE_CXX_COMPILER=clang++
$ make
$ sudo make install
```

#### Windows

On Windows systems CMake creates Visual Studio project files.

##### Build requirements

The build process currently supports a number Windows versions. The build process requires the following tools:
  * CMake GUI (cmake.org)
  * Visual Studio (www.visualstudio.com)

##### Build instructions

First install and open the cmake-gui application. This tutorial is based on cmake-gui 3.5.2.

Second, select the path to the Paho MQTT C library (CMAKE_PREFIX_PATH) if not installed in a standard path. Remember that the Paho MQTT C must be installed on the system. Next, choose if it is supposed to build the documentation (PAHO_BUILD_DOCUMENTATION) and/or the sample applications (PAHO_BUILD_SAMPLES).

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
$ sudo make install
```


## Example

Sample applications can be found in the source repository at _src/samples_:
https://github.com/eclipse/paho.mqtt.cpp/tree/master/src/samples

This is a partial example of what a typical example might look like:

```cpp
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

The original API organization and documentation were adapted from:

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
Copyright (c) 2013-2018, IBM Corp.

 All rights reserved. This program and the accompanying materials
 are made available under the terms of the Eclipse Public License v1.0
 and Eclipse Distribution License v1.0 which accompany this distribution.

 The Eclipse Public License is available at
    http://www.eclipse.org/legal/epl-v10.html
 and the Eclipse Distribution License is available at
   http://www.eclipse.org/org/documents/edl-v10.php.

