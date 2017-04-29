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
 *******************************************************************************/

#ifndef __mqtt_token_test_h
#define __mqtt_token_test_h

#include <algorithm>
#include <memory>
#include <stdexcept>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/token.h"
#include "dummy_async_client.h"
#include "dummy_action_listener.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class token_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( token_test );

	CPPUNIT_TEST( test_user_constructor_client );
	CPPUNIT_TEST( test_user_constructor_client_token );
	CPPUNIT_TEST( test_user_constructor_client_string );
	CPPUNIT_TEST( test_user_constructor_client_vector );
	CPPUNIT_TEST( test_on_success_with_data );
	CPPUNIT_TEST( test_on_success_without_data );
	CPPUNIT_TEST( test_on_failure_with_data );
	CPPUNIT_TEST( test_on_failure_without_data );
	CPPUNIT_TEST( test_action_callback );
	CPPUNIT_TEST( test_wait_success );
	CPPUNIT_TEST( test_wait_failure );
	CPPUNIT_TEST( test_wait_for_timeout );

	CPPUNIT_TEST_SUITE_END();

	using milliseconds = std::chrono::milliseconds;
	using steady_clock = std::chrono::steady_clock;

	mqtt::test::dummy_async_client cli;

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client)
// ----------------------------------------------------------------------

	void test_user_constructor_client() {
		mqtt::token tok{ cli };
		CPPUNIT_ASSERT_EQUAL(0, tok.get_message_id());
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iasync_client*>(&cli), tok.get_client());
		CPPUNIT_ASSERT(nullptr == tok.get_user_context());
		CPPUNIT_ASSERT(nullptr == tok.get_action_callback());
		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		CPPUNIT_ASSERT(nullptr == tok.get_topics());
	}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client, MQTTAsync_token)
// ----------------------------------------------------------------------

	void test_user_constructor_client_token() {
		MQTTAsync_token id {2};
		mqtt::token tok{ cli, id };
		CPPUNIT_ASSERT_EQUAL(id, tok.get_message_id());
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iasync_client*>(&cli), tok.get_client());
		CPPUNIT_ASSERT(nullptr == tok.get_user_context());
		CPPUNIT_ASSERT(nullptr == tok.get_action_callback());
		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		CPPUNIT_ASSERT(nullptr == tok.get_topics());
	}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client, string)
// ----------------------------------------------------------------------

	void test_user_constructor_client_string() {
		std::string topic { "topic" };
		mqtt::token tok{ cli, topic };
		CPPUNIT_ASSERT_EQUAL(0, tok.get_message_id());
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iasync_client*>(&cli), tok.get_client());
		CPPUNIT_ASSERT(nullptr == tok.get_user_context());
		CPPUNIT_ASSERT(nullptr == tok.get_action_callback());
		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		CPPUNIT_ASSERT(nullptr != tok.get_topics());
		CPPUNIT_ASSERT_EQUAL(size_t(1), tok.get_topics()->size());
		CPPUNIT_ASSERT_EQUAL(topic, (*tok.get_topics())[0]);
	}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client, vector<string>)
// ----------------------------------------------------------------------

	void test_user_constructor_client_vector() {
		auto topics = string_collection::create({ "topic1", "topic2" });
		mqtt::token tok{ cli, topics };
		CPPUNIT_ASSERT_EQUAL(0, tok.get_message_id());
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iasync_client*>(&cli), tok.get_client());
		CPPUNIT_ASSERT_EQUAL(static_cast<void*>(nullptr), tok.get_user_context());
		CPPUNIT_ASSERT_EQUAL(static_cast<mqtt::iaction_listener*>(nullptr), tok.get_action_callback());
		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		CPPUNIT_ASSERT(nullptr != tok.get_topics());
		CPPUNIT_ASSERT_EQUAL(size_t(2), tok.get_topics()->size());
		CPPUNIT_ASSERT_EQUAL((*topics)[0], (*tok.get_topics())[0]);
		CPPUNIT_ASSERT_EQUAL((*topics)[1], (*tok.get_topics())[1]);
	}

// ----------------------------------------------------------------------
// Test on success with data
// ----------------------------------------------------------------------

	void test_on_success_with_data() {
		mqtt::token tok{ cli };

		constexpr int MESSAGE_ID = 12;
		MQTTAsync_successData data = {
			.token = MESSAGE_ID,
		};

		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		token::on_success(&tok, &data);
		CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());
		CPPUNIT_ASSERT_EQUAL(MESSAGE_ID, tok.get_message_id());
	}

// ----------------------------------------------------------------------
// Test on success without data
// ----------------------------------------------------------------------

	void test_on_success_without_data() {
		mqtt::token tok{ cli };

		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		token::on_success(&tok, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());
	}

// ----------------------------------------------------------------------
// Test on failure with data
// ----------------------------------------------------------------------

	void test_on_failure_with_data() {
		mqtt::token tok{ cli };

		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		constexpr int MESSAGE_ID = 12;
		MQTTAsync_failureData data = {
			.token = MESSAGE_ID,
			.code = 13,
			.message = nullptr,
		};
		token::on_failure(&tok, &data);
		CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());
		CPPUNIT_ASSERT_EQUAL(MESSAGE_ID, tok.get_message_id());
	}

