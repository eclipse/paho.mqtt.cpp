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
 *******************************************************************************/

#ifndef __mqtt_token_test_h
#define __mqtt_token_test_h

#include <algorithm>
#include <memory>
#include <stdexcept>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/token.h"
#include "mqtt/response_options.h"
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
	CPPUNIT_TEST( test_on_success );
	CPPUNIT_TEST( test_on_failure_with_data );
	CPPUNIT_TEST( test_on_failure_without_data );
	CPPUNIT_TEST( test_action_callback );
	CPPUNIT_TEST( test_wait_for_completion_success );
	CPPUNIT_TEST( test_wait_for_completion_failure );
	CPPUNIT_TEST( test_wait_for_completion_timeout_success );
	CPPUNIT_TEST( test_wait_for_completion_timeout_failure );

	CPPUNIT_TEST_SUITE_END();

	mqtt::test::dummy_async_client cli;

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client)
// ----------------------------------------------------------------------

	void test_user_constructor_client() {
		mqtt::token token{ cli };
		CPPUNIT_ASSERT_EQUAL(0, token.get_message_id());
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iasync_client*>(&cli), token.get_client());
		CPPUNIT_ASSERT_EQUAL(static_cast<void*>(nullptr), token.get_user_context());
		CPPUNIT_ASSERT_EQUAL(static_cast<mqtt::iaction_listener*>(nullptr), token.get_action_callback());
		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(token.get_topics().size()));
	}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client, MQTTAsync_token)
// ----------------------------------------------------------------------

	void test_user_constructor_client_token() {
		MQTTAsync_token id {2};
		mqtt::token token{ cli, id };
		CPPUNIT_ASSERT_EQUAL(id, token.get_message_id());
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iasync_client*>(&cli), token.get_client());
		CPPUNIT_ASSERT_EQUAL(static_cast<void*>(nullptr), token.get_user_context());
		CPPUNIT_ASSERT_EQUAL(static_cast<mqtt::iaction_listener*>(nullptr), token.get_action_callback());
		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(token.get_topics().size()));
	}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client, string)
// ----------------------------------------------------------------------

	void test_user_constructor_client_string() {
		std::string topic { "topic" };
		mqtt::token token{ cli, topic };
		CPPUNIT_ASSERT_EQUAL(0, token.get_message_id());
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iasync_client*>(&cli), token.get_client());
		CPPUNIT_ASSERT_EQUAL(static_cast<void*>(nullptr), token.get_user_context());
		CPPUNIT_ASSERT_EQUAL(static_cast<mqtt::iaction_listener*>(nullptr), token.get_action_callback());
		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(token.get_topics().size()));
		CPPUNIT_ASSERT_EQUAL(topic, token.get_topics()[0]);
	}

// ----------------------------------------------------------------------
// Test user constructor (iasync_client, vector<string>)
// ----------------------------------------------------------------------

	void test_user_constructor_client_vector() {
		std::vector<std::string> topics { "topic1", "topic2" };
		mqtt::token token{ cli, topics };
		CPPUNIT_ASSERT_EQUAL(0, token.get_message_id());
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iasync_client*>(&cli), token.get_client());
		CPPUNIT_ASSERT_EQUAL(static_cast<void*>(nullptr), token.get_user_context());
		CPPUNIT_ASSERT_EQUAL(static_cast<mqtt::iaction_listener*>(nullptr), token.get_action_callback());
		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(token.get_topics().size()));
		CPPUNIT_ASSERT_EQUAL(topics[0], token.get_topics()[0]);
		CPPUNIT_ASSERT_EQUAL(topics[1], token.get_topics()[1]);
	}

// ----------------------------------------------------------------------
// Test on success
// ----------------------------------------------------------------------

	void test_on_success() {
		// Note: use connect_options to access token::on_success(). Otherwise
		//       we have to make the token_test class a friend of token. Because
		//       those methods are private.
		mqtt::token token{ cli };
		mqtt::connect_options co;
		MQTTAsync_connectOptions& c_struct = co.opts_;

		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		c_struct.onSuccess(&token, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, token.is_complete());
	}

// ----------------------------------------------------------------------
// Test on failure with data
// ----------------------------------------------------------------------

	void test_on_failure_with_data() {
		// Note: use disconnect_options to access token::on_failure(). Otherwise
		//       we have to make the token_test class a friend of token. Because
		//       those methods are private.
		mqtt::token token{ cli };
		mqtt::disconnect_options dco;

		dco.set_context(&token);
		MQTTAsync_disconnectOptions& c_struct = dco.opts_;

		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		constexpr int MESSAGE_ID = 12;
		MQTTAsync_failureData data = {
			.token = MESSAGE_ID,
			.code = 13,
			.message = nullptr,
		};
		c_struct.onFailure(&token, &data);
		CPPUNIT_ASSERT_EQUAL(true, token.is_complete());
		CPPUNIT_ASSERT_EQUAL(MESSAGE_ID, token.get_message_id());
	}

// ----------------------------------------------------------------------
// Test on failure without data
// ----------------------------------------------------------------------

	void test_on_failure_without_data() {
		// Note: use disconnect_options to access token::on_failure(). Otherwise
		//       we have to make the token_test class a friend of token. Because
		//       those methods are private.
		mqtt::token token{ cli };
		mqtt::disconnect_options dco;

		dco.set_context(&token);
		MQTTAsync_disconnectOptions& c_struct = dco.opts_;

		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		c_struct.onFailure(&token, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, token.is_complete());
		CPPUNIT_ASSERT_EQUAL(0, token.get_message_id());
	}

