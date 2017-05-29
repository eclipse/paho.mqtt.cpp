// token_test.h
// Unit tests for the token class in the Paho MQTT C++ library.

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
 *    Frank Pagliughi - additional tests. Made this test a friend of token.
 *    Guilherme M. Ferreira - changed test framework from CppUnit to GTest
 *******************************************************************************/

#ifndef __mqtt_token_test_h
#define __mqtt_token_test_h

#include <algorithm>
#include <memory>
#include <stdexcept>

#include <gtest/gtest.h>

#include "mqtt/token.h"
#include "dummy_async_client.h"
#include "dummy_action_listener.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class token_test : public ::testing::Test
{
protected:

	using milliseconds = std::chrono::milliseconds;
	using steady_clock = std::chrono::steady_clock;

	mqtt::test::dummy_async_client cli;

	MQTTAsync_connectOptions& get_c_struct(mqtt::connect_options& opts) {
		return opts.opts_;
	}

	MQTTAsync_disconnectOptions& get_c_struct(mqtt::disconnect_options& opts) {
		return opts.opts_;
	}

	MQTTAsync_responseOptions& get_c_struct(mqtt::response_options& opts) {
		return opts.opts_;
	}

	static void on_success(void* tokObj, MQTTAsync_successData* rsp) {
		token::on_success(tokObj, rsp);
	}

	static void on_failure(void* tokObj, MQTTAsync_failureData* rsp) {
		token::on_failure(tokObj, rsp);
	}

public:
	void SetUp() {}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test user constructor (iasync_client)
// ----------------------------------------------------------------------

TEST_F(token_test, test_user_constructor_client) {
	mqtt::token tok{ cli };
	EXPECT_EQ(0, tok.get_message_id());
	EXPECT_EQ(dynamic_cast<mqtt::iasync_client*>(&cli), tok.get_client());
	EXPECT_EQ(nullptr, tok.get_user_context());
	EXPECT_EQ(nullptr, tok.get_action_callback());
	EXPECT_FALSE(tok.is_complete());
	EXPECT_EQ(nullptr, tok.get_topics());
}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client, MQTTAsync_token)
// ----------------------------------------------------------------------

TEST_F(token_test, test_user_constructor_client_token) {
	MQTTAsync_token id {2};
	mqtt::token tok{ cli, id };
	EXPECT_EQ(id, tok.get_message_id());
	EXPECT_EQ(dynamic_cast<mqtt::iasync_client*>(&cli), tok.get_client());
	EXPECT_EQ(nullptr, tok.get_user_context());
	EXPECT_EQ(nullptr, tok.get_action_callback());
	EXPECT_FALSE(tok.is_complete());
	EXPECT_EQ(nullptr, tok.get_topics());
}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client, string)
// ----------------------------------------------------------------------

TEST_F(token_test, test_user_constructor_client_string) {
	std::string topic { "topic" };
	mqtt::token tok{ cli, topic };
	EXPECT_EQ(0, tok.get_message_id());
	EXPECT_EQ(dynamic_cast<mqtt::iasync_client*>(&cli), tok.get_client());
	EXPECT_EQ(nullptr, tok.get_user_context());
	EXPECT_EQ(nullptr, tok.get_action_callback());
	EXPECT_FALSE(tok.is_complete());
	EXPECT_NE(nullptr, tok.get_topics());
	EXPECT_EQ(size_t(1), tok.get_topics()->size());
	EXPECT_EQ(topic, (*tok.get_topics())[0]);
}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client, vector<string>)
// ----------------------------------------------------------------------

TEST_F(token_test, test_user_constructor_client_vector) {
	auto topics = string_collection::create({ "topic1", "topic2" });
	mqtt::token tok{ cli, topics };
	EXPECT_EQ(0, tok.get_message_id());
	EXPECT_EQ(dynamic_cast<mqtt::iasync_client*>(&cli), tok.get_client());
	EXPECT_EQ(static_cast<void*>(nullptr), tok.get_user_context());
	EXPECT_EQ(static_cast<mqtt::iaction_listener*>(nullptr), tok.get_action_callback());
	EXPECT_FALSE(tok.is_complete());
	EXPECT_NE(nullptr, tok.get_topics());
	EXPECT_EQ(size_t(2), tok.get_topics()->size());
	EXPECT_EQ((*topics)[0], (*tok.get_topics())[0]);
	EXPECT_EQ((*topics)[1], (*tok.get_topics())[1]);
}

// ----------------------------------------------------------------------
// Test on success with data
// ----------------------------------------------------------------------

TEST_F(token_test, test_on_success_with_data) {
	mqtt::token tok{ cli };

	constexpr int MESSAGE_ID = 12;
	MQTTAsync_successData data = {
		.token = MESSAGE_ID,
	};

	EXPECT_FALSE(tok.is_complete());
	token_test::on_success(&tok, &data);
	EXPECT_TRUE(tok.is_complete());
	EXPECT_EQ(MESSAGE_ID, tok.get_message_id());
}

// ----------------------------------------------------------------------
// Test on success without data
// ----------------------------------------------------------------------

TEST_F(token_test, test_on_success_without_data) {
	mqtt::token tok{ cli };

	EXPECT_FALSE(tok.is_complete());
	token_test::on_success(&tok, nullptr);
	EXPECT_TRUE(tok.is_complete());
}

// ----------------------------------------------------------------------
// Test on failure with data
// ----------------------------------------------------------------------

