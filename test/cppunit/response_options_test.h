// response_options_test.h
// Unit tests for the response_options class in the Paho MQTT C++ library.

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

#ifndef __mqtt_response_options_test_h
#define __mqtt_response_options_test_h

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/response_options.h"

#include "dummy_async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class response_options_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( response_options_test );

	CPPUNIT_TEST( test_dflt_constructor );
	CPPUNIT_TEST( test_user_constructor );
	CPPUNIT_TEST( test_set_token );

	CPPUNIT_TEST_SUITE_END();

	mqtt::test::dummy_async_client cli;

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test default constructor
// ----------------------------------------------------------------------

	void test_dflt_constructor() {
		mqtt::response_options opts;
		MQTTAsync_responseOptions& c_struct = opts.opts_;

		CPPUNIT_ASSERT(c_struct.context == nullptr);

		// Make sure the callback functions are set during object construction
		CPPUNIT_ASSERT(c_struct.onSuccess != nullptr);
		CPPUNIT_ASSERT(c_struct.onFailure != nullptr);
	}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

	void test_user_constructor() {
		mqtt::token_ptr token { std::make_shared<mqtt::token>(cli) };
		mqtt::response_options opts { token };
		MQTTAsync_responseOptions& c_struct = opts.opts_;

		CPPUNIT_ASSERT(c_struct.context == token.get());

		// Make sure the callback functions are set during object construction
		CPPUNIT_ASSERT(c_struct.onSuccess != nullptr);
		CPPUNIT_ASSERT(c_struct.onFailure != nullptr);
	}

// ----------------------------------------------------------------------
// Test set context
// ----------------------------------------------------------------------

	void test_set_token() {
		mqtt::response_options opts;
		MQTTAsync_responseOptions& c_struct = opts.opts_;

		CPPUNIT_ASSERT(c_struct.context == nullptr);
		mqtt::token_ptr token { std::make_shared<mqtt::token>(cli) };
		opts.set_token( token );
		CPPUNIT_ASSERT(c_struct.context == token.get());
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif //  __mqtt_response_options_test_h
