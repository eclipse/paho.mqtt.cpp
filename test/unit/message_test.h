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
 *    Guilherme M. Ferreira - added more tests
 *    Guilherme M. Ferreira - changed test framework from CppUnit to GTest
 *******************************************************************************/

#ifndef __mqtt_message_test_h
#define __mqtt_message_test_h

#include <gtest/gtest.h>

#include "mqtt/message.h"
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class message_test : public ::testing::Test
{
protected:
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

	MQTTAsync_message& get_c_struct(mqtt::message& msg) {
		return msg.msg_;
	}

public:
	void SetUp() {
		orgMsg = mqtt::message(TOPIC, PAYLOAD, QOS, true);
	}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

TEST_F(message_test, test_dflt_constructor) {
	mqtt::message msg;
	EXPECT_EQ(EMPTY_STR, msg.get_payload_str());
	EXPECT_EQ(DFLT_QOS, msg.get_qos());
	EXPECT_EQ(DFLT_RETAINED, msg.is_retained());
	EXPECT_EQ(DFLT_DUP, msg.is_duplicate());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(0, c_struct.payloadlen);
	EXPECT_EQ(nullptr, c_struct.payload);
	EXPECT_EQ(DFLT_QOS, c_struct.qos);
	EXPECT_EQ(DFLT_RETAINED, c_struct.retained != 0);
	EXPECT_EQ(DFLT_DUP, c_struct.dup != 0);
}

// ----------------------------------------------------------------------
// Test the raw buffer (void*) and length constructor
// ----------------------------------------------------------------------

TEST_F(message_test, test_buf_len_constructor) {
	mqtt::message msg(TOPIC, BUF, N);

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(DFLT_QOS, msg.get_qos());
	EXPECT_EQ(DFLT_RETAINED, msg.is_retained());
	EXPECT_EQ(DFLT_DUP, msg.is_duplicate());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(int(N), c_struct.payloadlen);
	EXPECT_EQ(0, memcmp(BUF, c_struct.payload, N));
	EXPECT_EQ(DFLT_QOS, c_struct.qos);
	EXPECT_EQ(DFLT_RETAINED, c_struct.retained != 0);
	EXPECT_EQ(DFLT_DUP, c_struct.dup != 0);
}

// ----------------------------------------------------------------------
// Test the raw buffer (void*) constructor
// ----------------------------------------------------------------------

TEST_F(message_test, test_buf_constructor) {
	mqtt::message msg(TOPIC, BUF, N, QOS, true);

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());
	EXPECT_EQ(DFLT_DUP, msg.is_duplicate());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(int(N), c_struct.payloadlen);
	EXPECT_EQ(0, memcmp(BUF, c_struct.payload, N));
	EXPECT_EQ(QOS, c_struct.qos);
	EXPECT_NE(0, c_struct.retained);
	EXPECT_EQ(DFLT_DUP, c_struct.dup);
}

// ----------------------------------------------------------------------
// Test the string buffer constructor
// ----------------------------------------------------------------------

TEST_F(message_test, test_string_constructor) {
	mqtt::message msg(TOPIC, PAYLOAD);

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(DFLT_QOS, msg.get_qos());
	EXPECT_FALSE(msg.is_retained());
	EXPECT_EQ(DFLT_DUP, msg.is_duplicate());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(int(PAYLOAD.size()), c_struct.payloadlen);
	EXPECT_EQ(0, memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
	EXPECT_EQ(DFLT_QOS, c_struct.qos);
	EXPECT_EQ(DFLT_RETAINED, c_struct.retained != 0);
	EXPECT_EQ(DFLT_DUP, c_struct.dup != 0);
}

// ----------------------------------------------------------------------
// Test the string buffer with QoS constructor
// ----------------------------------------------------------------------

TEST_F(message_test, test_string_qos_constructor) {
	mqtt::message msg(TOPIC, PAYLOAD, QOS, true);

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());
	EXPECT_EQ(DFLT_DUP, msg.is_duplicate());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(int(PAYLOAD.size()), c_struct.payloadlen);
	EXPECT_EQ(0, memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
	EXPECT_EQ(QOS, c_struct.qos);
	EXPECT_NE(0, c_struct.retained);
	EXPECT_EQ(DFLT_DUP, c_struct.dup != 0);
}

// ----------------------------------------------------------------------
// Test the initialization by C struct
// ----------------------------------------------------------------------

TEST_F(message_test, test_c_struct_constructor) {
	MQTTAsync_message c_msg = MQTTAsync_message_initializer;

	c_msg.payload = const_cast<char*>(BUF);
	c_msg.payloadlen = N;
	c_msg.qos = QOS;
	c_msg.retained = 1;
	c_msg.dup = 1;

	mqtt::message msg(TOPIC, c_msg);

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());
	EXPECT_TRUE(msg.is_duplicate());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(int(N), c_struct.payloadlen);
	EXPECT_EQ(0, memcmp(BUF, c_struct.payload, N));
	EXPECT_EQ(QOS, c_struct.qos);
	EXPECT_NE(0, c_struct.retained);
	EXPECT_NE(0, c_struct.dup);
}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

