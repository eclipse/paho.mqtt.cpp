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

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/will_options.h"
#include <cstring>

/////////////////////////////////////////////////////////////////////////////

class will_options_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( will_options_test );

	CPPUNIT_TEST( test_dflt_constructor );
	CPPUNIT_TEST( test_buf_constructor  );
	CPPUNIT_TEST( test_copy_constructor );
	CPPUNIT_TEST( test_move_constructor );
	CPPUNIT_TEST( test_copy_assignment  );
	CPPUNIT_TEST( test_move_assignment  );

	CPPUNIT_TEST_SUITE_END();

	const std::string EMPTY_STR;
	const int DFLT_QOS = 0;

	const std::string TOPIC = "hello";
	const char* BUF = "Hello there";
	const size_t N = std::strlen(BUF);
	const std::string PAYLOAD = std::string(BUF);
	const int QOS = 1;

	mqtt::will_options opts_;
	mqtt::message msg_;


public:
	void setUp() {
		msg_ = mqtt::message(PAYLOAD, QOS, true);
		opts_ = mqtt::will_options(TOPIC, msg_);
	}
	void tearDown() {}

	// ----------------------------------------------------------------------

	// Test the default constructor
	void test_dflt_constructor() {
		mqtt::will_options opts;
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, opts.get_qos());
		CPPUNIT_ASSERT(!opts.is_retained());
	}

	// Test the raw buffer (void*) constructor
	void test_buf_constructor() {
		mqtt::will_options opts(TOPIC, BUF, N, QOS, true);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT(opts.is_retained());
	}

	// Test the copy constructor
	void test_copy_constructor() {
		mqtt::will_options orgOpts(TOPIC, BUF, N, QOS, true);
		mqtt::will_options opts(orgOpts);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT(opts.is_retained());

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_topic("");
		orgOpts.set_payload("");
		orgOpts.set_qos(0);
		orgOpts.set_retained(false);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT(opts.is_retained());
	}

	// Test the move constructor
	void test_move_constructor() {
		mqtt::will_options orgOpts(TOPIC, msg_);
		mqtt::will_options opts(std::move(orgOpts));

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT(opts.is_retained());

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_topic());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_payload());
		CPPUNIT_ASSERT_EQUAL(0, orgOpts.get_qos());
		CPPUNIT_ASSERT(!orgOpts.is_retained());
	}

	// Test the copy assignment operator=(const&)
	void test_copy_assignment() {
		mqtt::will_options opts;

		opts = opts_;

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT(opts.is_retained());

		// Make sure it's a true copy, not linked to the original
		opts_.set_topic("");
		opts_.set_payload("");
		opts_.set_qos(0);
		opts_.set_retained(false);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT(opts.is_retained());

		// Self assignment should cause no harm
		opts = opts;

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT(opts.is_retained());
	}

	// Test the move assignment, operator=(&&)
	void test_move_assignment() {
		mqtt::will_options orgOpts(TOPIC, msg_);
		mqtt::will_options opts;

		opts = std::move(orgOpts);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT(opts.is_retained());

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_topic());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_payload());
		CPPUNIT_ASSERT_EQUAL(0, orgOpts.get_qos());
		CPPUNIT_ASSERT(!orgOpts.is_retained());

		// Self assignment should cause no harm
		opts = std::move(opts);

		CPPUNIT_ASSERT_EQUAL(TOPIC, opts.get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, opts.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, opts.get_qos());
		CPPUNIT_ASSERT(opts.is_retained());
	}
};

#endif		//  __mqtt_will_options_test_h


