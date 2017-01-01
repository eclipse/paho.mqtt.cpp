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

/////////////////////////////////////////////////////////////////////////////

class message_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( message_test );

	CPPUNIT_TEST( test_dflt_constructor );
	CPPUNIT_TEST( test_buf_constructor  );
	CPPUNIT_TEST( test_copy_constructor );
	CPPUNIT_TEST( test_move_constructor );
	CPPUNIT_TEST( test_copy_assignment  );
	CPPUNIT_TEST( test_move_assignment  );

	CPPUNIT_TEST_SUITE_END();

	const std::string EMPTY_STR;
	const int DFLT_QOS = 0;

	const char* BUF = "Hello there";
	const size_t N = std::strlen(BUF);
	const std::string PAYLOAD = std::string(BUF);
	const int QOS = 1;

	mqtt::message msg_;

public:
	void setUp() {
		msg_ = mqtt::message(PAYLOAD, QOS, true);
	}
	void tearDown() {}

	// ----------------------------------------------------------------------

	// Test the default constructor
	void test_dflt_constructor() {
		mqtt::message msg;
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, msg.get_payload());
	}

	// Test the raw buffer (void*) constructor
	void test_buf_constructor() {
		mqtt::message msg(BUF, N, QOS, true);

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
	}

	void test_string_constructor() {
		mqtt::message msg(PAYLOAD, QOS, true);

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
	}

	// Test the copy constructor
	void test_copy_constructor() {
		mqtt::message msg(msg_);

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		// Make sure it's a true copy, not linked to the original
		msg_.set_payload("");
		msg_.set_qos(0);
		msg_.set_retained(false);

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
	}

	// Test the move constructor
	void test_move_constructor() {
		mqtt::message msg(std::move(msg_));

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, msg_.get_payload());
		CPPUNIT_ASSERT_EQUAL(0, msg_.get_qos());
		CPPUNIT_ASSERT(!msg_.is_retained());
	}

	// Test the copy assignment operator=(const&)
	void test_copy_assignment() {
		mqtt::message msg;

		msg = msg_;

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		// Make sure it's a true copy, not linked to the original
		msg_.set_payload("");
		msg_.set_qos(0);
		msg_.set_retained(false);

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		// Self assignment should cause no harm
		msg = msg;

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
	}

	// Test the move assignment, operator=(&&)
	void test_move_assignment() {
		mqtt::message msg;

		msg = std::move(msg_);

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, msg_.get_payload());
		CPPUNIT_ASSERT_EQUAL(0, msg_.get_qos());
		CPPUNIT_ASSERT(!msg_.is_retained());

		// Self assignment should cause no harm
		msg = std::move(msg);

		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg.get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg.get_qos());
		CPPUNIT_ASSERT(msg.is_retained());
	}
};

#endif		//  __mqtt_message_test_h


