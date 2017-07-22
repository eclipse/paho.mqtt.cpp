// exception_test.h
// Unit tests for the exception class in the Paho MQTT C++ library.

/*******************************************************************************
 * Copyright (c) 2016-2017 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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

#ifndef __mqtt_exception_test_h
#define __mqtt_exception_test_h

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/exception.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class exception_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( exception_test );

	CPPUNIT_TEST( test_user_constructor );
	CPPUNIT_TEST( test_get_message );
	CPPUNIT_TEST( test_get_reason_code );
	CPPUNIT_TEST( test_to_str );
	CPPUNIT_TEST( test_what );

	CPPUNIT_TEST_SUITE_END();

	const std::string msg { "Some MQTT error" };

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

	void test_user_constructor() {
		mqtt::exception ex1(MQTTASYNC_FAILURE);
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex1.get_reason_code());

		mqtt::exception ex2(MQTTASYNC_PERSISTENCE_ERROR);
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_PERSISTENCE_ERROR, ex2.get_reason_code());

		mqtt::exception ex3(MQTTASYNC_OPERATION_INCOMPLETE);
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_OPERATION_INCOMPLETE, ex3.get_reason_code());
	}

// ----------------------------------------------------------------------
// Test get_message()
// ----------------------------------------------------------------------

	void test_get_message() {
		mqtt::exception ex(MQTTASYNC_FAILURE, msg);
		CPPUNIT_ASSERT_EQUAL(msg, ex.get_message());
	}

// ----------------------------------------------------------------------
// Test get_reason_code()
// ----------------------------------------------------------------------

	void test_get_reason_code() {
		mqtt::exception ex1(MQTTASYNC_FAILURE);
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex1.get_reason_code());
	}

// ----------------------------------------------------------------------
// Test to string
// ----------------------------------------------------------------------

	void test_to_str() {
		mqtt::exception ex1(MQTTASYNC_FAILURE);
		std::string msg1 { "MQTT error [-1]" };
		CPPUNIT_ASSERT_EQUAL(msg1, ex1.to_string());
	}

// ----------------------------------------------------------------------
// Test get name
// ----------------------------------------------------------------------

	void test_what() {
		mqtt::exception ex1(MQTTASYNC_FAILURE);
		const char *msg1 = "MQTT error [-1]";
		CPPUNIT_ASSERT(!strcmp(msg1, ex1.what()));
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_exception_test_h
