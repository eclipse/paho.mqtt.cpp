# Change Log

## Version 1.1 (2019-10-12)

This release was primarily to add MQTT v5 support and server responses.

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
- New unit tests have started using _Catch2_.
- Tested with Paho C v1.3.1

## Version 1.0.1 (2018-12-12)

This is a bug-fix released aimed mainly at issues with the build system and working towards more "modern" usage of CMake. In addition:

- Support for Paho C v1.2.1
- Fixed a number of build issues, particularly on Windows
- Windows shared libraries (DLL's) now supported
- Several minor bug fixes


##  Version 1.0.0 (2017-07-23)

The initial Paho C++ Client library for memory-managed platforms (Linux, Windows, etc).

- Requires Paho C Client Library v1.2.
- MQTT 3.1 & 3.1.1 
- SSL/TLS
- Asynchronous & Synchronous interfaces
- Persistence and off-line buffering
- Automatic reconnect
- High availability.