TEST_F(token_test, test_on_failure_with_data) {
	mqtt::token tok{ cli };

	EXPECT_FALSE(tok.is_complete());
	constexpr int MESSAGE_ID = 12;
	MQTTAsync_failureData data = {
		.token = MESSAGE_ID,
		.code = 13,
		.message = nullptr,
	};
	token_test::on_failure(&tok, &data);
	EXPECT_TRUE(tok.is_complete());
	EXPECT_EQ(MESSAGE_ID, tok.get_message_id());
}

// ----------------------------------------------------------------------
// Test on failure without data
// ----------------------------------------------------------------------

TEST_F(token_test, test_on_failure_without_data) {
	mqtt::token tok{ cli };

	EXPECT_FALSE(tok.is_complete());
	token_test::on_failure(&tok, nullptr);
	EXPECT_TRUE(tok.is_complete());
	EXPECT_EQ(0, tok.get_message_id());
}

// ----------------------------------------------------------------------
// Test set callbacks
// ----------------------------------------------------------------------

TEST_F(token_test, test_action_callback) {
	mqtt::test::dummy_action_listener listener;
	mqtt::token tok{ cli };
	tok.set_action_callback(listener);
	EXPECT_EQ(dynamic_cast<mqtt::iaction_listener*>(&listener), tok.get_action_callback());

	EXPECT_FALSE(listener.on_success_called);
	token_test::on_success(&tok, nullptr);
	EXPECT_TRUE(listener.on_success_called);

	EXPECT_FALSE(listener.on_failure_called);
	token_test::on_failure(&tok, nullptr);
	EXPECT_TRUE(listener.on_failure_called);
}

// ----------------------------------------------------------------------
// Test wait for completion on success case
// All wait's should succeed immediately on successful completion.
// ----------------------------------------------------------------------

TEST_F(token_test, test_wait_success) {
	const auto TIMEOUT = milliseconds(10);

	mqtt::token tok{ cli };

	// NOTE: Make sure the complete flag is already true and the return
	// code (rc) is MQTTASYNC_SUCCESS, so the token::wait()
	// returns immediately. Otherwise we will get stuck in a single thread
	// that can't change the complete flag.
	token_test::on_success(&tok, nullptr);
	EXPECT_TRUE(tok.is_complete());

	// A wait does not reset the "complete" flag.

	EXPECT_NO_THROW(tok.wait());
	EXPECT_TRUE(tok.is_complete());

	// try_wait()
	bool try_wait_ret = false;
	EXPECT_NO_THROW(try_wait_ret = tok.try_wait());
	EXPECT_TRUE(try_wait_ret);
	EXPECT_TRUE(tok.is_complete());

	// wait_for()
	bool wait_for_ret = false;
	EXPECT_NO_THROW(wait_for_ret = tok.wait_for(TIMEOUT));
	EXPECT_TRUE(wait_for_ret);
	EXPECT_TRUE(tok.is_complete());

	// wait_until()
	const auto TO = steady_clock::now() + TIMEOUT;
	bool wait_until_ret = false;
	EXPECT_NO_THROW(wait_until_ret = tok.wait_until(TO));
	EXPECT_TRUE(wait_until_ret);
	EXPECT_TRUE(tok.is_complete());
}

// ----------------------------------------------------------------------
// Test wait for completion on failure case
// All wait's should throw if the action failed
// ----------------------------------------------------------------------

TEST_F(token_test, test_wait_failure) {
	const auto TIMEOUT = milliseconds(10);

	mqtt::token tok{ cli };

	// NOTE: Make sure the complete flag is already true and the return
	// code (rc) is MQTTASYNC_FAILURE, so the token::wait()
	// returns immediately. Otherwise we will get stuck in a single thread
	// that can't change the complete flag.
	constexpr int MESSAGE_ID = 12;
	MQTTAsync_failureData data = {
		.token = MESSAGE_ID,
		.code = MQTTASYNC_FAILURE,
		.message = nullptr,
	};
	token_test::on_failure(&tok, &data);

	EXPECT_TRUE(tok.is_complete());
	EXPECT_EQ(MESSAGE_ID, tok.get_message_id());
	EXPECT_EQ(MQTTASYNC_FAILURE, tok.get_return_code());

	EXPECT_THROW(tok.wait(), mqtt::exception);

	EXPECT_THROW(tok.try_wait(), mqtt::exception);

	EXPECT_THROW(tok.wait_for(TIMEOUT), mqtt::exception);

	EXPECT_THROW(tok.wait_until(steady_clock::now() + TIMEOUT), mqtt::exception);
}

// ----------------------------------------------------------------------
// Test wait for completion on failure due timeout case
// All waits should return false, but not throw, on a timeout
// ----------------------------------------------------------------------

TEST_F(token_test, test_wait_for_timeout) {
	const auto TIMEOUT = milliseconds(10);

	mqtt::token tok{ cli };

	// Test for timeout on non-signaled token.
	EXPECT_FALSE(tok.is_complete());

	// try_wait()
	bool try_wait_ret = true;
	EXPECT_NO_THROW(try_wait_ret = tok.try_wait());
	EXPECT_FALSE(try_wait_ret);

	// wait_for()
	EXPECT_FALSE(tok.is_complete());
	bool wait_for_ret = true;
	EXPECT_NO_THROW(wait_for_ret = tok.wait_for(TIMEOUT));
	EXPECT_FALSE(wait_for_ret);

	// wait_until()
	const auto TO = steady_clock::now() + TIMEOUT;
	EXPECT_FALSE(tok.is_complete());
	bool wait_until_ret = true;
	EXPECT_NO_THROW(wait_until_ret = tok.wait_until(TO));
	EXPECT_FALSE(wait_until_ret);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_token_test_h
