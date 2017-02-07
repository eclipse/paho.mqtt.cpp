// will_options_test.h
// Unit tests for the will_options class in the Paho MQTT C++ library.

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

#ifndef __mqtt_will_options_test_h
#define __mqtt_will_options_test_h

#include <gtest/gtest.h>

#include "mqtt/will_options.h"
#include "dummy_async_client.h"

#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

// Note that at this time, the LWT payload has been converted to binary
// from the previous version's use of a text string (NUL-terminated C str).
// We now fill in the 'payload' fields of the underlying C struct,
// MQTTAsync_willOptions. The 'message' field _must_ stay NULL for the C lib
// to use 'payload'.

class will_options_test : public ::testing::Test
{
protected:

	const std::string EMPTY_STR;
	const int DFLT_QOS = will_options::DFLT_QOS;
	const bool DFLT_RETAINED = will_options::DFLT_RETAINED;

	// C struct signature/eyecatcher
	const char* CSIG = "MQTW";
	const size_t CSIG_LEN = std::strlen(CSIG);

	const std::string TOPIC = "hello";
	const char* BUF = "Hello there";
	const size_t N = std::strlen(BUF);
	const std::string PAYLOAD = std::string(BUF);
	const int QOS = 1;
	const bool RETAINED = true;

	mqtt::will_options orgOpts;

	MQTTAsync_willOptions& get_c_struct(mqtt::will_options& opts) {
		return opts.opts_;
	}

public:
	void SetUp() {
		orgOpts = mqtt::will_options(TOPIC, BUF, N, QOS, RETAINED);
	}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_dflt_constructor) {
	mqtt::will_options opts;

	EXPECT_EQ(EMPTY_STR, opts.get_topic());
	EXPECT_EQ(EMPTY_STR, opts.get_payload_str());
	EXPECT_EQ(DFLT_QOS, opts.get_qos());
	EXPECT_EQ(DFLT_RETAINED, opts.is_retained());

	// Test the C struct
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_NE(nullptr, c_struct.topicName);
	EXPECT_EQ(size_t(0), strlen(c_struct.topicName));
	EXPECT_EQ(nullptr, c_struct.message);
	EXPECT_EQ(0, c_struct.payload.len);
	EXPECT_EQ(nullptr, c_struct.payload.data);
}

// ----------------------------------------------------------------------
// Test the raw buffer (void*) constructor
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_string_buf_constructor) {
	test::dummy_async_client cli;
	mqtt::topic topic { cli, TOPIC };

	mqtt::will_options opts(topic, BUF, N, QOS, true);

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());

	// Test the C struct
	// Remember we now fill payload fields, not message
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TOPIC.c_str(), c_struct.topicName);
	EXPECT_EQ(nullptr, c_struct.message);
	EXPECT_EQ(N, size_t(c_struct.payload.len));
	EXPECT_EQ(0, memcmp(BUF, c_struct.payload.data, N));
}

// ----------------------------------------------------------------------
// Test the raw buffer (void*) constructor
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_topic_buf_constructor) {
	mqtt::will_options opts(TOPIC, BUF, N, QOS, true);

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());

	// Test the C struct
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TOPIC.c_str(), c_struct.topicName);
	EXPECT_EQ(nullptr, c_struct.message);
	EXPECT_EQ(N, size_t(c_struct.payload.len));
	EXPECT_EQ(0, memcmp(BUF, c_struct.payload.data, N));
}

// ----------------------------------------------------------------------
// Test the string payload constructor
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_string_string_constructor) {
	mqtt::will_options opts(TOPIC, PAYLOAD, QOS, true);

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());

	// Test the C struct
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TOPIC.c_str(), c_struct.topicName);
	EXPECT_EQ(nullptr, c_struct.message);
	EXPECT_EQ(PAYLOAD.size(), size_t(c_struct.payload.len));
	EXPECT_EQ(0, memcmp(PAYLOAD.data(), c_struct.payload.data, PAYLOAD.size()));
}