// ----------------------------------------------------------------------
// Test set callbacks
// ----------------------------------------------------------------------

	void test_action_callback() {
		// Note: use connect_options to access token::on_success() and
		//       token::on_failure(). Otherwise we have to make the
		//       token_test class a friend of token. Because those methods
		//       are private.
		mqtt::test::dummy_action_listener listener;
		mqtt::token token{ cli };
		token.set_action_callback(listener);
		CPPUNIT_ASSERT_EQUAL(dynamic_cast<mqtt::iaction_listener*>(&listener), token.get_action_callback());

		mqtt::response_options ro;
		MQTTAsync_responseOptions& c_struct = ro.opts_;

		CPPUNIT_ASSERT_EQUAL(false, listener.on_success_called);
		c_struct.onSuccess(&token, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, listener.on_success_called);

		CPPUNIT_ASSERT_EQUAL(false, listener.on_failure_called);
		c_struct.onFailure(&token, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, listener.on_failure_called);
	}

// ----------------------------------------------------------------------
// Test wait for completion on success case
// ----------------------------------------------------------------------

	void test_wait_for_completion_success() {
		mqtt::token token{ cli };
		mqtt::connect_options co;
		MQTTAsync_connectOptions& c_struct = co.opts_;

		// NOTE: Make sure the complete flag is already true and the return
		// code (rc) is MQTTASYNC_SUCCESS, so the token::wait_for_completion()
		// returns immediately. Otherwise we will get stuck in a single thread
		// that can't change the complete flag.
		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		c_struct.onSuccess(&token, nullptr);
		CPPUNIT_ASSERT_EQUAL(true, token.is_complete());
		token.wait_for_completion();
	}

// ----------------------------------------------------------------------
// Test wait for completion on failure case
// ----------------------------------------------------------------------

	void test_wait_for_completion_failure() {
		mqtt::token token{ cli };
		mqtt::connect_options co;
		MQTTAsync_connectOptions& c_struct = co.opts_;

		// NOTE: Make sure the complete flag is already true and the return
		// code (rc) is MQTTASYNC_FAILURE, so the token::wait_for_completion()
		// returns immediately. Otherwise we will get stuck in a single thread
		// that can't change the complete flag.
		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		constexpr int MESSAGE_ID = 12;
		MQTTAsync_failureData data = {
				.token = MESSAGE_ID,
				.code = MQTTASYNC_FAILURE,
				.message = nullptr,
		};
		c_struct.onFailure(&token, &data);
		CPPUNIT_ASSERT_EQUAL(true, token.is_complete());
		CPPUNIT_ASSERT_EQUAL(MESSAGE_ID, token.get_message_id());
		try {
			token.wait_for_completion();
		} catch (mqtt::exception& ex) {
			CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex.get_reason_code());
		}
	}

// ----------------------------------------------------------------------
// Test wait for completion on success due timeout case
// ----------------------------------------------------------------------

	void test_wait_for_completion_timeout_success() {
		mqtt::token token{ cli };

		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		mqtt::response_options ro;
		MQTTAsync_responseOptions& c_struct = ro.opts_;
		c_struct.onSuccess(&token, nullptr);

		// timeout == 0
		CPPUNIT_ASSERT_EQUAL(true, token.is_complete());
		token.wait_for_completion(0);

		// timeout > 0
		CPPUNIT_ASSERT_EQUAL(true, token.is_complete());
		token.wait_for_completion(10);

		// timeout < 0
		CPPUNIT_ASSERT_EQUAL(true, token.is_complete());
		token.wait_for_completion(-10);
	}

// ----------------------------------------------------------------------
// Test wait for completion on failure due timeout case
// ----------------------------------------------------------------------

	void test_wait_for_completion_timeout_failure() {
		mqtt::token token{ cli };

		// timeout == 0
		// NOTE: This test must be performed BEFORE invoking the onFailure()
		// callback. Because complete_ must be false.
		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		try {
			token.wait_for_completion(0);
		} catch (mqtt::exception& ex) {
			CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex.get_reason_code());
		}

		// timeout > 0
		// NOTE: This test must be performed BEFORE invoking the onFailure()
		// callback. Because we will make condition_variable::wait_for() to
		// fail.
		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		try {
			token.wait_for_completion(10);
		} catch (mqtt::exception& ex) {
			CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex.get_reason_code());
		}

		CPPUNIT_ASSERT_EQUAL(false, token.is_complete());
		constexpr int MESSAGE_ID = 12;
		MQTTAsync_failureData data = {
				.token = MESSAGE_ID,
				.code = MQTTASYNC_FAILURE,
				.message = nullptr,
		};
		mqtt::response_options ro;
		MQTTAsync_responseOptions& c_struct = ro.opts_;
		c_struct.onFailure(&token, &data);

		// timeout < 0
		CPPUNIT_ASSERT_EQUAL(true, token.is_complete());
		try {
			token.wait_for_completion(-10);
		} catch (mqtt::exception& ex) {
			CPPUNIT_ASSERT_EQUAL(MQTTASYNC_FAILURE, ex.get_reason_code());
		}
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_token_test_h
