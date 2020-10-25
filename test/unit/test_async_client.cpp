// test_async_client.cpp
//
// Unit tests for the async_client class in the Paho MQTT C++ library.
//

/*******************************************************************************
 * Copyright (c) 2017 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
 * Copyright (c) 2020 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Guilherme M. Ferreira - initial implementation and documentation
 *    Frank Pagliughi - Converted to Catch2
 *******************************************************************************/
#define UNIT_TESTS

#include "catch2/catch.hpp"
#include "mqtt/iasync_client.h"
#include "mqtt/async_client.h"
#include "mock_persistence.h"
#include "mock_callback.h"
#include "mock_action_listener.h"

using namespace std::chrono;
using namespace mqtt;

/////////////////////////////////////////////////////////////////////////////

// NOTE: This test case requires network access. It uses one of
//  	 the public available MQTT brokers
#if defined(TEST_EXTERNAL_SERVER)
	static const std::string GOOD_SERVER_URI { "tcp://mqtt.eclipse.org:1883" };
#else
	static const std::string GOOD_SERVER_URI { "tcp://localhost:1883" };
	static const std::string GOOD_SSL_SERVER_URI { "ssl://localhost:18885" };
#endif

static const std::string BAD_SERVER_URI  { "one://invalid.address" };
static const std::string CLIENT_ID { "" }; // { "async_client_unit_test" };
static const std::string PERSISTENCE_DIR { "persist" };
static const std::string TOPIC { "TOPIC" };

static const int GOOD_QOS { 0 };
static const int BAD_QOS  { 3 };
static const_string_collection_ptr TOPIC_COLL { string_collection::create({ "TOPIC0", "TOPIC1", "TOPIC2" })};

static mqtt::iasync_client::qos_collection GOOD_QOS_COLL { 0, 1, 2 };
static mqtt::iasync_client::qos_collection BAD_QOS_COLL  { BAD_QOS, 1, 2 };

static const std::string PAYLOAD { "PAYLOAD" };
static const int TIMEOUT { 1000 };
static int CONTEXT { 4 };
static mock_action_listener listener;
static const bool RETAINED { false };

//----------------------------------------------------------------------
// Test constructors async_client::async_client()
//----------------------------------------------------------------------

TEST_CASE("async_client user constructor 2 string_args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};

	REQUIRE(GOOD_SERVER_URI == cli.get_server_uri());
	REQUIRE(CLIENT_ID == cli.get_client_id());
}

TEST_CASE("async_client user constructor 2 string args failure", "[client]")
{
	int return_code = MQTTASYNC_SUCCESS;
	try {
		mqtt::async_client cli{BAD_SERVER_URI, CLIENT_ID};
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_BAD_PROTOCOL == return_code);
}

TEST_CASE("async_client user constructor 3 string args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID, PERSISTENCE_DIR};

	REQUIRE(GOOD_SERVER_URI == cli.get_server_uri());
	REQUIRE(CLIENT_ID == cli.get_client_id());
}

#if 0
TEST_CASE("async_client user constructor 3 args", "[client]")
{
	mock_persistence cp;
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID, &cp};

	REQUIRE(GOOD_SERVER_URI == cli.get_server_uri());
	REQUIRE(CLIENT_ID == cli.get_client_id());

	mqtt::async_client cli_no_persistence{GOOD_SERVER_URI, CLIENT_ID, nullptr};

	REQUIRE(GOOD_SERVER_URI == cli_no_persistence.get_server_uri());
	REQUIRE(CLIENT_ID == cli_no_persistence.get_client_id());
}
#endif

//----------------------------------------------------------------------
// Test async_client::connect()
//----------------------------------------------------------------------

TEST_CASE("async_client connect 0 arg", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	try {
		mqtt::token_ptr token_conn = cli.connect();
		REQUIRE(token_conn);
		token_conn->wait();
		REQUIRE(cli.is_connected());
	}
	catch (const std::exception& exc) {
		FAIL(std::string("Connection failure: ") + exc.what());
	}
}

TEST_CASE("async_client connect 1 arg", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::connect_options co;
	mqtt::token_ptr token_conn{cli.connect(co)};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());
}

