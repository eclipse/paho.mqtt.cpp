// client_test.h
// Unit tests for the client class in the Paho MQTT C++ library.

/*******************************************************************************
 * Copyright (c) 2017 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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
 *    Frank Pagliughi - updated tests for modified v1.0 client API
 *******************************************************************************/

#ifndef __mqtt_client_test_h
#define __mqtt_client_test_h

#include <stdexcept>
#include <vector>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/client.h"

#include "dummy_client_persistence.h"
#include "dummy_callback.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class client_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( client_test );

	CPPUNIT_TEST( test_user_constructor_2_string_args );
	CPPUNIT_TEST( test_user_constructor_3_string_args );
	CPPUNIT_TEST( test_user_constructor_3_args );

	CPPUNIT_TEST( test_connect_0_arg );
	CPPUNIT_TEST( test_connect_1_arg );
	CPPUNIT_TEST( test_connect_1_arg_failure );

	CPPUNIT_TEST( test_disconnect_0_arg );
	CPPUNIT_TEST( test_disconnect_1_arg );
	CPPUNIT_TEST( test_disconnect_1_arg_failure );

	CPPUNIT_TEST( test_timeout_int );
	CPPUNIT_TEST( test_timeout_duration );

	CPPUNIT_TEST( test_get_topic );

	CPPUNIT_TEST( test_publish_pointer_2_args );
	CPPUNIT_TEST( test_publish_pointer_2_args_failure );
	CPPUNIT_TEST( test_publish_reference_2_args );
	CPPUNIT_TEST( test_publish_5_args );

	CPPUNIT_TEST( test_set_callback );

	CPPUNIT_TEST( test_subscribe_single_topic_1_arg );
	CPPUNIT_TEST( test_subscribe_single_topic_1_arg_failure );
	CPPUNIT_TEST( test_subscribe_single_topic_2_args );
	CPPUNIT_TEST( test_subscribe_single_topic_2_args_failure );
	CPPUNIT_TEST( test_subscribe_many_topics_1_arg );
	CPPUNIT_TEST( test_subscribe_many_topics_1_arg_failure );
	CPPUNIT_TEST( test_subscribe_many_topics_2_args );
	CPPUNIT_TEST( test_subscribe_many_topics_2_args_failure );

	CPPUNIT_TEST( test_unsubscribe_single_topic_1_arg );
	CPPUNIT_TEST( test_unsubscribe_single_topic_1_arg_failure );
	CPPUNIT_TEST( test_unsubscribe_many_topics_1_arg );
	CPPUNIT_TEST( test_unsubscribe_many_topics_1_arg_failure );

	CPPUNIT_TEST_SUITE_END();

	// NOTE: This test case requires network access. It uses one of
	//  	 the public available MQTT brokers
	#if defined(TEST_EXTERNAL_SERVER)
		const std::string GOOD_SERVER_URI { "tcp://m2m.eclipse.org:1883" };
	#else
		const std::string GOOD_SERVER_URI { "tcp://localhost:1883" };
	#endif
	const std::string BAD_SERVER_URI  { "one://invalid.address" };
	const std::string CLIENT_ID { "client_test" };
	const std::string PERSISTENCE_DIR { "/tmp" };
	const std::string TOPIC { "TOPIC" };
	const int GOOD_QOS { 0 };
	const int BAD_QOS  { 3 };
	mqtt::string_collection TOPIC_COLL { "TOPIC0", "TOPIC1", "TOPIC2" };
	mqtt::client::qos_collection GOOD_QOS_COLL { 0, 1, 2 };
	mqtt::client::qos_collection BAD_QOS_COLL  { BAD_QOS };
	const std::string PAYLOAD { "PAYLOAD" };
	//const int TIMEOUT { 1000 };
	//int CONTEXT { 4 };
	mqtt::test::dummy_action_listener listener;
	const bool RETAINED { false };

public:
	void setUp() {}
	void tearDown() {}

