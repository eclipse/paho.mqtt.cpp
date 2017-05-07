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
 *******************************************************************************/

#ifndef __mqtt_will_options_test_h
#define __mqtt_will_options_test_h

#include "mqtt/will_options.h"
#include "dummy_async_client.h"
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

// Note that at this time, the LWT payload has been converted to binary
// from the previous version's use of a text string (NUL-terminated C str).
// We now fill in the 'payload' fields of the underlying C struct,
// MQTTAsync_willOptions. The 'message' field _must_ stay NULL for the C lib
// to use 'payload'.

class will_options_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( will_options_test );

	CPPUNIT_TEST( test_dflt_constructor );
	CPPUNIT_TEST( test_string_buf_constructor  );
	CPPUNIT_TEST( test_topic_buf_constructor  );
	CPPUNIT_TEST( test_string_string_constructor  );
	CPPUNIT_TEST( test_string_message_constructor  );
	CPPUNIT_TEST( test_copy_constructor );
	CPPUNIT_TEST( test_move_constructor );
	CPPUNIT_TEST( test_copy_assignment  );
	CPPUNIT_TEST( test_move_assignment  );
	CPPUNIT_TEST( test_set_topic_str  );
	CPPUNIT_TEST( test_set_payload  );

	CPPUNIT_TEST_SUITE_END();

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

public:
	void setUp() {
		orgOpts = mqtt::will_options(TOPIC, BUF, N, QOS, RETAINED);
	}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

	void test_dflt_constructor() {
		mqtt::will_options opts;

		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(DFLT_RETAINED, opts.is_retained());

		// Test the C struct
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
		CPPUNIT_ASSERT(c_struct.topicName != nullptr);
		CPPUNIT_ASSERT_EQUAL(size_t(0), strlen(c_struct.topicName));
		CPPUNIT_ASSERT(c_struct.message == nullptr);
		CPPUNIT_ASSERT_EQUAL(0, c_struct.payload.len);
		CPPUNIT_ASSERT(c_struct.payload.data == nullptr);
	}

// ----------------------------------------------------------------------
// Test the raw buffer (void*) constructor
// ----------------------------------------------------------------------

	void test_string_buf_constructor() {
		test::dummy_async_client cli;
		mqtt::topic topic { cli, TOPIC };

		mqtt::will_options opts(topic, BUF, N, QOS, true);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());

		// Test the C struct
		// Remember we now fill payload fields, not message
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
		CPPUNIT_ASSERT(!strcmp(c_struct.topicName, TOPIC.c_str()));
		CPPUNIT_ASSERT(c_struct.message == nullptr);
		CPPUNIT_ASSERT_EQUAL(N, size_t(c_struct.payload.len));
		CPPUNIT_ASSERT(!memcmp(BUF, c_struct.payload.data, N));
	}

// ----------------------------------------------------------------------
// Test the raw buffer (void*) constructor
// ----------------------------------------------------------------------

	void test_topic_buf_constructor() {
		mqtt::will_options opts(TOPIC, BUF, N, QOS, true);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());

		// Test the C struct
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
		CPPUNIT_ASSERT(!strcmp(c_struct.topicName, TOPIC.c_str()));
		CPPUNIT_ASSERT(c_struct.message == nullptr);
		CPPUNIT_ASSERT_EQUAL(N, size_t(c_struct.payload.len));
		CPPUNIT_ASSERT(!memcmp(BUF, c_struct.payload.data, N));
	}

// ----------------------------------------------------------------------
// Test the string payload constructor
// ----------------------------------------------------------------------

	void test_string_string_constructor() {
		mqtt::will_options opts(TOPIC, PAYLOAD, QOS, true);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());

		// Test the C struct
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
		CPPUNIT_ASSERT(!strcmp(c_struct.topicName, TOPIC.c_str()));
		CPPUNIT_ASSERT(c_struct.message == nullptr);
		CPPUNIT_ASSERT_EQUAL(PAYLOAD.size(), size_t(c_struct.payload.len));
		CPPUNIT_ASSERT(!memcmp(PAYLOAD.data(), c_struct.payload.data, PAYLOAD.size()));
	}