TEST_CASE("async_client connect 1 arg failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn; //{ nullptr };
	mqtt::connect_options co;
	mqtt::will_options wo;
	wo.set_qos(BAD_QOS); // Invalid QoS causes connection failure
	co.set_will(wo);
	int return_code = MQTTASYNC_SUCCESS;
	try {
		token_conn = cli.connect(co);
		REQUIRE(token_conn);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(nullptr == token_conn);
	REQUIRE(!cli.is_connected());
	REQUIRE(MQTTASYNC_BAD_QOS == return_code);
}

TEST_CASE("async_client connect 2 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mock_action_listener listener;
	mqtt::token_ptr token_conn{cli.connect(&CONTEXT, listener)};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());
	REQUIRE(CONTEXT == *static_cast<int*>(token_conn->get_user_context()));
	REQUIRE(listener.succeeded());
}

TEST_CASE("async_client connect 3 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::connect_options co;
	mock_action_listener listener;
	mqtt::token_ptr token_conn{cli.connect(co, &CONTEXT, listener)};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());
	REQUIRE(CONTEXT == *static_cast<int*>(token_conn->get_user_context()));
	REQUIRE(listener.succeeded());
}

TEST_CASE("async_client connect 3 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn;     //{ nullptr };
	mqtt::connect_options co;
	mqtt::will_options wo;
	wo.set_qos(BAD_QOS); // Invalid QoS causes connection failure
	co.set_will(wo);
	mock_action_listener listener;
	int reasonCode = MQTTASYNC_SUCCESS;
	try {
		token_conn = cli.connect(co, &CONTEXT, listener);
		REQUIRE(token_conn);
		token_conn->wait();
	}
	catch (mqtt::exception& ex) {
		reasonCode = ex.get_return_code();
	}
	REQUIRE(nullptr == token_conn);
	REQUIRE(!cli.is_connected());
	REQUIRE(MQTTASYNC_BAD_QOS == reasonCode);
	// TODO Why listener.on_failure() is not called?
	//REQUIRE(listener.failed());
}

// An improperly initialized SSL connect request should fail gracefully
TEST_CASE("async_client connect uninitialized ssl", "[client]")
{
	int reasonCode = MQTTASYNC_SUCCESS;
	try {
		// Compiled against a non-SSL library should throw here.
		mqtt::async_client cli{GOOD_SSL_SERVER_URI, CLIENT_ID};

		mqtt::connect_options opts;
		opts.set_keep_alive_interval(10);
		opts.set_clean_session(true);
		// Note that we're not setting SSL options.

		mqtt::token_ptr tok;

		// Compiled against the SSL library should throw here
		tok = cli.connect(opts);
		tok->wait();
	}
	catch (mqtt::exception& ex) {
		reasonCode = ex.get_return_code();
	}
	REQUIRE(reasonCode != MQTTASYNC_SUCCESS);
}

//----------------------------------------------------------------------
// Test async_client::disconnect()
//----------------------------------------------------------------------