//----------------------------------------------------------------------
// Test constructors client::client()
//----------------------------------------------------------------------

	void test_user_constructor_2_string_args() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };

		CPPUNIT_ASSERT_EQUAL(GOOD_SERVER_URI, cli.get_server_uri());
		CPPUNIT_ASSERT_EQUAL(CLIENT_ID, cli.get_client_id());
	}

	void test_user_constructor_3_string_args() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID, PERSISTENCE_DIR };

		CPPUNIT_ASSERT_EQUAL(GOOD_SERVER_URI, cli.get_server_uri());
		CPPUNIT_ASSERT_EQUAL(CLIENT_ID, cli.get_client_id());
	}

	void test_user_constructor_3_args() {
		mqtt::test::dummy_client_persistence cp;
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID, &cp };

		CPPUNIT_ASSERT_EQUAL(GOOD_SERVER_URI, cli.get_server_uri());
		CPPUNIT_ASSERT_EQUAL(CLIENT_ID, cli.get_client_id());

		mqtt::client cli_no_persistence { GOOD_SERVER_URI, CLIENT_ID, nullptr };

		CPPUNIT_ASSERT_EQUAL(GOOD_SERVER_URI, cli_no_persistence.get_server_uri());
		CPPUNIT_ASSERT_EQUAL(CLIENT_ID, cli_no_persistence.get_client_id());
	}

//----------------------------------------------------------------------
// Test client::connect()
//----------------------------------------------------------------------

	void test_connect_0_arg() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());
	}

	void test_connect_1_arg() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		mqtt::connect_options co;
		cli.connect(co);
		CPPUNIT_ASSERT(cli.is_connected());
	}

	void test_connect_1_arg_failure() {
		mqtt::client cli { BAD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		mqtt::connect_options co;
		mqtt::will_options wo;
		wo.set_qos(BAD_QOS); // Invalid QoS causes connection failure
		co.set_will(wo);
		int reason_code = MQTTASYNC_SUCCESS;
		try {
			cli.connect(co);
		} catch (mqtt::exception& ex) {
			reason_code = ex.get_reason_code();
		}
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_BAD_QOS, reason_code);
	}

//----------------------------------------------------------------------
// Test client::disconnect()
//----------------------------------------------------------------------

	void test_disconnect_0_arg() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_disconnect_1_arg() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		cli.disconnect(0);
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_disconnect_1_arg_failure() {
		mqtt::client cli { BAD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		int reason_code = MQTTASYNC_SUCCESS;
		try {
			cli.disconnect(0);
		} catch (mqtt::exception& ex) {
			reason_code = ex.get_reason_code();
		}
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_DISCONNECTED, reason_code);
	}

//----------------------------------------------------------------------
// Test client::get_timeout() and client::set_timeout() using ints
//----------------------------------------------------------------------

	void test_timeout_int() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		int timeout { std::numeric_limits<int>::min() };
		cli.set_timeout(timeout);
		CPPUNIT_ASSERT_EQUAL(timeout, (int) cli.get_timeout().count());

		timeout = 0;
		cli.set_timeout(timeout);
		CPPUNIT_ASSERT_EQUAL(timeout, (int) cli.get_timeout().count());

		timeout = std::numeric_limits<int>::max();
		cli.set_timeout(timeout);
		CPPUNIT_ASSERT_EQUAL(timeout, (int) cli.get_timeout().count());
	}

//----------------------------------------------------------------------
// Test client::get_timeout() and client::set_timeout() using durations
//----------------------------------------------------------------------

	void test_timeout_duration() {
		const int TIMEOUT_SEC = 120;
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };

		std::chrono::seconds timeout{ TIMEOUT_SEC };
		cli.set_timeout(timeout);
		CPPUNIT_ASSERT(timeout == cli.get_timeout());
		CPPUNIT_ASSERT_EQUAL(TIMEOUT_SEC*1000, (int) cli.get_timeout().count());
	}

//----------------------------------------------------------------------
// Test client::get_topic()
//----------------------------------------------------------------------

	void test_get_topic() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		mqtt::topic t { cli.get_topic(TOPIC) };
		CPPUNIT_ASSERT_EQUAL(TOPIC, t.get_name());
	}