// ----------------------------------------------------------------------
// Test on failure without data
// ----------------------------------------------------------------------

	void test_on_failure_without_data() {
		mqtt::token tok{ cli };

		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		token::on_failure(&tok, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());
		CPPUNIT_ASSERT_EQUAL(0, tok.get_message_id());
	}

// ----------------------------------------------------------------------
// Test set callbacks
// ----------------------------------------------------------------------

	void test_action_callback() {
		mqtt::test::dummy_action_listener listener;
		mqtt::token tok{ cli };
		tok.set_action_callback(listener);
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iaction_listener*>(&listener), tok.get_action_callback());

		CPPUNIT_ASSERT_EQUAL(false, listener.on_success_called);
		token::on_success(&tok, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, listener.on_success_called);

		CPPUNIT_ASSERT_EQUAL(false, listener.on_failure_called);
		token::on_failure(&tok, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, listener.on_failure_called);
	}

// ----------------------------------------------------------------------
// Test wait for completion on success case
// All wait's should succeed immediately on successful completion.
// ----------------------------------------------------------------------

	void test_wait_success() {
		const auto TIMEOUT = milliseconds(10);

		mqtt::token tok{ cli };

		// NOTE: Make sure the complete flag is already true and the return
		// code (rc) is MQTTASYNC_SUCCESS, so the token::wait()
		// returns immediately. Otherwise we will get stuck in a single thread
		// that can't change the complete flag.
		token::on_success(&tok, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());

		// A wait does not reset the "complete" flag.

		try {
			tok.wait();
			CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());
		}
		catch (...) {
			CPPUNIT_FAIL("token::wait() should not throw on success");
		}

		// try_wait()
		try {
			CPPUNIT_ASSERT_EQUAL(true, tok.try_wait());
			CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());
		}
		catch (...) {
			CPPUNIT_FAIL("token::wait() should not throw on success");
		}

		// wait_for()
		try {
			CPPUNIT_ASSERT_EQUAL(true, tok.wait_for(TIMEOUT));
			CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());
		}
		catch (...) {
			CPPUNIT_FAIL("token::wait_for() should not throw on success");
		}

		// wait_until()
		const auto TO = steady_clock::now() + TIMEOUT;
		try {
			CPPUNIT_ASSERT_EQUAL(true, tok.wait_until(TO));
			CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());
		}
		catch (...) {
			CPPUNIT_FAIL("token::wait_until() should not throw on success");
		}
	}

// ----------------------------------------------------------------------
// Test wait for completion on failure case
// All wait's should throw if the action failed
// ----------------------------------------------------------------------

	void test_wait_failure() {
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
		token::on_failure(&tok, &data);

		CPPUNIT_ASSERT_EQUAL(true, tok.is_complete());
		CPPUNIT_ASSERT_EQUAL(MESSAGE_ID, tok.get_message_id());
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, tok.get_return_code());

		try {
			tok.wait();
			CPPUNIT_FAIL("token::wait() should throw on failure");
		}
		catch (mqtt::exception& ex) {
			CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex.get_reason_code());
		}

		try {
			tok.try_wait();
			CPPUNIT_FAIL("token::try_wait() should throw on failure");
		}
		catch (mqtt::exception& ex) {
			CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex.get_reason_code());
		}

		try {
			tok.wait_for(TIMEOUT);
			CPPUNIT_FAIL("token::wait_for() should throw on failure");
		}
		catch (mqtt::exception& ex) {
			CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex.get_reason_code());
		}

		try {
			tok.wait_until(steady_clock::now() + TIMEOUT);
			CPPUNIT_FAIL("token::wait_until() should throw on failure");
		}
		catch (mqtt::exception& ex) {
			CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex.get_reason_code());
		}
	}

// ----------------------------------------------------------------------
// Test wait for completion on failure due timeout case
// All waits should return false, but not throw, on a timeout
// ----------------------------------------------------------------------

	void test_wait_for_timeout() {
		const auto TIMEOUT = milliseconds(10);

		mqtt::token tok{ cli };

		// Test for timeout on non-signaled token.
		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());

		// try_wait()
		try {
			CPPUNIT_ASSERT_EQUAL(false, tok.try_wait());
		} 
		catch (...) {
			CPPUNIT_FAIL("token::try_wait() should not throw");
		}

		// wait_for()
		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		try {
			CPPUNIT_ASSERT_EQUAL(false, tok.wait_for(TIMEOUT));
		}
		catch (...) {
			CPPUNIT_FAIL("token::wait_for() should not throw on timeout");
		}

		// wait_until()
		const auto TO = steady_clock::now() + TIMEOUT;
		CPPUNIT_ASSERT_EQUAL(false, tok.is_complete());
		try {
			CPPUNIT_ASSERT_EQUAL(false, tok.wait_until(TO));
		}
		catch (...) {
			CPPUNIT_FAIL("token::wait_until() should not throw on timeout");
		}

	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_token_test_h

