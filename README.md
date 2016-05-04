# Eclipse Paho MQTT C++ client library

This repository contains the source code for the [Eclipse Paho](http://eclipse.org/paho) MQTT C++ client library on memory managed operating systems such as Linux/Posix and Windows.

This code builds a library which enables C++11 applications to connect to an [MQTT](http://mqtt.org) broker to publish messages, and to subscribe to topics and receive published messages.

Both synchronous and asynchronous modes of operation are supported.

This code requires the [Paho C library](https://github.com/eclipse/paho.mqtt.c) by Ian Craggs

## Building from source

### Linux

The library uses C++11 features, and thus requires a conforming compiler such as g++ 4.8 or above.
The C++ library can be built against the current release version of the Paho C library or the latest 
development tree, if it is stable.

To build against the release version of the C library, download, build, and install the library as described
[here](https://github.com/eclipse/paho.mqtt.c)

Then clone the C++ library and build:

```
$ git clone https://git.eclipse.org/r/paho/org.eclipse.paho.mqtt.cpp
$ cd paho.mqtt.cpp
$ make
```

To build against the latest C development branch, fFirst, clone the C library repository, checkout the *develop* branch and build in-place (you don't need to install).

```
$ git clone https://git.eclipse.org/r/paho/org.eclipse.paho.mqtt.c
$ cd paho.mqtt.c
$ git branch develop
$ make
$ export LD_LIBRARY_PATH=$PWD/build/output
$ cd ..
```

Then clone the C++ library at the same directory level as the C library, and build the C++ library with the DEVELOP flag set:

```
$ git clone https://git.eclipse.org/r/paho/org.eclipse.paho.mqtt.cpp
$ cd paho.mqtt.cpp
$ make DEVELOP=1
```

This will use the latest C headers and libraries from the local workspace.

### CMake

The CMake is a cross-platform building system suitable for Unix and non-Unix platforms, like Microsoft Windows.

#### Unix and GNU/Linux

On Unix systems CMake creates Makefiles.

##### Build requirements

The build process currently supports a number of Unix and Linux flavors. The build process requires the following tools:
  * CMake (http://cmake.org)
  * GNU Make (https://www.gnu.org/software/make/), and
  * GCC (https://gcc.gnu.org/).

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
PAHO_BUILD_DOCUMENTATION | FALSE | Create and install the HTML based API documentation (requires Doxygen)
PAHO_BUILD_SAMPLES | FALSE | Build sample programs

Using these variables CMake can be used to generate your Makefiles. The out-of-source build is the default on CMake. Therefore it is recommended to invoke all build commands inside your chosen build directory.

An example build session targeting the build platform could look like this:

```
$ git clone https://github.com/eclipse/paho.mqtt.cpp
$ cd paho.mqtt.cpp
$ mkdir build
$ cd build
$ cmake -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE \
  -DPAHO_MQTT_C_PATH=../paho.mqtt.c ..
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
  * CMake GUI (http://cmake.org), and
  * Visual Studio (https://www.gnu.org/software/make/).

##### Build instructions

First install and open the cmake-gui application. This tutorial is based on cmake-gui 3.5.2.

Second, select the path to the Paho MQTT C library (PAHO_MQTT_C_PATH). Remeber that the Paho MQTT C must be installed on the system. Next, choose if it is supposed to build the documentation (PAHO_BUILD_DOCUMENTATION) and/or the sample applications (PAHO_BUILD_SAMPLES). 

Once the configuration is done, click on the Configure button, select the version of the Visual Studio, and then click on Generate button.

At the end of this process you have a Visual Studio solution.


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
        std::cout << "Connecting..." << std::flush;
        client.connect(connOpts);
        std::cout << "OK" << std::endl;

        // First use a message pointer.

        std::cout << "Sending message..." << std::flush;
        mqtt::message_ptr pubmsg = std::make_shared(PAYLOAD1);
        pubmsg->set_qos(QOS);
        client.publish(TOPIC, pubmsg);
        std::cout << "OK" << std::endl;

        // Now try with itemized publish.

        std::cout << "Sending next message..." << std::flush;
        client.publish(TOPIC, PAYLOAD2, strlen(PAYLOAD2)+1, 0, false);
        std::cout << "OK" << std::endl;

        // Now try with a listener, but no token

        std::cout << "Sending final message..." << std::flush;
        pubmsg = std::make_shared(PAYLOAD3);
        pubmsg->set_qos(QOS);
        client.publish(TOPIC, pubmsg);
        std::cout << "OK" << std::endl;

        // Disconnect
        std::cout << "Disconnecting..." << std::flush;
        client.disconnect();
        std::cout << "OK" << std::endl;
    }
    catch (const mqtt::persistence_exception& exc) {
        std::cerr << "Persistence Error: " << exc.what() << " ["
            << exc.get_reason_code() << "]" << std::endl;
        return 1;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << " ["
            << exc.get_reason_code() << "]" << std::endl;
        return 1;
    }

    return 0;
}
```

-----------

The API organization and documentation were adapted from the Paho Java library
by Dave Locke.
Copyright (c) 2012, IBM Corp

 All rights reserved. This program and the accompanying materials
 are made available under the terms of the Eclipse Public License v1.0
 which accompanies this distribution, and is available at
 http://www.eclipse.org/legal/epl-v10.html

-----------

This code requires the Paho C library by Ian Craggs
Copyright (c) 2013 IBM Corp.

 All rights reserved. This program and the accompanying materials
 are made available under the terms of the Eclipse Public License v1.0
 and Eclipse Distribution License v1.0 which accompany this distribution. 

 The Eclipse Public License is available at 
    http://www.eclipse.org/legal/epl-v10.html
 and the Eclipse Distribution License is available at 
   http://www.eclipse.org/org/documents/edl-v10.php.