// ----------------------------------------------------------------------
// Test the message payload constructor
// ----------------------------------------------------------------------

	void test_string_message_constructor() {
		mqtt::message msg(TOPIC, PAYLOAD, QOS, true);
		mqtt::will_options opts(msg);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());

		// Test the C struct
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
		CPPUNIT_ASSERT(!strcmp(c_struct.topicName, TOPIC.c_str()));
		CPPUNIT_ASSERT(c_struct.message == nullptr);
		CPPUNIT_ASSERT_EQUAL(PAYLOAD.size(), size_t(c_struct.payload.len));
		CPPUNIT_ASSERT(!memcmp(PAYLOAD.data(), c_struct.payload.data, PAYLOAD.size()));
	}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

	void test_copy_constructor() {
		mqtt::will_options opts(orgOpts);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());

		// Check the C struct
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
		CPPUNIT_ASSERT(!strcmp(c_struct.topicName, TOPIC.c_str()));
		CPPUNIT_ASSERT(c_struct.message == nullptr);
		CPPUNIT_ASSERT_EQUAL(N, size_t(c_struct.payload.len));
		CPPUNIT_ASSERT(!memcmp(BUF, c_struct.payload.data, N));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_topic(EMPTY_STR);
		orgOpts.set_payload(EMPTY_STR);
		orgOpts.set_qos(DFLT_QOS);
		orgOpts.set_retained(false);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());
	}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

	void test_move_constructor() {
		mqtt::will_options opts(std::move(orgOpts));

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());

		// Check the C struct
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
		CPPUNIT_ASSERT(!strcmp(c_struct.topicName, TOPIC.c_str()));
		CPPUNIT_ASSERT(c_struct.message == nullptr);
		CPPUNIT_ASSERT_EQUAL(N, size_t(c_struct.payload.len));
		CPPUNIT_ASSERT(!memcmp(BUF, c_struct.payload.data, N));

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_topic());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_payload_str());
	}

// ----------------------------------------------------------------------
// Test the copy assignment operator=(const&)
// ----------------------------------------------------------------------

	void test_copy_assignment() {
		mqtt::will_options opts;

		opts = orgOpts;

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());

		// Check the C struct
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
		CPPUNIT_ASSERT(!strcmp(c_struct.topicName, TOPIC.c_str()));
		CPPUNIT_ASSERT(c_struct.message == nullptr);
		CPPUNIT_ASSERT_EQUAL(N, size_t(c_struct.payload.len));
		CPPUNIT_ASSERT(!memcmp(BUF, c_struct.payload.data, N));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_topic(EMPTY_STR);
		orgOpts.set_payload(EMPTY_STR);
		orgOpts.set_qos(DFLT_QOS);
		orgOpts.set_retained(false);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());

		// Self assignment should cause no harm
		opts = opts;

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());
	}

// ----------------------------------------------------------------------
// Test the move assignment, operator=(&&)
// ----------------------------------------------------------------------

	void test_move_assignment() {
		mqtt::will_options opts;

		opts = std::move(orgOpts);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());

		// Check the C struct
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
		CPPUNIT_ASSERT(!strcmp(c_struct.topicName, TOPIC.c_str()));
		CPPUNIT_ASSERT(c_struct.message == nullptr);
		CPPUNIT_ASSERT_EQUAL(N, size_t(c_struct.payload.len));
		CPPUNIT_ASSERT(!memcmp(BUF, c_struct.payload.data, N));

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_topic());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_payload_str());

		// Self assignment should cause no harm
		// (clang++ is smart enough to warn about this)
		#if !defined(__clang__)
			opts = std::move(opts);
			CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
			CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());
			CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
			CPPUNIT_ASSERT_EQUAL(RETAINED, opts.is_retained());
		#endif
	}

// ----------------------------------------------------------------------
// Test setting the (text) topic
// ----------------------------------------------------------------------

	void test_set_topic_str() {
		mqtt::will_options opts;

		opts.set_topic(TOPIC);
		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());

		const auto& c_struct = opts.opts_;
		CPPUNIT_ASSERT(!strcmp(c_struct.topicName, TOPIC.c_str()));

		// Setting empty string should _not_ create nullptr entry, in
		// C struct, rather a valid zero-length string.
		opts.set_topic(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_topic());
		CPPUNIT_ASSERT(opts.opts_.topicName != nullptr);
		CPPUNIT_ASSERT_EQUAL(size_t(0), strlen(opts.opts_.topicName));
	}

// ----------------------------------------------------------------------
// Test setting the (binary) payload
// ----------------------------------------------------------------------

	void test_set_payload() {
		mqtt::will_options opts;

		opts.set_payload(PAYLOAD);
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload_str());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT_EQUAL(PAYLOAD.size(), size_t(c_struct.payload.len));
		CPPUNIT_ASSERT(!memcmp(PAYLOAD.data(), c_struct.payload.data, PAYLOAD.size()));

		// Setting empty string set a valid, but zero-len payload
		// TODO: We need to check what the C lib now accepts.
		opts.set_payload(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_payload_str());
		CPPUNIT_ASSERT_EQUAL(size_t(0), opts.get_payload().size());

		CPPUNIT_ASSERT_EQUAL(0, opts.opts_.payload.len);
		CPPUNIT_ASSERT(opts.opts_.payload.data != nullptr);
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_will_options_test_h
