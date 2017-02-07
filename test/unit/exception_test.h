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
 *    Guilherme M. Ferreira - changed test framework from CppUnit to GTest
 *******************************************************************************/

#ifndef __mqtt_exception_test_h
#define __mqtt_exception_test_h

#include <gtest/gtest.h>

#include "mqtt/exception.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class exception_test : public ::testing::Test
{
public:
	void SetUp() {}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

TEST_F(exception_test, test_user_constructor) {
	mqtt::exception ex1(MQTTASYNC_FAILURE);
	EXPECT_EQ(MQTTASYNC_FAILURE, ex1.get_reason_code());

	mqtt::exception ex2(MQTTASYNC_PERSISTENCE_ERROR);
	EXPECT_EQ(MQTTASYNC_PERSISTENCE_ERROR, ex2.get_reason_code());

	mqtt::exception ex3(MQTTASYNC_OPERATION_INCOMPLETE);
	EXPECT_EQ(MQTTASYNC_OPERATION_INCOMPLETE, ex3.get_reason_code());
}

// ----------------------------------------------------------------------
// Test get_message()
// ----------------------------------------------------------------------

TEST_F(exception_test, test_get_message) {
	std::string msg1 { "MQTT exception -1" };
	mqtt::exception ex1(MQTTASYNC_FAILURE, msg1);
	EXPECT_EQ(msg1, ex1.get_message());
}

// ----------------------------------------------------------------------
// Test get_reason_code()
// ----------------------------------------------------------------------

TEST_F(exception_test, test_get_reason_code) {
	mqtt::exception ex1(MQTTASYNC_FAILURE);
	EXPECT_EQ(MQTTASYNC_FAILURE, ex1.get_reason_code());
}

// ----------------------------------------------------------------------
// Test to string
// ----------------------------------------------------------------------

TEST_F(exception_test, test_to_str) {
	mqtt::exception ex1(MQTTASYNC_FAILURE);
	std::string msg1 { "MQTT error [-1]" };

	EXPECT_EQ(msg1, ex1.to_string());
}

// ----------------------------------------------------------------------
// Test get name
// ----------------------------------------------------------------------

TEST_F(exception_test, test_what) {
	mqtt::exception ex1(MQTTASYNC_FAILURE);
	const char *msg1 = "MQTT error [-1]";

	EXPECT_STRCASEEQ(msg1, ex1.what());
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_exception_test_h
