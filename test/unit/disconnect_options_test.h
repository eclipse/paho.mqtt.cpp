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
	CPPUNIT_TEST( test_set_context );

	CPPUNIT_TEST_SUITE_END();

	const std::string EMPTY_STR;
	mqtt::test::dummy_async_client cli;

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test default constructor
// ----------------------------------------------------------------------

	void test_dflt_constructor() {
		mqtt::disconnect_options opts{};

		MQTTAsync_disconnectOptions& c_struct = opts.opts_;
		CPPUNIT_ASSERT(nullptr == c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr == c_struct.onFailure);

		CPPUNIT_ASSERT_EQUAL(0, opts.get_timeout());
	}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

	void test_user_constructor() {
		const int TIMEOUT { 14 };
		mqtt::token CONTEXT { cli };
		mqtt::disconnect_options opts { TIMEOUT, &CONTEXT };

		MQTTAsync_disconnectOptions& c_struct = opts.opts_;
		CPPUNIT_ASSERT(nullptr != c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr != c_struct.onFailure);

		CPPUNIT_ASSERT_EQUAL(TIMEOUT, opts.get_timeout());
		CPPUNIT_ASSERT_EQUAL(&CONTEXT, opts.get_context());
	}

// ----------------------------------------------------------------------
// Test set timeout
// ----------------------------------------------------------------------

	void test_set_timeout() {
		mqtt::disconnect_options opts{};

		const int TIMEOUT { 14 };
		opts.set_timeout(TIMEOUT);
		CPPUNIT_ASSERT_EQUAL(TIMEOUT, opts.get_timeout());
	}

// ----------------------------------------------------------------------
// Test set context
// ----------------------------------------------------------------------

	void test_set_context() {
		mqtt::token CONTEXT { cli };
		mqtt::disconnect_options opts{};

		MQTTAsync_disconnectOptions& c_struct = opts.opts_;
		CPPUNIT_ASSERT(nullptr == c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr == c_struct.onFailure);

		opts.set_context(nullptr);
		CPPUNIT_ASSERT(nullptr == c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr == c_struct.onFailure);

		opts.set_context(&CONTEXT);
		CPPUNIT_ASSERT(nullptr != c_struct.onSuccess);
		CPPUNIT_ASSERT(nullptr != c_struct.onFailure);

		CPPUNIT_ASSERT_EQUAL(&CONTEXT, opts.get_context());
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		//  __mqtt_disconnect_options_test_h
