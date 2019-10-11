# Eclipse Paho MQTT C++ Client Library

[![Build Status](https://travis-ci.org/eclipse/paho.mqtt.cpp.svg?branch=master)](https://travis-ci.org/eclipse/paho.mqtt.cpp)

This repository contains the source code for the [Eclipse Paho](http://eclipse.org/paho) MQTT C++ client library on memory-managed operating systems such as Linux/Posix and Windows.

This code builds a library which enables C++11 applications to connect to an [MQTT](http://mqtt.org) broker, publish messages to the broker, and to subscribe to topics and receive published messages.

The library has the following features:

- Support for MQTT v5, v3.1.1, and v 3.1
- TCP, SSL/TLS, and WebSocket transports
- Message persistence
- Automatic reconnect
- Offline buffering
- High availability
- Blocking and non-blocking API's

This code requires the [Paho C library](https://github.com/eclipse/paho.mqtt.c) by Ian Craggs, et al., specifically version 1.3.1 or possibly later.

## Latest News

To keep up with the latest announcements for this project, or to ask questions:

**Twitter:** [@eclipsepaho](https://twitter.com/eclipsepaho) and [@fmpagliughi](https://twitter.com/fmpagliughi)

**EMail:** [Eclipse Paho Mailing List](https://accounts.eclipse.org/mailing-list/paho-dev)

**Mattermost:** [Eclipse Mattermost Paho Channel](https://mattermost.eclipse.org/eclipse/channels/paho)


### New Features in Paho C++ v1.1

- MQTT v5 support:
    - **Properties**
        - New `property` class acts something like a std::variant to hold a property of any supported type.
        - New `properties` class is a collection type to hold all the properties for a single transmitted packet.
        - Properties can be added to outbound messages and obtained from received messages.
        - Properties can also be obtained from server responses to requests such as from a _connect_ call. These are available in the `token` objects when they complete.
    - The client object tracks the desired MQTT version that the app requested and/or is currently connected at. Internally this is now required by the `response_options` the need to distinguish between pre-v5 and post-v5 callback functions.
    - MQTT v5 reason codes for requests are available via `token` objects when they complete. They are also available in `exception` objects that are thrown by tokens.
    - Support for subscibe options, like no local subscriptions, etc.
    - Sample applications were added showing how to do basic Remote Procedure Calls (RPC's) with MQTT v5 using the *RESPONSE_TOPIC* and *CORRELATION_DATA* properties. These are *rpc_math_cli* and *rpc_math_srvr* in the _src/samples_ directory.
    - A sample "chat" application was added, showing how to use subscribe options, such as "no local".
- More descriptive error messages (PR #154), integrated into the `mqtt::exception` class. MQTT v5 reason codes are also included in the exceptions when an error occurs.
- Applications can (finally) get server responses from the various ACK packets. These are available through the tokens after they complete, as `connect_response`, `subscribe_response`, and `unsubscribe_response`.
- The `topic` objects can be used to subscribe.
- Applications can register individual callback functions instead of using a `callback` interface object. This allows easy use of lambda functions for callbacks.
- The connect options can take a LWT as a plain message, via `connect_options::set_will_message()` 

### _Catch2_ Unit Tests

Unit tests are being converted to use _Catch2_ for the test framework. The legacy unit tests are still using _CppUnit_, compiled into a separate test executable. If everything goes well with _Catch2_, the older unit tests will be ported to _Catch2_ as well.

_Catch2_ can be found here: [Catch2](https://github.com/catchorg/Catch2)

## Contributing

Contributions to this project are gladly welcomed. Before submitting a Pull Request, please keep two things in mind:

 - This is an official Eclipse project, so it is required that all contributors sign an [Eclipse Contributor Agreement (ECA)](https://www.eclipse.org/legal/ECA.php)
 - Please submit all Pull Requests against the _develop_ branch (not master).
 
 For full details, see [CONTRIBUTING.md](https://github.com/eclipse/paho.mqtt.cpp/blob/master/CONTRIBUTING.md).
 
## Building from source

*GNU Make and autotools were deprecated and removed in the v1.1 release.*

_CMake_  is a cross-platform build system suitable for Unix and non-Unix platforms such as Microsoft Windows. It is now the only supported build system.

The Paho C++ library requires the Paho C library, v1.3.1 or greater, to be built and installed first. More information below.

CMake allows for options to direct the build. The following are specific to Paho C++:

Variable | Default Value | Description
------------ | ------------- | -------------
PAHO_BUILD_SHARED | TRUE (Linux), FALSE (Win32) | Whether to build the shared library
PAHO_BUILD_STATIC | FALSE (Linux), TRUE (Win32) | Whether to build the static library
PAHO_BUILD_DOCUMENTATION | FALSE | Create and install the HTML based API documentation (requires Doxygen)
PAHO_BUILD_SAMPLES | FALSE | Build sample programs
PAHO_BUILD_TESTS | FALSE | Build the unit tests. (This currently requires both _CppUnit_ and _Catch2_)
PAHO_WITH_SSL | TRUE (Linux), FALSE (Win32) | Flag that defines whether to build ssl-enabled binaries too

In addition, the C++ build might commonly use `CMAKE_PREFIX_PATH` to help the build system find the location of the Paho C library.

### Unix and Linux

On *nix systems CMake creates Makefiles.

The build process currently supports a number of Unix and Linux flavors. The build process requires the following tools:

  * CMake v3.5 or newer
  * GCC v4.8 or newer or Clang v3.9 or newer
  * GNU Make

On Debian based systems this would mean that the following packages have to be installed:

```
$ sudo apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui
```

If you will be using secure sockets (and you probably should):

```
$ sudo apt-get install libssl-dev 
```

Building the documentation requires doxygen and optionally graphviz to be installed:

```
$ sudo apt-get install doxygen graphviz
```

Unite tests are currently being built using both _CppUnit_ and _Catch2_. The _CppUnit_ tests are being deprecated and replaced with _Catch2_ equivalents. In the meantime, however, both systems are required to build the tests.

```
$ sudo apt-get install libcppunit-dev
```

_Catch2_ can be found here: [Catch2](https://github.com/catchorg/Catch2)


Before building the C++ library, first, build and install the Paho C library:

```
$ git clone https://github.com/eclipse/paho.mqtt.c.git
$ cd paho.mqtt.c
$ git checkout v1.3.1

$ cmake -Bbuild -H. -DPAHO_WITH_SSL=ON -DPAHO_ENABLE_TESTING=OFF
$ sudo cmake --build build/ --target install
$ sudo ldconfig
```

This builds with SSL/TLS enabled. If that is not desired, omit the `-DPAHO_WITH_SSL=ON`.

If you installed the C library on a non-standard path, you might want to pass it as value to the `CMAKE_PREFIX_PATH` option.

Using these variables CMake can be used to generate your Makefiles. The out-of-source build is the default on CMake. Therefore it is recommended to invoke all build commands inside your chosen build directory.

An example build session might look like this:

```
$ git clone https://github.com/eclipse/paho.mqtt.cpp
$ cd paho.mqtt.cpp
$ cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
$ sudo cmake --build build/ --target install
$ sudo ldconfig
```

If you did not install Paho C library to a default system location or you want to build against a different version, use the `CMAKE_PREFIX_PATH` to specify its install location:

```
$ cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE \
    -DCMAKE_PREFIX_PATH=../../paho.mqtt.c/build/install/usr/local
```

To use another compiler, either the CXX environment variable can be specified in the configuration step:

```
$ CXX=clang++ cmake ..
```

or the `CMAKE_CXX_COMPILER` flag can be used:


```
$ cmake -DCMAKE_CXX_COMPILER=clang++
```

#### Updating CMake on Ubuntu 14.04 or 16.04

The versions of CMake on Ubuntu 14.04 or 16.04 LTS are pretty old and have some problems with Paho C++ library. A newer version can be added by downloading the source and building it. If the older cmake can be removed from the system using the package manager, or it can be kept, using the Ububtu alternatives to chose between the versions. 

For example, here's how to install CMake v3.6 on Ubuntu 14.04, while keeping the older CMake available as _cmake-2.8:_

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

You can speed up the CMake build on multi-core systems, by specifying parallel buid jobs for the configure and make steps, above, such as the following for a 4-core system:
```
$ ./configure --parallel=4
$ make -j4
$ sudo make install
```

### Windows

On Windows systems CMake creates Visual Studio project files.

The build process currently supports a number Windows versions. The build process requires the following tools:
  * CMake GUI v3.5 or newer
  * Visual Studio 2015 or newer

First install and open the cmake-gui application. This tutorial is based on cmake-gui 3.5.2.

Second, select the path to the Paho MQTT C library (CMAKE_PREFIX_PATH) if not installed in a standard path. Remember that the Paho MQTT C must be installed on the system. Next, choose if it is supposed to build the documentation (PAHO_BUILD_DOCUMENTATION) and/or the sample applications (PAHO_BUILD_SAMPLES).

Once the configuration is done, click on the Configure button, select the version of the Visual Studio, and then click on Generate button.

At the end of this process you have a Visual Studio solution.

Alternately, the libraries can be completely built at an MSBuild Command Prompt. Download the Paho C and C++ library sources, then open a command window and first compile the Paho C library:

```
> cd paho.mqtt.c
> cmake -Bbuild -H. -DCMAKE_INSTALL_PREFIX=C:\mqtt\paho-c
> cmake --build build/ --target install
```

Then build the C++ library:

```
> cd ..\paho.mqtt.cpp
> cmake -Bbuild -H. -DCMAKE_INSTALL_PREFIX=C:\mqtt\paho-cpp -DPAHO_BUILD_SAMPLES=ON -DPAHO_WITH_SSL=OFF -DCMAKE_PREFIX_PATH=C:\mqtt\paho-c
> cmake --build build/ --target install
```
This builds and installs both libraries to a non-standard location under `C:\mqtt`. Modify this location as desired or use the default location, but either way, the C++ library will most likely need to be told where the C library was built using `CMAKE_PREFIX_PATH`.

It seems quite odd, but even on a 64-bit system using a 64-bit compiler, MSVC seems to default to a 32-bit build target. 

The 64-bit target can be selected using tge CMake generator switch, *-G*, at configuration time. The full version must be provided. For Visual Studio 2015 which is v14 do this to first build the Paho C library:

```
> cmake -G "Visual Studio 14 Win64" -Bbuild -H. -DCMAKE_INSTALL_PREFIX=C:\mqtt\paho-c
...
```

Then use it to build the C++ library:

```
> cmake -G "Visual Studio 14 Win64" -Bbuild -H. -DCMAKE_INSTALL_PREFIX=C:\mqtt\paho-cpp -DPAHO_WITH_SSL=OFF -DCMAKE_PREFIX_PATH=C:\mqtt\paho-c
...
```

*Note that it is very important that you use the same generator (target) to build BOTH libraries, otherwise you will get lots of linker errors when you try to build the C++ library.*


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

