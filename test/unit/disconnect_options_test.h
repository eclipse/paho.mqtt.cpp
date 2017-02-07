// disconnect_options_test.h
// Unit tests for the disconnect_options class in the Paho MQTT C++ library.

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

#ifndef __mqtt_disconnect_options_test_h
#define __mqtt_disconnect_options_test_h

#include <gtest/gtest.h>

#include "mqtt/disconnect_options.h"

#include "dummy_async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class disconnect_options_test : public ::testing::Test
{
protected:
	const int DFLT_TIMEOUT = 0;

	const std::string EMPTY_STR;
	mqtt::test::dummy_async_client cli;

	MQTTAsync_disconnectOptions& get_c_struct(mqtt::disconnect_options& opts) {
		return opts.opts_;
	}

public:
	void SetUp() {}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test default constructor
// ----------------------------------------------------------------------

TEST_F(disconnect_options_test, test_dflt_constructor) {
	mqtt::disconnect_options opts;

	EXPECT_EQ(DFLT_TIMEOUT, (int) opts.get_timeout().count());
	EXPECT_EQ(nullptr, opts.get_token());

	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(nullptr, c_struct.onSuccess);
	EXPECT_EQ(nullptr, c_struct.onFailure);

	EXPECT_EQ(DFLT_TIMEOUT, c_struct.timeout);
}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

TEST_F(disconnect_options_test, test_user_constructor) {
	const int TIMEOUT = 10;

	auto tok = token::create(cli);
	mqtt::disconnect_options opts { TIMEOUT, tok };

	const auto& c_struct = get_c_struct(opts);

	EXPECT_NE(nullptr, c_struct.onSuccess);
	EXPECT_NE(nullptr, c_struct.onFailure);

	EXPECT_EQ(TIMEOUT, (int) opts.get_timeout().count());
	EXPECT_EQ(tok, opts.get_token());
}

// ----------------------------------------------------------------------
// Test set timeout
// ----------------------------------------------------------------------

TEST_F(disconnect_options_test, test_set_timeout) {
	mqtt::disconnect_options opts;
	const auto& c_struct = get_c_struct(opts);

	const int TIMEOUT = 5000;	// ms

	// Set with integer
	opts.set_timeout(TIMEOUT);
	EXPECT_EQ(TIMEOUT, (int) opts.get_timeout().count());
	EXPECT_EQ(TIMEOUT, c_struct.timeout);

	// Set with chrono duration
	opts.set_timeout(std::chrono::seconds(2*TIMEOUT/1000));
	EXPECT_EQ(2*TIMEOUT, (int) opts.get_timeout().count());
	EXPECT_EQ(2*TIMEOUT, c_struct.timeout);
}

// ----------------------------------------------------------------------
// Test set contect token
// ----------------------------------------------------------------------

TEST_F(disconnect_options_test, test_set_token) {
	auto tok = token::create(cli);
	mqtt::disconnect_options opts;

	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(nullptr, c_struct.onSuccess);
	EXPECT_EQ(nullptr, c_struct.onFailure);

	opts.set_token(mqtt::token_ptr());
	EXPECT_EQ(nullptr, c_struct.onSuccess);
	EXPECT_EQ(nullptr, c_struct.onFailure);

	opts.set_token(tok);
	EXPECT_NE(nullptr, c_struct.onSuccess);
	EXPECT_NE(nullptr, c_struct.onFailure);

	EXPECT_EQ(tok, opts.get_token());
}

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		//  __mqtt_disconnect_options_test_h
