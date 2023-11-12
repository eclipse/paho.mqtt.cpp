# Eclipse Paho MQTT C++ Client Library

This repository contains the source code for the [Eclipse Paho](http://eclipse.org/paho) MQTT C++ client library for memory-managed operating systems such as Linux, MacOS, and Windows.

This code builds a library which enables Modern C++ applications (C++11 and beyond) to connect to an [MQTT](http://mqtt.org) broker, publish messages, subscribe to topics, and receive messages from the broker.

The library has the following features:

- Support for MQTT v3.1, v3.1.1, and v5.
- Network Transports:
    - Standard TCP
    - Secure sockets with SSL/TLS
    - WebSockets
        - Secure and insecure
        - Proxy support
- Message persistence
    - User configurable
    - Built-in File persistence
    - User-defined key/value persistence easy to implement
- Automatic Reconnect
- Offline Buffering
- High Availability
- Blocking and non-blocking API's
- Modern C++ interface (C++11 and beyond)

This code requires the [Paho C library](https://github.com/eclipse/paho.mqtt.c) by Ian Craggs, et al., specifically version 1.3.13 or possibly later.

## Latest News

To keep up with the latest announcements for this project, or to ask questions:

**Twitter:** [@eclipsepaho](https://twitter.com/eclipsepaho) and [@fmpagliughi](https://twitter.com/fmpagliughi)

**Email:** [Eclipse Paho Mailing List](https://accounts.eclipse.org/mailing-list/paho-dev)


### Upcoming Version 1.3

It's been a while since there's been an update. But work has started on v1.3! It will do the following:

- Cover all the new features in and up to Paho C v1.3.13
- Finally release all the new features and bug fixed already in the repository
- Optionally build the Paho C++ and C libraries at the same time.
- Resolve many of the reported Issues

Hopefully it will be out by the end of 2023. Just a few weeks away!

### Unreleased features in this branch (upcoming v1.3.0)

- Updated create and connect options to better deal with MQTT protocol version
- Defaulting connect version to v5 if specified in create options.
- Added a `topic_filter` class to match a single filter to specific topics.
- Added a `topic_matcher` class to create a collection of items in a trie structure that can contain items tied to topic filters. (Useful for queues or callbacks per-subscription topic).
- Minor tweaks to prepare for C++20
- Support for Catch2 v3.x for unit tests (v2.x also still supported).

- [#313](https://github.com/eclipse/paho.mqtt.cpp/issues/313) Get unit tests building on Windows. Needed to get rid of make_unique<> for Windows
- [#397](https://github.com/eclipse/paho.mqtt.cpp/issues/397) Doc about clean session in connect_options.h is wrong
- [#442](https://github.com/eclipse/paho.mqtt.cpp/issues/442) g++ complains with multiple definition of static constexpr for mixed C++11/17 builds
- [#445](https://github.com/eclipse/paho.mqtt.cpp/pull/445)Fix copy/move constructor for connect/disconnect opts with properties
- [#425](https://github.com/eclipse/paho.mqtt.cpp/pull/425) Silence warning for unused variable rsp in class `unsubscribe_response`
- [#440](https://github.com/eclipse/paho.mqtt.cpp/pull/440) Fix typos across the project
- [#428](https://github.com/eclipse/paho.mqtt.cpp/issues/428) Fixed type in create_options.h
- [#407](https://github.com/eclipse/paho.mqtt.cpp/pull/407) Fix nodiscard warnings in sync client
- [#385](https://github.com/eclipse/paho.mqtt.cpp/issues/385) Thread queue deadlock with multiple consumers
- [#374](https://github.com/eclipse/paho.mqtt.cpp/pull/374) Add Paho C as a submodule
- [#350](https://github.com/eclipse/paho.mqtt.cpp/pull/350) avoid adding Paho MQTT C library twice
- [#253](https://github.com/eclipse/paho.mqtt.cpp/issues/253) implicit capture of 'this' via '[=]' is deprecated in C++20
- [#337](https://github.com/eclipse/paho.mqtt.cpp/issues/337) copy/move of caPath_ in ssl_options
- [#330](https://github.com/eclipse/paho.mqtt.cpp/pull/330) added /build/ folder to .gitignore
- [#317](https://github.com/eclipse/paho.mqtt.cpp/issues/317) String constructor using just len instead of end iterator.
- [#323](https://github.com/eclipse/paho.mqtt.cpp/issues/323) Added Session Expiry Interval to v5 chat sample to test.


### New in Version 1.2.0

Version 1.2.0 brought in some missing MQTT v5 features, support for websocket headers and proxies, ALPN protocol lists, added the builder pattern for options, and fixed a number of bugs in both the C++ library and the underlying C lib.

Requires Paho C v1.3.8

- Missing MQTT v5 features:
    - Ability to add properties to Subscribe and Unsubscribe packets (i.e. subscription identifiers)
    - "Disconnected" callback gives reason code and properties for server disconnect
- New `create_options` that can be used to construct a client with new features:
    - Send while disconnected before the 1st successful connection
    - Output buffer can delete oldest messages when full
    - Can choose to clear the persistence store on startup
    - Select whether to persist QoS 0 messages
- Started classes to create options using the Builder Pattern, with the `create_options_builder`, `connect_options_builder`, `message_ptr_builder`, etc.
- User-defined websocket HTTP headers.
- HTTP/S proxy support
- Added ALPN protocol support to SSL/TLS options
- SSL/TLS error and PSK callback support
- Update connection callback support (change credentials when using auto-reconnect)
- Updates to the sample apps:
    - Overall cleanup with better consistency
    - Example of using websockets and a proxy
    - User-based file persistence with simple encoding/encryption
    - Sharing a client between multiple threads
- Converted the unit tests to use Catch2
- All library exceptions are now properly derived from the `mqtt::exception` base class.
- [#231] Added `on_disconnected` callback to handle receipt of disconnect packet from server.
- [#211, #223, #235] Removed use of Log() function from the Paho C library.
- [#227] Fixed race condition in thread-safe queue
- [#224] & [#255] Subscribing to MQTT v3 broker with array of one topic causes segfault.
- [#282] Ability to build Debian/Ubuntu package
- [#300] Calling `reconnect()` was hanging forever, even when successful. In addition several of the synchronous `client` calls were hanging forever on failure. They now properly throw a `timeout_error` exception.
- Several memory issues and bug fixes from updated Paho C library support.

### _Catch2_ Unit Tests

Unit tests were converted to use _Catch2_ for the test framework. 

_Catch2_ can be found here: [Catch2](https://github.com/catchorg/Catch2)

## Contributing

Contributions to this project are gladly welcomed and appreciated Before submitting a Pull Request, please keep three things in mind:

 - This is an official Eclipse project, so it is required that all contributors sign an [Eclipse Contributor Agreement (ECA)](https://www.eclipse.org/legal/ECA.php)
 - Please submit all Pull Requests against the _develop_ branch (not master).
 - Please sign all commits.

 For full details, see [CONTRIBUTING.md](https://github.com/eclipse/paho.mqtt.cpp/blob/master/CONTRIBUTING.md).

## Building from source

_CMake_  is a cross-platform build system suitable for Unix and non-Unix platforms such as Microsoft Windows. It is now the only supported build system.

The Paho C++ library requires the Paho C library, v1.3.8 or greater, to be built and installed first. More information below.

CMake allows for options to direct the build. The following are specific to Paho C++:

Variable | Default Value | Description
------------ | ------------- | -------------
PAHO_BUILD_SHARED | TRUE (Linux), FALSE (Win32) | Whether to build the shared library
PAHO_BUILD_STATIC | FALSE (Linux), TRUE (Win32) | Whether to build the static library
PAHO_WITH_SSL | TRUE (Linux), FALSE (Win32) | Whether to build SSL/TLS support into the library
PAHO_BUILD_DOCUMENTATION | FALSE | Create the HTML API documentation (requires _Doxygen_)
PAHO_BUILD_SAMPLES | FALSE | Whether to build the sample programs
PAHO_BUILD_TESTS | FALSE | Build the unit tests. (Requires _Catch2_)
PAHO_BUILD_DEB_PACKAGE | FALSE | Flag that configures cpack to build a Debian/Ubuntu package
PAHO_WITH_MQTT_C | FALSE | Whether to build the bundled Paho C library

In addition, the C++ build might commonly use `CMAKE_PREFIX_PATH` to help the build system find the location of the Paho C library.

### Unix and Linux

On *nix systems CMake creates Makefiles.

The build process currently supports a number of Unix and Linux flavors. The build process requires the following tools:

  * CMake v3.5 or newer
  * A fully-compatible C++11 compiler (GCC, Clang, etc)

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

Unit tests are being built using _Catch2_.

_Catch2_ can be found here: [Catch2](https://github.com/catchorg/Catch2).  You must download and install _Catch2_ to build and run the unit tests locally.

#### Building the Paho C library

Before building the C++ library, first, build and install the Paho C library, if not already present. Note, this version of the C++ library requires Paho C v1.3.13 or greater.

As of C++ v1.3.0, the recommended version of the Paho C library is included in the repo as a submodule. This can be built together with the C++ library all at the same time, which simplifies the overall build of the packages.

But, if you want to build the Paho C library manually, it can be done as follows:

```
$ git clone https://github.com/eclipse/paho.mqtt.c.git
$ cd paho.mqtt.c
$ git checkout v1.3.13

$ cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON \
    -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON
$ sudo cmake --build build/ --target install
$ sudo ldconfig
```

This builds the C library with SSL/TLS enabled. If that is not desired, omit the `-DPAHO_WITH_SSL=ON`.

It also uses the "high performance" option of the C library to disable more extensive internal memory checks. Remove the _PAHO_HIGH_PERFORMANCE_ option (i.e. turn it off) to debug memory issues, but for most production systems, leave it on for better performance.

The above will install the library to the default location on the host, which for Linux is normally `/usr/local`. To install the library to a non-standard location, use the `CMAKE_INSTALL_PREFIX` to specify a location. For example, to install into a directory under the user's home directory, perhaps for local testing, do this:

```
$ cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON \
    -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON \
    -DCMAKE_INSTALL_PREFIX=$HOME/install
```

#### Building the Paho C++ library

If the Paho C library is not already installed, the recommended version can be built along with the C++ library in a single step using the CMake option `PAHO_WITH_MQTT_C` set on.

```
$ git clone https://github.com/eclipse/paho.mqtt.cpp
$ cd paho.mqtt.cpp

$ cmake -Bbuild -H. -DPAHO_WITH_MQTT_C=ON -DPAHO_BUILD_STATIC=ON \
    -DPAHO_BUILD_DOCUMENTATION=ON -DPAHO_BUILD_SAMPLES=ON
$ sudo cmake --build build/ --target install
$ sudo ldconfig
```

If a recent version of the Paho C library is available on the build host, and it's installed to a default location, it does not need to be built again. Omit the `PAHO_WITH_MQTT_C` option:

```
$ cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON \
    -DPAHO_BUILD_DOCUMENTATION=ON -DPAHO_BUILD_SAMPLES=ON
```

If the Paho C library is installed to a _non-default_ location, or you want to build against a different version, use the `CMAKE_PREFIX_PATH` to specify its install location. Perhaps something like this:

```
$ cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON \
    -DPAHO_BUILD_DOCUMENTATION=ON -DPAHO_BUILD_SAMPLES=ON \
    -DCMAKE_PREFIX_PATH=$HOME/install
```


#### Building a Debian/Ubuntu package

A Debian/Ubuntu install `.deb` file can be created as follows:

```
$ cmake -Bbuild -H. -DPAHO_WITH_SSL=ON -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_DEB_PACKAGE=ON
$ cmake --build build
$ (cd build && cpack)
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

The 64-bit target can be selected using the CMake generator switch, *-G*, at configuration time. The full version must be provided. For Visual Studio 2015 which is v14 do this to first build the Paho C library:

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

## Supported Network Protocols

The library supports connecting to an MQTT server/broker using TCP, SSL/TLS, and websockets (secure and insecure). This is chosen by the URI supplied to the connect() call. It can be specified as:

```
"mqtt://<host>:<port>"   - TCP, unsecure
 "tcp://<host>:<port>"    (same)

"mqtts://<host>:<port>"  - SSL/TLS
 "ssl://<host>:<port>"     (same)

"ws://<host>:<port>"    - Unsecure websockets
"wss://<host>:<port>"   - Secure websockets
```

The "mqtt://" and "tcp://" schemas are identical. They indicate an insecure connection over TCP. The "mqtt://" variation is new for the library, but becoming more common across different MQTT libraries.

Similarly, the "mqtts://" and "ssl://" schemas are identical. They specify a secure connection over SSL/TLS sockets.

Note that to use any of the secure connect options, "mqtts://, "ssl://", or "wss://" you must compile the library with the `PAHO_WITH_SSL=ON` CMake option to include OpenSSL. In addition, you _must_ specify `ssl_options` when you connect to the broker - i.e. you must add an instance of `ssl_options` to the `connect_options` when calling `connect()`.

## Example

Sample applications can be found in the source repository at _src/samples_:
https://github.com/eclipse/paho.mqtt.cpp/tree/master/src/samples

This is a partial example of what a typical example might look like:

```cpp
int main(int argc, char* argv[])
{
    sample_mem_persistence persist;
    mqtt::client cli(ADDRESS, CLIENT_ID, &persist);

    callback cb;
    cli.set_callback(cb);

    auto connOpts = mqtt::connect_options_builder() 
        .keep_alive_interval(20);
        .clean_session()
        .finalize();

    try {
        cli.connect(connOpts);

        // First use a message pointer.

        mqtt::message_ptr pubmsg = mqtt::make_message(PAYLOAD1);
        pubmsg->set_qos(QOS);
        cli.publish(TOPIC, pubmsg);

        // Now try with itemized publish.

        cli.publish(TOPIC, PAYLOAD2, strlen(PAYLOAD2)+1, 0, false);

        // Disconnect

        cli.disconnect();
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
by Dave Locke et al.
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

