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
 *    Guilherme M. Ferreira - changed test framework from CppUnit to GTest
 *******************************************************************************/

#ifndef __mqtt_topic_test_h
#define __mqtt_topic_test_h

#include <algorithm>
#include <memory>
#include <stdexcept>

#include <gtest/gtest.h>

#include "mqtt/topic.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class topic_test : public ::testing::Test
{
protected:
	const int DFLT_QOS = message::DFLT_QOS;
	const bool DFLT_RETAINED = message::DFLT_RETAINED;

	const std::string TOPIC { "topic_name" };
	const int QOS = 1;
	const bool RETAINED = true;

	const int BAD_LOW_QOS  = -1;
	const int BAD_HIGH_QOS =  3;

	const char* BUF = "Hello there";
	const size_t N = std::strlen(BUF);
	const binary PAYLOAD { BUF };

	mqtt::test::dummy_async_client cli;

public:
	void SetUp() {}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test basic constructor
// ----------------------------------------------------------------------

TEST_F(topic_test, test_basic_ctor) {
	mqtt::topic topic{ cli, TOPIC };

	EXPECT_EQ(static_cast<iasync_client*>(&cli),
			&(topic.get_client()));
	EXPECT_EQ(TOPIC, topic.get_name());
	EXPECT_EQ(DFLT_QOS, topic.get_qos());
	EXPECT_EQ(DFLT_RETAINED, topic.get_retained());
}

// ----------------------------------------------------------------------
// Test full constructor
// ----------------------------------------------------------------------

TEST_F(topic_test, test_full_ctor) {
	mqtt::topic topic{ cli, TOPIC, QOS, RETAINED };

	EXPECT_EQ(TOPIC, topic.get_name());
	EXPECT_EQ(QOS, topic.get_qos());
	EXPECT_EQ(RETAINED, topic.get_retained());
}

// ----------------------------------------------------------------------
// Test set qos
// ----------------------------------------------------------------------

TEST_F(topic_test, test_set_qos) {
	mqtt::topic topic{ cli, TOPIC };

	EXPECT_EQ(DFLT_QOS, topic.get_qos());
	topic.set_qos(QOS);
	EXPECT_EQ(QOS, topic.get_qos());

	EXPECT_THROW(topic.set_qos(BAD_LOW_QOS), mqtt::exception);

	EXPECT_THROW(topic.set_qos(BAD_HIGH_QOS), mqtt::exception);
}

// ----------------------------------------------------------------------
// Test set retained
// ----------------------------------------------------------------------

TEST_F(topic_test, test_set_retained) {
	mqtt::topic topic{ cli, TOPIC };

	EXPECT_EQ(DFLT_RETAINED, topic.get_retained());
	topic.set_retained(RETAINED);
	EXPECT_EQ(RETAINED, topic.get_retained());
}

// ----------------------------------------------------------------------
// Test publish with the basic C array form
// ----------------------------------------------------------------------

TEST_F(topic_test, test_publish_basic_c_arr) {
	mqtt::topic topic{ cli, TOPIC, QOS, RETAINED };

	auto tok = topic.publish(BUF, N);

	EXPECT_NE(nullptr, tok);

	auto msg = tok->get_message();

	EXPECT_NE(nullptr, msg);
	EXPECT_EQ(TOPIC, msg->get_topic());
	EXPECT_NE(nullptr, msg->get_payload().data());
	EXPECT_EQ(0, memcmp(BUF, msg->get_payload().data(), N));
	EXPECT_EQ(QOS, msg->get_qos());
	EXPECT_EQ(RETAINED, msg->is_retained());
}

// ----------------------------------------------------------------------
// Test publish with the full C array form
// ----------------------------------------------------------------------

TEST_F(topic_test, test_publish_full_c_arr) {
	mqtt::topic topic{ cli, TOPIC };

	auto tok = topic.publish(BUF, N, QOS, RETAINED);

	EXPECT_NE(nullptr, tok);

	auto msg = tok->get_message();

	EXPECT_NE(nullptr, msg);
	EXPECT_EQ(TOPIC, msg->get_topic());
	EXPECT_NE(nullptr, msg->get_payload().data());
	EXPECT_EQ(0, memcmp(BUF, msg->get_payload().data(), N));
	EXPECT_EQ(QOS, msg->get_qos());
	EXPECT_EQ(RETAINED, msg->is_retained());
}

// ----------------------------------------------------------------------
// Test publish with the basic C array form
// ----------------------------------------------------------------------

TEST_F(topic_test, test_publish_basic_binary) {
	mqtt::topic topic{ cli, TOPIC, QOS, RETAINED };

	auto tok = topic.publish(PAYLOAD);

	EXPECT_NE(nullptr, tok);

	auto msg = tok->get_message();

	EXPECT_NE(nullptr, msg);
	EXPECT_EQ(TOPIC, msg->get_topic());
	EXPECT_EQ(PAYLOAD, msg->get_payload());
	EXPECT_EQ(QOS, msg->get_qos());
	EXPECT_EQ(RETAINED, msg->is_retained());
}

// ----------------------------------------------------------------------
// Test publish with the full C array form
// ----------------------------------------------------------------------

TEST_F(topic_test, test_publish_full_binary) {
	mqtt::topic topic{ cli, TOPIC };

	auto tok = topic.publish(PAYLOAD, QOS, RETAINED);

	EXPECT_NE(nullptr, tok);

	auto msg = tok->get_message();

	EXPECT_NE(nullptr, msg);
	EXPECT_EQ(TOPIC, msg->get_topic());
	EXPECT_EQ(PAYLOAD, msg->get_payload());
	EXPECT_EQ(QOS, msg->get_qos());
	EXPECT_EQ(RETAINED, msg->is_retained());
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_topic_test_h