TEST_F(message_test, test_copy_constructor) {
	mqtt::message msg(orgMsg);

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(int(PAYLOAD.size()), c_struct.payloadlen);
	EXPECT_EQ(0, memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
	EXPECT_EQ(QOS, c_struct.qos);
	EXPECT_NE(0, c_struct.retained);
	EXPECT_EQ(DFLT_DUP, c_struct.dup != 0);

	// Make sure it's a true copy, not linked to the original
	orgMsg.set_payload(EMPTY_STR);
	orgMsg.set_qos(DFLT_QOS);
	orgMsg.set_retained(false);

	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());
}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

TEST_F(message_test, test_move_constructor) {
	mqtt::message msg(std::move(orgMsg));

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(int(PAYLOAD.size()), c_struct.payloadlen);
	EXPECT_EQ(0, memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
	EXPECT_EQ(QOS, c_struct.qos);
	EXPECT_NE(0, c_struct.retained);
	EXPECT_EQ(DFLT_DUP, c_struct.dup != 0);

	// Check that the original was moved
	EXPECT_EQ(size_t(0), orgMsg.get_payload().size());
}

// ----------------------------------------------------------------------
// Test the copy assignment operator=(const&)
// ----------------------------------------------------------------------

TEST_F(message_test, test_copy_assignment) {
	mqtt::message msg;

	msg = orgMsg;

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(int(PAYLOAD.size()), c_struct.payloadlen);
	EXPECT_EQ(0, memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
	EXPECT_EQ(QOS, c_struct.qos);
	EXPECT_NE(0, c_struct.retained);
	EXPECT_EQ(DFLT_DUP, c_struct.dup != 0);

	// Make sure it's a true copy, not linked to the original
	orgMsg.set_payload(EMPTY_STR);
	orgMsg.set_qos(DFLT_QOS);
	orgMsg.set_retained(false);

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());

	// Self assignment should cause no harm
	msg = msg;

	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());
}

// ----------------------------------------------------------------------
// Test the move assignment, operator=(&&)
// ----------------------------------------------------------------------

TEST_F(message_test, test_move_assignment) {
	mqtt::message msg;
	msg = std::move(orgMsg);

	EXPECT_EQ(TOPIC, msg.get_topic());
	EXPECT_EQ(PAYLOAD, msg.get_payload_str());
	EXPECT_EQ(QOS, msg.get_qos());
	EXPECT_TRUE(msg.is_retained());

	const auto& c_struct = get_c_struct(msg);

	EXPECT_EQ(int(PAYLOAD.size()), c_struct.payloadlen);
	EXPECT_EQ(0, memcmp(PAYLOAD.data(), c_struct.payload, PAYLOAD.size()));
	EXPECT_EQ(QOS, c_struct.qos);
	EXPECT_NE(0, c_struct.retained);
	EXPECT_EQ(DFLT_DUP, c_struct.dup != 0);

	// Check that the original was moved
	EXPECT_EQ(size_t(0), orgMsg.get_payload().size());

	// Self assignment should cause no harm
	// (clang++ is smart enough to warn about this)
	#if !defined(__clang__)
		msg = std::move(msg);
		EXPECT_EQ(PAYLOAD, msg.get_payload_str());
		EXPECT_EQ(QOS, msg.get_qos());
		EXPECT_TRUE(msg.is_retained());
	#endif
}

// ----------------------------------------------------------------------
// Test the validate_qos()
// ----------------------------------------------------------------------

TEST_F(message_test, test_validate_qos) {
	ASSERT_THROW(mqtt::message::validate_qos(-1), mqtt::exception);

	for (int i=0; i<=2; ++i) {
		ASSERT_NO_THROW(mqtt::message::validate_qos(0));
	}

	ASSERT_THROW(mqtt::message::validate_qos(3), mqtt::exception);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_message_test_h

