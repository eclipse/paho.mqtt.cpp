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
- Blocking and non-blocking APIs
- Modern C++ interface (C++11 and beyond)

This code requires the [Paho C library](https://github.com/eclipse/paho.mqtt.c) by Ian Craggs, et al., specifically version 1.3.13 or possibly later.

## Latest News

To keep up with the latest announcements for this project, or to ask questions:

**Twitter:** [@eclipsepaho](https://twitter.com/eclipsepaho) and [@fmpagliughi](https://twitter.com/fmpagliughi)

**Email:** [Eclipse Paho Mailing List](https://accounts.eclipse.org/mailing-list/paho-dev)


### What's New in v1.3.1

- [#462](https://github.com/eclipse/paho.mqtt.cpp/pull/462) Fix version string for version v1.3.x

### What's New in v1.3.0

The first new release in a while! It's primarily a bug-fix release, but has a few minor new features. The full list is here:

- Fixed building and using library as DLL on Windows with MSVC
- Updated License to Eclipse Public License v2.0
- Updated create and connect options to better deal with MQTT protocol version
- Defaulting connect version to v5 if specified in create options.
- Added a `topic_filter` class to match a single filter to specific topics.
- Added a `topic_matcher` class to create a collection of items in a trie structure that can contain items tied to topic filters. (Useful for queues or callbacks per-subscription topic).
- Minor tweaks to prepare for C++20
- Support for Catch2 v3.x for unit tests (v2.x also still supported).
- Changed the sample apps to use the newer "mqtt://" schemas.
- Connect option initializers for v5 and WebSockets.
- [#343](https://github.com/eclipse/paho.mqtt.cpp/issues/343) async_client::try_consume_message_until taking single parameter fails to compile
- [#445](https://github.com/eclipse/paho.mqtt.cpp/pull/445) Update properties when moving/copying connect options.
- [#325]() Cache connect options in client to keep memory valid for callbacks like SSL on_error()
- [#361](https://github.com/eclipse/paho.mqtt.cpp/issues/361) Added missing LICENSE file to conform to GitHub conventions.
- [#304](https://github.com/eclipse/paho.mqtt.cpp/issues/304) Missing create_options::DFLT_C_STRUCT symbol when linking with MSVC.
- [#429](https://github.com/eclipse/paho.mqtt.cpp/issues/429) Remove declaration of connect_options::to_string() with missing implementation.
- [#411](https://github.com/eclipse/paho.mqtt.cpp/issues/411) Missing virtual keyword for some client methods
- [#444](https://github.com/eclipse/paho.mqtt.cpp/issues/444) Unit tests to check that connect options builder sets properties.
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
- [#350](https://github.com/eclipse/paho.mqtt.cpp/pull/350) Avoid adding Paho MQTT C library twice
- [#253](https://github.com/eclipse/paho.mqtt.cpp/issues/253) Implicit capture of 'this' via '[=]' is deprecated in C++20
- [#337](https://github.com/eclipse/paho.mqtt.cpp/issues/337) Copy/move of caPath_ in ssl_options
- [#330](https://github.com/eclipse/paho.mqtt.cpp/pull/330) Added /build/ folder to .gitignore
- [#317](https://github.com/eclipse/paho.mqtt.cpp/issues/317) String constructor using just len instead of end iterator.
- [#323](https://github.com/eclipse/paho.mqtt.cpp/issues/323) Added Session Expiry Interval to v5 chat sample to test.

## Coming Next

Two new releases are planned for the near future:

- v1.4 will add some minor new features like callback-per-subscription (see [#202](https://github.com/eclipse/paho.mqtt.cpp/issues/202)) and continue with bug fixes and improvements to the build system.
- v1.5 will upgrade the library to C++17 and start adding features the the newer C++ versions support, like an API that uses `std::variant<>`, `std::option<>`, `constexpr`, and so on.

## Contributing

Contributions to this project are gladly welcomed and appreciated Before submitting a Pull Request, please keep three things in mind:

 - This is an official Eclipse project, so it is required that all contributors sign an [Eclipse Contributor Agreement (ECA)](https://www.eclipse.org/legal/ECA.php)
 - Please submit all Pull Requests against the _develop_ branch (not master).
 - Please sign all commits.

 For full details, see [CONTRIBUTING.md](https://github.com/eclipse/paho.mqtt.cpp/blob/master/CONTRIBUTING.md).

## Building from source

_CMake_  is a cross-platform build system suitable for Unix and non-Unix platforms such as Microsoft Windows. It is now the only supported build system.

The Paho C++ library requires the Paho C library, v1.3.13 or greater, to be built and installed first. More information below.

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

On Windows systems CMake creates Visual Studio project files for use with MSVC. Currently, other compilers like _clang_ or _MinGW_ are not directly supported.

#### Using Paho C++ as a Windows DLL

The project can be built as a static library or shared DLL on Windows. If using it as a DLL in your application, yuo should define the macro `PAHO_MQTTPP_IMPORTS` before including any Paho C++ include files. Preferably, make it a global definition in the application's build file, like in CMake:

    target_compile_definitions(myapp PUBLIC PAHO_MQTTPP_IMPORTS)

It's better not to mix DLLs and static libraries, but if you do link the Paho C++ DLL against the Paho C static library, you may need to manually resolve some system dependencies, like adding the WinSock library as a dependency to your application:

    target_link_libraries(myapp ws2_32)

#### Building the Library on Windows

The build process currently supports a number Windows versions. The build process requires the following tools:
  * CMake GUI v3.5 or newer
  * Visual Studio 2015 or newer

First install and open the cmake-gui application. This tutorial is based on cmake-gui 3.5.2.

Second, select the path to the Paho MQTT C library (CMAKE_PREFIX_PATH) if not installed in a standard path. Remember that the Paho MQTT C must be installed on the system. Next, choose if it is supposed to build the documentation (PAHO_BUILD_DOCUMENTATION) and/or the sample applications (PAHO_BUILD_SAMPLES).

Once the configuration is done, click on the Configure button, select the version of the Visual Studio, and then click on Generate button.

At the end of this process you have a Visual Studio solution.

Alternately, the libraries can be completely built at an MSBuild Command Prompt. Download the Paho C and C++ library sources, then open a command window and first compile the Paho C library:

    > cd paho.mqtt.c
    > cmake -Bbuild -H. -DCMAKE_INSTALL_PREFIX=C:\mqtt\paho-c
    > cmake --build build/ --target install

Then build the C++ library:

    > cd ..\paho.mqtt.cpp
    > cmake -Bbuild -H. -DCMAKE_INSTALL_PREFIX=C:\mqtt\paho-cpp -DPAHO_BUILD_SAMPLES=ON -DPAHO_WITH_SSL=OFF -DCMAKE_PREFIX_PATH=C:\mqtt\paho-c
    > cmake --build build/ --target install

This builds and installs both libraries to a non-standard location under `C:\mqtt`. Modify this location as desired or use the default location, but either way, the C++ library will most likely need to be told where the C library was built using `CMAKE_PREFIX_PATH`.

It seems quite odd, but even on a 64-bit system using a 64-bit compiler, MSVC seems to default to a 32-bit build target.

The 64-bit target can be selected using the CMake generator switch, *-G*, at configuration time. The full version must be provided.

For MSVS 2019 and beyond:

    > cmake -G "Visual Studio 16 2019" -Ax64 -Bbuild -H. -DCMAKE_INSTALL_PREFIX=C:\mqtt\paho-c
    > ...

For Visual Studio 2015 which is v14 do this to first build the Paho C library:

    > cmake -G "Visual Studio 14 Win64" -Bbuild -H. -DCMAKE_INSTALL_PREFIX=C:\mqtt\paho-c
    ...

Then use it to build the C++ library:

    > cmake -G "Visual Studio 14 Win64" -Bbuild -H. -DCMAKE_INSTALL_PREFIX=C:\mqtt\paho-cpp -DPAHO_WITH_SSL=OFF -DCMAKE_PREFIX_PATH=C:\mqtt\paho-c
    ...

*Note that it is very important that you use the same generator (target) to build BOTH libraries, otherwise you will get lots of linker errors when you try to build the C++ library.*

## Supported Network Protocols

The library supports connecting to an MQTT server/broker using TCP, SSL/TLS, and websockets (secure and insecure). This is chosen by the URI supplied to the connect() call. It can be specified as:

    "mqtt://<host>:<port>"   - TCP, unsecure
     "tcp://<host>:<port>"    (same)

    "mqtts://<host>:<port>"  - SSL/TLS
     "ssl://<host>:<port>"     (same)

    "ws://<host>:<port>"    - Unsecure websockets
    "wss://<host>:<port>"   - Secure websockets

The "mqtt://" and "tcp://" schemas are identical. They indicate an insecure connection over TCP. The "mqtt://" variation is new for the library, but becoming more common across different MQTT libraries.

Similarly, the "mqtts://" and "ssl://" schemas are identical. They specify a secure connection over SSL/TLS sockets.

Note that to use any of the secure connect options, "mqtts://, "ssl://", or "wss://" you must compile the library with the `PAHO_WITH_SSL=ON` CMake option to include OpenSSL. In addition, you _must_ specify `ssl_options` when you connect to the broker - i.e. you must add an instance of `ssl_options` to the `connect_options` when calling `connect()`.

## _Catch2_ Unit Tests

Unit tests use _Catch2_ for the test framework. Versions 2.x and 3.x are supported.

_Catch2_ can be found here: [Catch2](https://github.com/catchorg/Catch2)


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

This code requires:

The Paho C library by Ian Craggs, et al.
https://github.com/eclipse/paho.mqtt.c