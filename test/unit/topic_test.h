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

	CPPUNIT_TEST( test_basic_ctor );
	CPPUNIT_TEST( test_full_ctor );
	CPPUNIT_TEST( test_set_qos );
	CPPUNIT_TEST( test_set_retained );
	CPPUNIT_TEST( test_publish_basic_c_arr );
	CPPUNIT_TEST( test_publish_full_c_arr );
	CPPUNIT_TEST( test_publish_basic_binary );
	CPPUNIT_TEST( test_publish_basic_binary );

	CPPUNIT_TEST_SUITE_END();

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
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test basic constructor
// ----------------------------------------------------------------------

	void test_basic_ctor() {
		mqtt::topic topic{ cli, TOPIC };

		CPPUNIT_ASSERT_EQUAL(static_cast<iasync_client*>(&cli),
							 &(topic.get_client()));
		CPPUNIT_ASSERT_EQUAL(TOPIC, topic.get_name());
		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, topic.get_qos());
		CPPUNIT_ASSERT_EQUAL(DFLT_RETAINED, topic.get_retained());
	}

// ----------------------------------------------------------------------
// Test full constructor
// ----------------------------------------------------------------------

	void test_full_ctor() {
		mqtt::topic topic{ cli, TOPIC, QOS, RETAINED };

		CPPUNIT_ASSERT_EQUAL(TOPIC, topic.get_name());
		CPPUNIT_ASSERT_EQUAL(QOS, topic.get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, topic.get_retained());
	}

// ----------------------------------------------------------------------
// Test set qos
// ----------------------------------------------------------------------

	void test_set_qos() {
		mqtt::topic topic{ cli, TOPIC };

		CPPUNIT_ASSERT_EQUAL(DFLT_QOS, topic.get_qos());
		topic.set_qos(QOS);
		CPPUNIT_ASSERT_EQUAL(QOS, topic.get_qos());

		try {
			topic.set_qos(BAD_LOW_QOS);
			CPPUNIT_FAIL("topic should not accept bad (low) QOS");
		}
		catch (...) {}

		try {
			topic.set_qos(BAD_HIGH_QOS);
			CPPUNIT_FAIL("topic should not accept bad (low) QOS");
		}
		catch (...) {}
	}

// ----------------------------------------------------------------------
// Test set retained
// ----------------------------------------------------------------------

	void test_set_retained() {
		mqtt::topic topic{ cli, TOPIC };

		CPPUNIT_ASSERT_EQUAL(DFLT_RETAINED, topic.get_retained());
		topic.set_retained(RETAINED);
		CPPUNIT_ASSERT_EQUAL(RETAINED, topic.get_retained());
	}

// ----------------------------------------------------------------------
// Test publish with the basic C array form
// ----------------------------------------------------------------------

	void test_publish_basic_c_arr() {
		mqtt::topic topic{ cli, TOPIC, QOS, RETAINED };

		auto tok = topic.publish(BUF, N);

		CPPUNIT_ASSERT(tok);

		auto msg = tok->get_message();

		CPPUNIT_ASSERT(msg);
		CPPUNIT_ASSERT_EQUAL(TOPIC, msg->get_topic());
		CPPUNIT_ASSERT(msg->get_payload().data());
		CPPUNIT_ASSERT(!memcmp(BUF, msg->get_payload().data(), N));
		CPPUNIT_ASSERT_EQUAL(QOS, msg->get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, msg->is_retained());
	}

// ----------------------------------------------------------------------
// Test publish with the full C array form
// ----------------------------------------------------------------------

	void test_publish_full_c_arr() {
		mqtt::topic topic{ cli, TOPIC };

		auto tok = topic.publish(BUF, N, QOS, RETAINED);

		CPPUNIT_ASSERT(tok);

		auto msg = tok->get_message();

		CPPUNIT_ASSERT(msg);
		CPPUNIT_ASSERT_EQUAL(TOPIC, msg->get_topic());
		CPPUNIT_ASSERT(msg->get_payload().data());
		CPPUNIT_ASSERT(!memcmp(BUF, msg->get_payload().data(), N));
		CPPUNIT_ASSERT_EQUAL(QOS, msg->get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, msg->is_retained());
	}

// ----------------------------------------------------------------------
// Test publish with the basic C array form
// ----------------------------------------------------------------------

	void test_publish_basic_binary() {
		mqtt::topic topic{ cli, TOPIC, QOS, RETAINED };

		auto tok = topic.publish(PAYLOAD);

		CPPUNIT_ASSERT(tok);

		auto msg = tok->get_message();

		CPPUNIT_ASSERT(msg);
		CPPUNIT_ASSERT_EQUAL(TOPIC, msg->get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg->get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg->get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, msg->is_retained());
	}

// ----------------------------------------------------------------------
// Test publish with the full C array form
// ----------------------------------------------------------------------

	void test_publish_full_binary() {
		mqtt::topic topic{ cli, TOPIC };

		auto tok = topic.publish(PAYLOAD, QOS, RETAINED);

		CPPUNIT_ASSERT(tok);

		auto msg = tok->get_message();

		CPPUNIT_ASSERT(msg);
		CPPUNIT_ASSERT_EQUAL(TOPIC, msg->get_topic());
		CPPUNIT_ASSERT_EQUAL(PAYLOAD, msg->get_payload());
		CPPUNIT_ASSERT_EQUAL(QOS, msg->get_qos());
		CPPUNIT_ASSERT_EQUAL(RETAINED, msg->is_retained());
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_topic_test_h
