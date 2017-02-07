// delivery_response_options_test.h
// Unit tests for the delivery_response_options class in the Paho MQTT C++ library.

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

#ifndef __mqtt_delivery_response_options_test_h
#define __mqtt_delivery_response_options_test_h

#include <gtest/gtest.h>

#include "mqtt/response_options.h"

#include "dummy_async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class delivery_response_options_test : public ::testing::Test
{
protected:
	MQTTAsync_responseOptions& get_c_struct(mqtt::delivery_response_options& opts) {
		return opts.opts_;
	}

public:

	void SetUp() {}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test default constructor
// ----------------------------------------------------------------------

TEST_F(delivery_response_options_test, test_dflt_constructor) {
	mqtt::delivery_response_options opts;
	MQTTAsync_responseOptions& c_struct = get_c_struct(opts);

	EXPECT_EQ(c_struct.context, nullptr);

	// Make sure the callback functions are set during object construction
	EXPECT_NE(c_struct.onSuccess, nullptr);
	EXPECT_NE(c_struct.onFailure, nullptr);
}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

TEST_F(delivery_response_options_test, test_user_constructor) {
	mqtt::test::dummy_async_client client;
	mqtt::delivery_token_ptr token { new mqtt::delivery_token{ client } };
	mqtt::delivery_response_options opts { token };
	MQTTAsync_responseOptions& c_struct = get_c_struct(opts);

	EXPECT_EQ(c_struct.context, token.get());

	// Make sure the callback functions are set during object construction
	EXPECT_NE(c_struct.onSuccess, nullptr);
	EXPECT_NE(c_struct.onFailure, nullptr);
}

// ----------------------------------------------------------------------
// Test set context
// ----------------------------------------------------------------------

TEST_F(delivery_response_options_test, test_set_token) {
	mqtt::delivery_response_options opts;
	MQTTAsync_responseOptions& c_struct = get_c_struct(opts);

	EXPECT_EQ(c_struct.context, nullptr);
	mqtt::test::dummy_async_client client;
	mqtt::delivery_token_ptr token { new mqtt::delivery_token{ client } };
	opts.set_token( token );
	EXPECT_EQ(c_struct.context, token.get());
}

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif //  __mqtt_delivery_response_options_test_h
