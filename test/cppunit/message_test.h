// message_test.h
// Unit tests for the message class in the Paho MQTT C++ library.

/*******************************************************************************
 * Copyright (c) 2016 Frank Pagliughi <fpagliughi@mindspring.com>
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
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_message_test_h
#define __mqtt_message_test_h

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/message.h"
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class message_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( message_test );

	CPPUNIT_TEST( test_dflt_constructor );
	CPPUNIT_TEST( test_buf_len_constructor  );
	CPPUNIT_TEST( test_buf_constructor  );
	CPPUNIT_TEST( test_string_constructor  );
	CPPUNIT_TEST( test_string_qos_constructor );
	CPPUNIT_TEST( test_c_struct_constructor );
	CPPUNIT_TEST( test_copy_constructor );
	CPPUNIT_TEST( test_move_constructor );
	CPPUNIT_TEST( test_copy_assignment );
	CPPUNIT_TEST( test_move_assignment );
	CPPUNIT_TEST( test_validate_qos );

	CPPUNIT_TEST_SUITE_END();

	const std::string EMPTY_STR;
	const int DFLT_QOS = 0;
	const bool DFLT_RETAINED = false;
	const bool DFLT_DUP = false;

	const std::string TOPIC { "hello" };
	const char* BUF = "Hello there";
	const size_t N = std::strlen(BUF);
	const std::string PAYLOAD = std::string(BUF);
	const int QOS = 1;

	mqtt::message orgMsg;

public:
	void setUp() {
		orgMsg = mqtt::message(TOPIC, PAYLOAD, QOS, true);
	}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

	void test_dflt_constructor() {
		mqtt::message msg;
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, msg.get_qos());
		CPPUNIT_ASSERT_EQUAL(DFLT_RETAINED, msg.is_retained());
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, msg.is_duplicate());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(0, c_struct.payloadlen);
		CPPUNIT_ASSERT(nullptr == c_struct.payload);
		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, c_struct.qos);
		CPPUNIT_ASSERT_EQUAL(DFLT_RETAINED, c_struct.retained != 0);
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, c_struct.dup != 0);
	}

// ----------------------------------------------------------------------
// Test the raw buffer (void*) and length constructor
// ----------------------------------------------------------------------

	void test_buf_len_constructor() {
		mqtt::message msg(TOPIC, BUF, N);

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, msg.get_qos());
		CPPUNIT_ASSERT_EQUAL(DFLT_RETAINED, msg.is_retained());
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, msg.is_duplicate());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(int(N), c_struct.payloadlen);
		CPPUNIT_ASSERT(!memcmp(BUF, c_struct.payload, N));
		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, c_struct.qos);
		CPPUNIT_ASSERT_EQUAL(DFLT_RETAINED, c_struct.retained != 0);
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, c_struct.dup != 0);
	}

// ----------------------------------------------------------------------
// Test the raw buffer (void*) constructor
// ----------------------------------------------------------------------

	void test_buf_constructor() {
		mqtt::message msg(TOPIC, BUF, N, QOS, true);

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, msg.is_duplicate());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(int(N), c_struct.payloadlen);
		CPPUNIT_ASSERT(!memcmp(BUF, c_struct.payload, N));
		CPPUNIT_ASSERT_EQUAL(QOS, c_struct.qos);
		CPPUNIT_ASSERT(c_struct.retained != 0);
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, c_struct.dup != 0);
	}

// ----------------------------------------------------------------------
// Test the string buffer constructor
// ----------------------------------------------------------------------

	void test_string_constructor() {
		mqtt::message msg(TOPIC, PAYLOAD);

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, msg.get_qos());
		CPPUNIT_ASSERT(!msg.is_retained());
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, msg.is_duplicate());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(int(PAYLOAD.size()), c_struct.payloadlen);
		CPPUNIT_ASSERT(!memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, c_struct.qos);
		CPPUNIT_ASSERT_EQUAL(DFLT_RETAINED, c_struct.retained != 0);
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, c_struct.dup != 0);
	}

// ----------------------------------------------------------------------
// Test the string buffer with QoS constructor
// ----------------------------------------------------------------------

	void test_string_qos_constructor() {
		mqtt::message msg(TOPIC, PAYLOAD, QOS, true);

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, msg.is_duplicate());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(int(PAYLOAD.size()), c_struct.payloadlen);
		CPPUNIT_ASSERT(!memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
		CPPUNIT_ASSERT_EQUAL(QOS, c_struct.qos);
		CPPUNIT_ASSERT(c_struct.retained != 0);
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, c_struct.dup != 0);
	}

// ----------------------------------------------------------------------
// Test the initialization by C struct
// ----------------------------------------------------------------------

	void test_c_struct_constructor() {
		MQTTAsync_message c_msg = MQTTAsync_message_initializer;

		c_msg.payload = const_cast<char*>(BUF);
		c_msg.payloadlen = N;
		c_msg.qos = QOS;
		c_msg.retained = 1;
		c_msg.dup = 1;

		mqtt::message msg(TOPIC, c_msg);

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
		CPPUNIT_ASSERT(msg.is_duplicate());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(int(N), c_struct.payloadlen);
		CPPUNIT_ASSERT(!memcmp(BUF, c_struct.payload, N));
		CPPUNIT_ASSERT_EQUAL(QOS, c_struct.qos);
		CPPUNIT_ASSERT(c_struct.retained != 0);
		CPPUNIT_ASSERT(c_struct.dup != 0);
	}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

	void test_copy_constructor() {
		mqtt::message msg(orgMsg);

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(int(PAYLOAD.size()), c_struct.payloadlen);
		CPPUNIT_ASSERT(!memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
		CPPUNIT_ASSERT_EQUAL(QOS, c_struct.qos);
		CPPUNIT_ASSERT(c_struct.retained != 0);
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, c_struct.dup != 0);

		// Make sure it's a true copy, not linked to the original
		orgMsg.set_payload(EMPTY_STR);
		orgMsg.set_qos(DFLT_QOS);
		orgMsg.set_retained(false);

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
	}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

	void test_move_constructor() {
		mqtt::message msg(std::move(orgMsg));

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(int(PAYLOAD.size()), c_struct.payloadlen);
		CPPUNIT_ASSERT(!memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
		CPPUNIT_ASSERT_EQUAL(QOS, c_struct.qos);
		CPPUNIT_ASSERT(c_struct.retained != 0);
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, c_struct.dup != 0);

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(size_t(0), orgMsg.get_payload().size());
	}

// ----------------------------------------------------------------------
// Test the copy assignment operator=(const&)
// ----------------------------------------------------------------------

	void test_copy_assignment() {
		mqtt::message msg;

		msg = orgMsg;

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(int(PAYLOAD.size()), c_struct.payloadlen);
		CPPUNIT_ASSERT(!memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
		CPPUNIT_ASSERT_EQUAL(QOS, c_struct.qos);
		CPPUNIT_ASSERT(c_struct.retained != 0);
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, c_struct.dup != 0);

		// Make sure it's a true copy, not linked to the original
		orgMsg.set_payload(EMPTY_STR);
		orgMsg.set_qos(DFLT_QOS);
		orgMsg.set_retained(false);

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		// Self assignment should cause no harm
		msg = msg;

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
	}

// ----------------------------------------------------------------------
// Test the move assignment, operator=(&&)
// ----------------------------------------------------------------------

	void test_move_assignment() {
		mqtt::message msg;
		msg = std::move(orgMsg);

		CPPUNIT_ASSERT_EQUAL(TOPIC, msg.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		const auto& c_struct = msg.msg_;

		CPPUNIT_ASSERT_EQUAL(int(PAYLOAD.size()), c_struct.payloadlen);
		CPPUNIT_ASSERT(!memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
		CPPUNIT_ASSERT_EQUAL(QOS, c_struct.qos);
		CPPUNIT_ASSERT(c_struct.retained != 0);
		CPPUNIT_ASSERT_EQUAL(DFLT_DUP, c_struct.dup != 0);

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(size_t(0), orgMsg.get_payload().size());

		// Self assignment should cause no harm
		// (clang++ is smart enough to warn about this)
		#if !defined(__clang__)
			msg = std::move(msg);
			CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload_str());
			CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
			CPPUNIT_ASSERT(msg.is_retained());
		#endif
	}

// ----------------------------------------------------------------------
// Test the validate_qos()
// ----------------------------------------------------------------------

	void test_validate_qos() {
		try {
			mqtt::message::validate_qos(-1);
			CPPUNIT_FAIL("message shouldn't accept QoS <min");
		} 
		catch (const mqtt::exception& ex) {}

		for (int i=0; i<=2; ++i) {
			try {
				mqtt::message::validate_qos(0);
			} 
			catch (...) {
				CPPUNIT_FAIL("mesage should accept valid QoS: "+std::to_string(i));
			}
		}

		try {
			mqtt::message::validate_qos(3);
			CPPUNIT_FAIL("message shouldn't accept QoS >max");
		}
		catch (const mqtt::exception& ex) {}
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_message_test_h

