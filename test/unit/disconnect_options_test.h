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
 *******************************************************************************/

#ifndef __mqtt_disconnect_options_test_h
#define __mqtt_disconnect_options_test_h

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/disconnect_options.h"

#include "dummy_async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class disconnect_options_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( disconnect_options_test );

	CPPUNIT_TEST( test_dflt_constructor );
	CPPUNIT_TEST( test_user_constructor );
	CPPUNIT_TEST( test_set_timeout );
	CPPUNIT_TEST( test_set_token );

	CPPUNIT_TEST_SUITE_END();

	const int DFLT_TIMEOUT = 0;

	const std::string EMPTY_STR;
	mqtt::test::dummy_async_client cli;

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test default constructor
// ----------------------------------------------------------------------

	void test_dflt_constructor() {
		mqtt::disconnect_options opts;

		CPPUNIT_ASSERT_EQUAL(DFLT_TIMEOUT, (int) opts.get_timeout().count());
		CPPUNIT_ASSERT(!opts.get_token());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(nullptr == c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr == c_struct.onFailure);

		CPPUNIT_ASSERT_EQUAL(DFLT_TIMEOUT, c_struct.timeout);
	}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

	void test_user_constructor() {
		const int TIMEOUT = 10;

		auto tok = token::create(cli);
		mqtt::disconnect_options opts { TIMEOUT, tok };

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(nullptr != c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr != c_struct.onFailure);

		CPPUNIT_ASSERT_EQUAL(TIMEOUT, (int) opts.get_timeout().count());
		CPPUNIT_ASSERT_EQUAL(tok, opts.get_token());
	}

// ----------------------------------------------------------------------
// Test set timeout
// ----------------------------------------------------------------------

	void test_set_timeout() {
		mqtt::disconnect_options opts;
		const auto& c_struct = opts.opts_;

		const int TIMEOUT = 5000;	// ms

		// Set with integer
		opts.set_timeout(TIMEOUT);
		CPPUNIT_ASSERT_EQUAL(TIMEOUT, (int) opts.get_timeout().count());
		CPPUNIT_ASSERT_EQUAL(TIMEOUT, c_struct.timeout);

		// Set with chrono duration
		opts.set_timeout(std::chrono::seconds(2*TIMEOUT/1000));
		CPPUNIT_ASSERT_EQUAL(2*TIMEOUT, (int) opts.get_timeout().count());
		CPPUNIT_ASSERT_EQUAL(2*TIMEOUT, c_struct.timeout);
	}

// ----------------------------------------------------------------------
// Test set contect token
// ----------------------------------------------------------------------

	void test_set_token() {
		auto tok = token::create(cli);
		mqtt::disconnect_options opts;

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(nullptr == c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr == c_struct.onFailure);

		opts.set_token(mqtt::token_ptr());
		CPPUNIT_ASSERT(nullptr == c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr == c_struct.onFailure);

		opts.set_token(tok);
		CPPUNIT_ASSERT(nullptr != c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr != c_struct.onFailure);

		CPPUNIT_ASSERT_EQUAL(tok, opts.get_token());
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		//  __mqtt_disconnect_options_test_h