// ----------------------------------------------------------------------
// Test the message payload constructor
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_string_message_constructor) {
	mqtt::message msg(TOPIC, PAYLOAD, QOS, true);
	mqtt::will_options opts(msg);

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());

	// Test the C struct
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TOPIC.c_str(), c_struct.topicName);
	EXPECT_EQ(nullptr, c_struct.message);
	EXPECT_EQ(PAYLOAD.size(), size_t(c_struct.payload.len));
	EXPECT_EQ(0, memcmp(PAYLOAD.data(), c_struct.payload.data, PAYLOAD.size()));
}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_copy_constructor) {
	mqtt::will_options opts(orgOpts);

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());

	// Check the C struct
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TOPIC.c_str(), c_struct.topicName);
	EXPECT_EQ(nullptr, c_struct.message);
	EXPECT_EQ(N, size_t(c_struct.payload.len));
	EXPECT_EQ(0, memcmp(BUF, c_struct.payload.data, N));

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_topic(EMPTY_STR);
	orgOpts.set_payload(EMPTY_STR);
	orgOpts.set_qos(DFLT_QOS);
	orgOpts.set_retained(false);

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());
}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_move_constructor) {
	mqtt::will_options opts(std::move(orgOpts));

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());

	// Check the C struct
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TOPIC.c_str(), c_struct.topicName);
	EXPECT_EQ(nullptr, c_struct.message);
	EXPECT_EQ(N, size_t(c_struct.payload.len));
	EXPECT_EQ(0, memcmp(BUF, c_struct.payload.data, N));

	// Check that the original was moved
	EXPECT_EQ(EMPTY_STR, orgOpts.get_topic());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_payload_str());
}

// ----------------------------------------------------------------------
// Test the copy assignment operator=(const&)
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_copy_assignment) {
	mqtt::will_options opts;

	opts = orgOpts;

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());

	// Check the C struct
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TOPIC.c_str(), c_struct.topicName);
	EXPECT_EQ(nullptr, c_struct.message);
	EXPECT_EQ(N, size_t(c_struct.payload.len));
	EXPECT_EQ(0, memcmp(BUF, c_struct.payload.data, N));

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_topic(EMPTY_STR);
	orgOpts.set_payload(EMPTY_STR);
	orgOpts.set_qos(DFLT_QOS);
	orgOpts.set_retained(false);

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());

	// Self assignment should cause no harm
	opts = opts;

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());
}

// ----------------------------------------------------------------------
// Test the move assignment, operator=(&&)
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_move_assignment) {
	mqtt::will_options opts;

	opts = std::move(orgOpts);

	EXPECT_EQ(TOPIC, opts.get_topic());
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());
	EXPECT_EQ(QOS, opts.get_qos());
	EXPECT_EQ(RETAINED, opts.is_retained());

	// Check the C struct
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TOPIC.c_str(), c_struct.topicName);
	EXPECT_EQ(nullptr, c_struct.message);
	EXPECT_EQ(N, size_t(c_struct.payload.len));
	EXPECT_EQ(0, memcmp(BUF, c_struct.payload.data, N));

	// Check that the original was moved
	EXPECT_EQ(EMPTY_STR, orgOpts.get_topic());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_payload_str());

	// Self assignment should cause no harm
	// (clang++ is smart enough to warn about this)
	#if !defined(__clang__)
		opts = std::move(opts);
		EXPECT_EQ(TOPIC, opts.get_topic());
		EXPECT_EQ(PAYLOAD, opts.get_payload_str());
		EXPECT_EQ(QOS, opts.get_qos());
		EXPECT_EQ(RETAINED, opts.is_retained());
	#endif
}

// ----------------------------------------------------------------------
// Test setting the (text) topic
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_set_topic_str) {
	mqtt::will_options opts;

	opts.set_topic(TOPIC);
	EXPECT_EQ(TOPIC, opts.get_topic());

	const auto& c_struct = get_c_struct(opts);
	EXPECT_STRCASEEQ(TOPIC.c_str(), c_struct.topicName);

	// Setting empty string should _not_ create nullptr entry, in
	// C struct, rather a valid zero-length string.
	opts.set_topic(EMPTY_STR);

	EXPECT_EQ(EMPTY_STR, opts.get_topic());
	EXPECT_NE(nullptr, get_c_struct(opts).topicName);
	EXPECT_EQ(size_t(0), strlen(get_c_struct(opts).topicName));
}

// ----------------------------------------------------------------------
// Test setting the (binary) payload
// ----------------------------------------------------------------------

TEST_F(will_options_test, test_set_payload) {
	mqtt::will_options opts;

	opts.set_payload(PAYLOAD);
	EXPECT_EQ(PAYLOAD, opts.get_payload_str());

	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(PAYLOAD.size(), size_t(c_struct.payload.len));
	EXPECT_EQ(0, memcmp(PAYLOAD.data(), c_struct.payload.data, PAYLOAD.size()));

	// Setting empty string set a valid, but zero-len payload
	// TODO: We need to check what the C lib now accepts.
	opts.set_payload(EMPTY_STR);

	EXPECT_EQ(EMPTY_STR, opts.get_payload_str());
	EXPECT_EQ(size_t(0), opts.get_payload().size());

	EXPECT_EQ(0, get_c_struct(opts).payload.len);
	EXPECT_NE(nullptr, get_c_struct(opts).payload.data);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_will_options_test_h
