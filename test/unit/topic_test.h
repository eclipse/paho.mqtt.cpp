// topic_test.h
// Unit tests for the topic class in the Paho MQTT C++ library.

/*******************************************************************************
 * Copyright (c) 2016 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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
 *******************************************************************************/

#ifndef __mqtt_topic_test_h
#define __mqtt_topic_test_h

#include <algorithm>
#include <memory>
#include <stdexcept>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/topic.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class topic_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( topic_test );

	CPPUNIT_TEST( test_user_constructor );
	CPPUNIT_TEST( test_publish_1_arg );
	CPPUNIT_TEST( test_publish_3_arg );
	CPPUNIT_TEST( test_publish_4_arg );
	CPPUNIT_TEST( test_get_name );
	CPPUNIT_TEST( test_to_str );

	CPPUNIT_TEST_SUITE_END();

	const std::string TOPIC_NAME { "topic_name" };
	mqtt::test::dummy_async_client cli;

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

	void test_user_constructor() {
		mqtt::topic topic{ TOPIC_NAME, cli };

		CPPUNIT_ASSERT_EQUAL(TOPIC_NAME, topic.get_name());
		CPPUNIT_ASSERT_EQUAL(TOPIC_NAME, topic.to_string());
	}

// ----------------------------------------------------------------------
// Test publish with one argument
// ----------------------------------------------------------------------

	void test_publish_1_arg() {
		mqtt::topic topic{ TOPIC_NAME, cli };

		mqtt::const_message_ptr msg_in { new mqtt::message { TOPIC_NAME, "message" } };

		mqtt::delivery_token_ptr token { topic.publish(msg_in) };
		CPPUNIT_ASSERT(token);

		mqtt::const_message_ptr msg_out { token->get_message() };
		CPPUNIT_ASSERT(msg_out);

		CPPUNIT_ASSERT_EQUAL(msg_in->get_payload_str(), msg_out->get_payload_str());
		CPPUNIT_ASSERT_EQUAL(msg_in->get_qos(), msg_out->get_qos());
		CPPUNIT_ASSERT_EQUAL(0, msg_out->get_qos());
	}

// ----------------------------------------------------------------------
// Test publish with three arguments
// ----------------------------------------------------------------------

	void test_publish_3_arg() {
		mqtt::topic topic{ TOPIC_NAME, cli };

		std::string payload { "message" };
		int qos { 1 };

		mqtt::delivery_token_ptr token { topic.publish(payload, qos, false) };
		CPPUNIT_ASSERT(token);

		mqtt::const_message_ptr msg_out { token->get_message() };
		CPPUNIT_ASSERT(msg_out);

		CPPUNIT_ASSERT_EQUAL(payload, msg_out->get_payload_str());
		CPPUNIT_ASSERT_EQUAL(qos, msg_out->get_qos());
	}

// ----------------------------------------------------------------------
// Test publish with four arguments
// ----------------------------------------------------------------------

	void test_publish_4_arg() {
		mqtt::topic topic{ TOPIC_NAME, cli };

		std::string payload { "message" };
		std::size_t payload_size { payload.size() };
		int qos { 2 };

		mqtt::delivery_token_ptr token = topic.publish(payload.c_str(), payload_size, qos, false);
		CPPUNIT_ASSERT(token);

		mqtt::const_message_ptr msg_out = token->get_message();
		CPPUNIT_ASSERT(msg_out);

		CPPUNIT_ASSERT_EQUAL(payload, msg_out->get_payload_str());
		CPPUNIT_ASSERT_EQUAL(qos, msg_out->get_qos());
	}

// ----------------------------------------------------------------------
// Test get name
// ----------------------------------------------------------------------

	void test_get_name() {
		mqtt::topic topic{ TOPIC_NAME, cli };

		CPPUNIT_ASSERT_EQUAL(TOPIC_NAME, topic.get_name());
	}

// ----------------------------------------------------------------------
// Test to string
// ----------------------------------------------------------------------

	void test_to_str() {
		mqtt::topic topic { TOPIC_NAME, cli };

		CPPUNIT_ASSERT_EQUAL(TOPIC_NAME, topic.to_string());
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_topic_test_h