TEST_CASE("async_client disconnect 0 arg", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client disconnect 1 arg", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mqtt::token_ptr token_disconn{cli.disconnect(0)};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client disconnect 1 arg failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_disconn;  //{ nullptr };
	int return_code = MQTTASYNC_SUCCESS;
	try {
		token_disconn = cli.disconnect(0);
		REQUIRE(token_disconn);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(!cli.is_connected());
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

TEST_CASE("async_client disconnect 2 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mock_action_listener listener;
	mqtt::token_ptr token_disconn{cli.disconnect(&CONTEXT, listener)};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
	REQUIRE(CONTEXT == *static_cast<int*>(token_disconn->get_user_context()));
}

TEST_CASE("async_client disconnect 3 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mock_action_listener listener;
	mqtt::token_ptr token_disconn{cli.disconnect(0, &CONTEXT, listener)};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
	REQUIRE(CONTEXT == *static_cast<int*>(token_disconn->get_user_context()));
}

TEST_CASE("async_client disconnect 3 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_disconn;  //{ nullptr };
	mock_action_listener listener;
	int return_code = MQTTASYNC_SUCCESS;
	try {
		token_disconn = cli.disconnect(0, &CONTEXT, listener);
		REQUIRE(token_disconn);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(!cli.is_connected());
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

//----------------------------------------------------------------------
// Test async_client::get_pending_delivery_token()
//----------------------------------------------------------------------

TEST_CASE("async_client get pending delivery token", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	REQUIRE(0 == GOOD_QOS_COLL[0]);
	REQUIRE(1 == GOOD_QOS_COLL[1]);
	REQUIRE(2 == GOOD_QOS_COLL[2]);

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	// NOTE: async_client::publish() is the only method that adds
	// delivery_token via async_client::add_token(delivery_token_ptr tok).
	// The other functions add token async_client::add_token(token_ptr tok).

	mqtt::delivery_token_ptr token_pub; // { nullptr };
	mqtt::delivery_token_ptr token_pending; // { nullptr };

	// NOTE: message IDs are 16-bit numbers sequentially incremented, from
	// 1 to 65535 (MAX_MSG_ID). See MQTTAsync_assignMsgId() at Paho MQTT C.
	int message_id = 1;

	// NOTE: All of the MQTT messages that require a response/acknowledge
	// should have a non-zero 16-bit message ID. This mainly applies to a
	// message with QOS=1 or QOS=2. The C++ library keeps a collection of
	// pointers to token objects for all of these messages that are in
	// flight. When the acknowledge comes back from the broker, the C++
	// library can look up the token from the msgID and signal it, indicating
	// completion.

	// Messages with QOS=2 are kept by the library
	mqtt::message_ptr msg2{mqtt::message::create(TOPIC, PAYLOAD, GOOD_QOS_COLL[2], RETAINED)};
	token_pub = cli.publish(msg2);
	REQUIRE(token_pub);
	token_pending = cli.get_pending_delivery_token(message_id++);
	REQUIRE(token_pending);

	// Messages with QOS=1 are kept by the library
	mqtt::message_ptr msg1{mqtt::message::create(TOPIC, PAYLOAD, GOOD_QOS_COLL[1], RETAINED)};
	token_pub = cli.publish(msg1);
	REQUIRE(token_pub);
	token_pending = cli.get_pending_delivery_token(message_id++);
	REQUIRE(token_pending);

	// NOTE: Messages with QOS=0 are fire-and-forget. These just get sent
	// to the broker without any tracking. Their tokens are signaled as
	// "complete" in the send function (by the calling thread).  So, as
	// soon as send returns, the message is considered completed. These
	// have a msgID that is always zero.

	// Messages with QOS=0 are NOT kept by the library
	mqtt::message_ptr msg0{mqtt::message::create(TOPIC, PAYLOAD, GOOD_QOS_COLL[0], RETAINED)};
	token_pub = cli.publish(msg0);
	REQUIRE(token_pub);
	token_pending = cli.get_pending_delivery_token(message_id++);
	REQUIRE(!token_pending);

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client get pending delivery tokens", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	REQUIRE(0 == GOOD_QOS_COLL[0]);
	REQUIRE(1 == GOOD_QOS_COLL[1]);
	REQUIRE(2 == GOOD_QOS_COLL[2]);

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mqtt::delivery_token_ptr token_pub; // { nullptr };

	// NOTE: async_client::publish() is the only method that adds
	// delivery_token via async_client::add_token(delivery_token_ptr tok).
	// The other functions add token async_client::add_token(token_ptr tok).

	// Messages with QOS=0 are NOT kept by the library
	mqtt::message_ptr msg0{mqtt::message::create(TOPIC, PAYLOAD, GOOD_QOS_COLL[0], RETAINED)};
	token_pub = cli.publish(msg0);
	REQUIRE(token_pub);

	// Messages with QOS=1 are kept by the library
	mqtt::message_ptr msg1{mqtt::message::create(TOPIC, PAYLOAD, GOOD_QOS_COLL[1], RETAINED)};
	token_pub = cli.publish(msg1);
	REQUIRE(token_pub);

	// Messages with QOS=2 are kept by the library
	mqtt::message_ptr msg2{mqtt::message::create(TOPIC, PAYLOAD, GOOD_QOS_COLL[2], RETAINED)};
	token_pub = cli.publish(msg2);
	REQUIRE(token_pub);

	// NOTE: Only tokens for messages with QOS=1 and QOS=2 are kept. That's
	// why the vector's size does not account for QOS=0 message tokens
	std::vector<mqtt::delivery_token_ptr> tokens_pending{cli.get_pending_delivery_tokens()};
	REQUIRE(2 == static_cast<int>(tokens_pending.size()));

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

//----------------------------------------------------------------------
// Test async_client::publish()
//----------------------------------------------------------------------

TEST_CASE("async_client publish 2 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mqtt::message_ptr msg{mqtt::message::create(TOPIC, PAYLOAD)};
	mqtt::delivery_token_ptr token_pub{cli.publish(msg)};
	REQUIRE(token_pub);
	token_pub->wait_for(TIMEOUT);

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client publish 2 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	int return_code = MQTTASYNC_SUCCESS;
	try {
		mqtt::message_ptr msg{mqtt::message::create(TOPIC, PAYLOAD)};
		mqtt::delivery_token_ptr token_pub{cli.publish(msg)};
		REQUIRE(token_pub);
		token_pub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

TEST_CASE("async_client publish 4 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mqtt::message_ptr msg{mqtt::message::create(TOPIC, PAYLOAD)};
	mock_action_listener listener;
	mqtt::delivery_token_ptr token_pub{cli.publish(msg, &CONTEXT, listener)};
	REQUIRE(token_pub);
	token_pub->wait_for(TIMEOUT);
	REQUIRE(CONTEXT == *static_cast<int*>(token_pub->get_user_context()));

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client publish 4 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	int return_code = MQTTASYNC_SUCCESS;
	try {
		mqtt::message_ptr msg{mqtt::message::create(TOPIC, PAYLOAD)};
		mock_action_listener listener;
		mqtt::delivery_token_ptr token_pub{cli.publish(msg, &CONTEXT, listener)};
		REQUIRE(token_pub);
		token_pub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

TEST_CASE("async_client publish 5 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	const void* payload{PAYLOAD.data()};
	const size_t payload_size{PAYLOAD.size()};
	mqtt::delivery_token_ptr token_pub{cli.publish(TOPIC, payload, payload_size, GOOD_QOS, RETAINED)};
	REQUIRE(token_pub);
	token_pub->wait_for(TIMEOUT);

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client publish 7 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	const void* payload{PAYLOAD.c_str()};
	const size_t payload_size{PAYLOAD.size()};
	mock_action_listener listener;
	mqtt::delivery_token_ptr token_pub{cli.publish(TOPIC, payload, payload_size, GOOD_QOS, RETAINED, &CONTEXT, listener)};
	REQUIRE(token_pub);
	token_pub->wait_for(TIMEOUT);
	REQUIRE(CONTEXT == *static_cast<int*>(token_pub->get_user_context()));

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

//----------------------------------------------------------------------
// Test async_client::set_callback()
//----------------------------------------------------------------------

TEST_CASE("async_client set callback", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mock_callback cb;
	cli.set_callback(cb);

	//REQUIRE(cb.delivery_complete_called);
}

//----------------------------------------------------------------------
// Test async_client::subscribe()
//----------------------------------------------------------------------

TEST_CASE("async_client subscribe single topic 2 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mqtt::token_ptr token_sub{cli.subscribe(TOPIC, GOOD_QOS)};
	REQUIRE(token_sub);
	token_sub->wait_for(TIMEOUT);

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client subscribe single topic 2 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	int return_code = MQTTASYNC_SUCCESS;
	try {
		mqtt::token_ptr token_sub{cli.subscribe(TOPIC, BAD_QOS)};
		REQUIRE(token_sub);
		token_sub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

TEST_CASE("async_client subscribe single topic 4 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mock_action_listener listener;
	mqtt::token_ptr token_sub{cli.subscribe(TOPIC, GOOD_QOS, &CONTEXT, listener)};
	REQUIRE(token_sub);
	token_sub->wait_for(TIMEOUT);
	REQUIRE(CONTEXT == *static_cast<int*>(token_sub->get_user_context()));

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client subscribe single topic 4 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	int return_code = MQTTASYNC_SUCCESS;
	try {
		mock_action_listener listener;
		mqtt::token_ptr token_sub{cli.subscribe(TOPIC, BAD_QOS, &CONTEXT, listener)};
		REQUIRE(token_sub);
		token_sub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

TEST_CASE("async_client subscribe many topics 2 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	cli.connect()->wait();
	REQUIRE(cli.is_connected());

	try {
		cli.subscribe(TOPIC_COLL, GOOD_QOS_COLL)->wait_for(TIMEOUT);
	}
	catch (const mqtt::exception& exc) {
		FAIL(exc.what());
	}

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

// There was an odd failure when subscribe_many was given a single topic.
TEST_CASE("async_client subscribe many topics 2 args_single", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	cli.connect()->wait();
	REQUIRE(cli.is_connected());

	mqtt::const_string_collection_ptr TOPIC_1_COLL{
		mqtt::string_collection::create({ "TOPIC0" })
	};
	mqtt::iasync_client::qos_collection GOOD_QOS_1_COLL{0};
	try {
		cli.subscribe(TOPIC_1_COLL, GOOD_QOS_1_COLL)->wait_for(TIMEOUT);
	}
	catch (const mqtt::exception& exc) {
		FAIL(exc.what());
	}

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client subscribe many topics 2 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	try {
		mqtt::token_ptr token_sub{cli.subscribe(TOPIC_COLL, BAD_QOS_COLL)};
		REQUIRE(token_sub);
		token_sub->wait_for(TIMEOUT);
	}
	catch (const mqtt::exception& ex) {
		//REQUIRE(MQTTASYNC_BAD_QOS == ex.get_return_code());
	}

	int return_code = MQTTASYNC_SUCCESS;
	try {
		mqtt::token_ptr token_sub{cli.subscribe(TOPIC_COLL, GOOD_QOS_COLL)};
		REQUIRE(token_sub);
		token_sub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

TEST_CASE("async_client subscribe many topics 4 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mock_action_listener listener;
	mqtt::token_ptr token_sub{cli.subscribe(TOPIC_COLL, GOOD_QOS_COLL, &CONTEXT, listener)};
	REQUIRE(token_sub);
	token_sub->wait_for(TIMEOUT);
	REQUIRE(CONTEXT == *static_cast<int*>(token_sub->get_user_context()));

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client subscribe many topics 4 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mock_action_listener listener;

	try {
		cli.subscribe(TOPIC_COLL, BAD_QOS_COLL, &CONTEXT, listener)->wait_for(TIMEOUT);
	}
	catch (const mqtt::exception& ex) {
		//REQUIRE(MQTTASYNC_BAD_QOS == ex.get_return_code());
	}

	int return_code = MQTTASYNC_SUCCESS;
	try {
		mqtt::token_ptr token_sub{cli.subscribe(TOPIC_COLL, GOOD_QOS_COLL, &CONTEXT, listener)};
		REQUIRE(token_sub);
		token_sub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

//----------------------------------------------------------------------
// Test async_client::unsubscribe()
//----------------------------------------------------------------------

TEST_CASE("async_client unsubscribe single topic 1 arg", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mqtt::token_ptr token_unsub{cli.unsubscribe(TOPIC)};
	REQUIRE(token_unsub);
	token_unsub->wait_for(TIMEOUT);

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client unsubscribe single topic 1 arg failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	int return_code = MQTTASYNC_SUCCESS;
	try {
		mqtt::token_ptr token_unsub{cli.unsubscribe(TOPIC)};
		REQUIRE(token_unsub);
		token_unsub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

TEST_CASE("async_client unsubscribe single topic 3 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mock_action_listener listener;
	mqtt::token_ptr token_unsub{cli.unsubscribe(TOPIC, &CONTEXT, listener)};
	REQUIRE(token_unsub);
	token_unsub->wait_for(TIMEOUT);
	REQUIRE(CONTEXT == *static_cast<int*>(token_unsub->get_user_context()));

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client unsubscribe single topic 3 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	int return_code = MQTTASYNC_SUCCESS;
	try {
		mock_action_listener listener;
		mqtt::token_ptr token_unsub{cli.unsubscribe(TOPIC, &CONTEXT, listener)};
		REQUIRE(token_unsub);
		token_unsub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

TEST_CASE("async_client unsubscribe many topics 1 arg", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mqtt::token_ptr token_unsub{cli.unsubscribe(TOPIC_COLL)};
	REQUIRE(token_unsub);
	token_unsub->wait_for(TIMEOUT);

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client unsubscribe many topics 1 arg_failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	int return_code = MQTTASYNC_SUCCESS;
	try {
		mqtt::token_ptr token_unsub{cli.unsubscribe(TOPIC_COLL)};
		REQUIRE(token_unsub);
		token_unsub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

TEST_CASE("async_client unsubscribe many topics 3 args", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mqtt::token_ptr token_conn{cli.connect()};
	REQUIRE(token_conn);
	token_conn->wait();
	REQUIRE(cli.is_connected());

	mock_action_listener listener;
	mqtt::token_ptr token_unsub{cli.unsubscribe(TOPIC_COLL, &CONTEXT, listener)};
	REQUIRE(token_unsub);
	token_unsub->wait_for(TIMEOUT);
	REQUIRE(CONTEXT == *static_cast<int*>(token_unsub->get_user_context()));

	mqtt::token_ptr token_disconn{cli.disconnect()};
	REQUIRE(token_disconn);
	token_disconn->wait();
	REQUIRE(!cli.is_connected());
}

TEST_CASE("async_client unsubscribe many topics 3 args failure", "[client]")
{
	mqtt::async_client cli{GOOD_SERVER_URI, CLIENT_ID};
	REQUIRE(!cli.is_connected());

	mock_action_listener listener;
	int return_code = MQTTASYNC_SUCCESS;
	try {
		mqtt::token_ptr token_unsub{cli.unsubscribe(TOPIC_COLL, &CONTEXT, listener)};
		REQUIRE(token_unsub);
		token_unsub->wait_for(TIMEOUT);
	}
	catch (mqtt::exception& ex) {
		return_code = ex.get_return_code();
	}
	REQUIRE(MQTTASYNC_DISCONNECTED == return_code);
}