//----------------------------------------------------------------------
// Test client::publish()
//----------------------------------------------------------------------

	void test_publish_pointer_2_args() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		mqtt::message_ptr msg { mqtt::message::create(TOPIC, PAYLOAD) };
		cli.publish(msg);

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_publish_pointer_2_args_failure() {
		mqtt::client cli { BAD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		int reason_code = MQTTASYNC_SUCCESS;
		try {
			mqtt::message_ptr msg { mqtt::message::create(TOPIC, PAYLOAD) };
			cli.publish(msg);
		} catch (mqtt::exception& ex) {
			reason_code = ex.get_reason_code();
		}
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_DISCONNECTED, reason_code);
	}

	void test_publish_reference_2_args() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		mqtt::message msg { TOPIC, PAYLOAD };
		cli.publish(msg);

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_publish_5_args() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		const void* payload { PAYLOAD.c_str() };
		const size_t payload_size { PAYLOAD.size() };
		cli.publish(TOPIC, payload, payload_size, GOOD_QOS, RETAINED);

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

//----------------------------------------------------------------------
// Test client::set_callback()
//----------------------------------------------------------------------

	void test_set_callback() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		mqtt::test::dummy_callback cb;
		cli.set_callback(cb);
	}

//----------------------------------------------------------------------
// Test client::subscribe()
//----------------------------------------------------------------------

	void test_subscribe_single_topic_1_arg() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		cli.subscribe(TOPIC);

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_subscribe_single_topic_1_arg_failure() {
		mqtt::client cli { BAD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		int reason_code = MQTTASYNC_SUCCESS;
		try {
			cli.subscribe(TOPIC);
		} catch (mqtt::exception& ex) {
			reason_code = ex.get_reason_code();
		}
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_DISCONNECTED, reason_code);
	}

	void test_subscribe_single_topic_2_args() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		cli.subscribe(TOPIC, GOOD_QOS);

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_subscribe_single_topic_2_args_failure() {
		mqtt::client cli { BAD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		int reason_code = MQTTASYNC_SUCCESS;
		try {
			cli.subscribe(TOPIC, BAD_QOS);
		} catch (mqtt::exception& ex) {
			reason_code = ex.get_reason_code();
		}
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_DISCONNECTED, reason_code);
	}

	void test_subscribe_many_topics_1_arg() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		cli.subscribe(TOPIC_COLL);

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_subscribe_many_topics_1_arg_failure() {
		mqtt::client cli { BAD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		int reason_code = MQTTASYNC_SUCCESS;
		try {
			cli.subscribe(TOPIC_COLL);
		} catch (mqtt::exception& ex) {
			reason_code = ex.get_reason_code();
		}
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_DISCONNECTED, reason_code);
	}

	void test_subscribe_many_topics_2_args() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		cli.subscribe(TOPIC_COLL, GOOD_QOS_COLL);

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_subscribe_many_topics_2_args_failure() {
		mqtt::client cli { BAD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		try {
			cli.subscribe(TOPIC_COLL, BAD_QOS_COLL);
		} catch (std::invalid_argument& ex) {}

		int reason_code = MQTTASYNC_SUCCESS;
		try {
			cli.subscribe(TOPIC_COLL, GOOD_QOS_COLL);
		} catch (mqtt::exception& ex) {
			reason_code = ex.get_reason_code();
		}
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_DISCONNECTED, reason_code);
	}

//----------------------------------------------------------------------
// Test client::unsubscribe()
//----------------------------------------------------------------------

	void test_unsubscribe_single_topic_1_arg() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		cli.unsubscribe(TOPIC);

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_unsubscribe_single_topic_1_arg_failure() {
		mqtt::client cli { BAD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		int reason_code = MQTTASYNC_SUCCESS;
		try {
			cli.unsubscribe(TOPIC);
		} catch (mqtt::exception& ex) {
			reason_code = ex.get_reason_code();
		}
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_DISCONNECTED, reason_code);
	}

	void test_unsubscribe_many_topics_1_arg() {
		mqtt::client cli { GOOD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		cli.connect();
		CPPUNIT_ASSERT(cli.is_connected());

		cli.unsubscribe(TOPIC_COLL);

		cli.disconnect();
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());
	}

	void test_unsubscribe_many_topics_1_arg_failure() {
		mqtt::client cli { BAD_SERVER_URI, CLIENT_ID };
		CPPUNIT_ASSERT_EQUAL(false, cli.is_connected());

		int reason_code = MQTTASYNC_SUCCESS;
		try {
			cli.unsubscribe(TOPIC_COLL);
		} catch (mqtt::exception& ex) {
			reason_code = ex.get_reason_code();
		}
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_DISCONNECTED, reason_code);
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_client_test_h
