// iclient_persistence_test.h
// Unit tests for the iclient_persistence class in the Paho MQTT C++ library.

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

#ifndef __mqtt_iclient_persistence_test_h
#define __mqtt_iclient_persistence_test_h

#include <algorithm>
#include <memory>
#include <stdexcept>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/iclient_persistence.h"

#include "dummy_persistable.h"
#include "dummy_client_persistence.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class iclient_persistence_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( iclient_persistence_test );

	CPPUNIT_TEST( test_persistence_open );
	CPPUNIT_TEST( test_persistence_close );
	CPPUNIT_TEST( test_persistence_put_0_buffer );
	CPPUNIT_TEST( test_persistence_put_1_buffer );
	CPPUNIT_TEST( test_persistence_put_2_buffers );
	CPPUNIT_TEST( test_persistence_put_3_buffers );
	CPPUNIT_TEST( test_persistence_put_3_empty_buffers );
	CPPUNIT_TEST( test_persistence_get );
	CPPUNIT_TEST( test_persistence_remove );
	CPPUNIT_TEST( test_persistence_keys );
	CPPUNIT_TEST( test_persistence_clear );
	CPPUNIT_TEST( test_persistence_containskey );

	CPPUNIT_TEST_SUITE_END();

	using dp = mqtt::test::dummy_persistable;
	using dcp = mqtt::test::dummy_client_persistence;

	mqtt::test::dummy_client_persistence cli_per;

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test static method persistence_open()
// ----------------------------------------------------------------------

	void test_persistence_open() {
		void* context = static_cast<void*>(&cli_per);
		void* handle = nullptr;
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_open(&handle, dcp::CLIENT_ID, "", context));
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_open(&handle, "", dcp::SERVER_URI, context));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_open(&handle, dcp::CLIENT_ID, dcp::SERVER_URI, context));
		CPPUNIT_ASSERT_EQUAL(handle, context);
	}

// ----------------------------------------------------------------------
// Test static method persistence_close()
// ----------------------------------------------------------------------

	void test_persistence_close() {
		void* handle = static_cast<void*>(&cli_per);
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_close(handle));
	}

// ----------------------------------------------------------------------
// Test static method persistence_put()
// ----------------------------------------------------------------------

	void test_persistence_put_0_buffer() {
		void* handle = static_cast<void*>(&cli_per);

		// Put no buffer
		int bufcount = 0;
		const char* buffers[] = { dp::PAYLOAD };
		int buflens[] = { static_cast<int>(strlen(buffers[0])) };
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_put(handle, const_cast<char*>(dcp::KEY_INVALID), bufcount, const_cast<char**>(buffers), buflens));
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_put(handle, const_cast<char*>(dcp::KEY_VALID), bufcount, const_cast<char**>(buffers), buflens));
	}

	void test_persistence_put_1_buffer() {
		void* handle = static_cast<void*>(&cli_per);

		// Put one buffer
		int bufcount = 1;
		const char* buffers[] = { dp::PAYLOAD };
		int buflens[] = { static_cast<int>(strlen(buffers[0])) };
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_put(handle, const_cast<char*>(dcp::KEY_INVALID), bufcount, const_cast<char**>(buffers), buflens));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_put(handle, const_cast<char*>(dcp::KEY_VALID), bufcount, const_cast<char**>(buffers), buflens));
	}

	void test_persistence_put_2_buffers() {
		void* handle = static_cast<void*>(&cli_per);

		// Put two buffers
		int bufcount = 2;
		const char* buffers[] = { dp::PAYLOAD, dp::PAYLOAD };
		int buflens[] = { static_cast<int>(strlen(buffers[0])), static_cast<int>(strlen(buffers[1])) };
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_put(handle, const_cast<char*>(dcp::KEY_INVALID), bufcount, const_cast<char**>(buffers), buflens));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_put(handle, const_cast<char*>(dcp::KEY_VALID), bufcount, const_cast<char**>(buffers), buflens));
	}

	void test_persistence_put_3_buffers() {
		void* handle = static_cast<void*>(&cli_per);

		// Put three buffers
		int bufcount = 3;
		const char* buffers[] = { dp::PAYLOAD, dp::PAYLOAD, dp::PAYLOAD };
		int buflens[] = { static_cast<int>(strlen(buffers[0])), static_cast<int>(strlen(buffers[1])), static_cast<int>(strlen(buffers[2])) };
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_put(handle, const_cast<char*>(dcp::KEY_INVALID), bufcount, const_cast<char**>(buffers), buflens));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_put(handle, const_cast<char*>(dcp::KEY_VALID), bufcount, const_cast<char**>(buffers), buflens));
	}

	void test_persistence_put_3_empty_buffers() {
		void* handle = static_cast<void*>(&cli_per);

		// Put three empty buffers
		int bufcount = 3;
		const char* buffers[] = { "", "", "" };
		int buflens[] = { 0, 0, 0 };
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_put(handle, const_cast<char*>(dcp::KEY_VALID), bufcount, const_cast<char**>(buffers), buflens));
	}

// ----------------------------------------------------------------------
// Test static method persistence_get()
// ----------------------------------------------------------------------

	void test_persistence_get() {
		void* handle = static_cast<void*>(&cli_per);
		char* buffer = nullptr;
		int buflen = 0;
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_get(handle, const_cast<char*>(dcp::KEY_INVALID), &buffer, &buflen));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_get(handle, const_cast<char*>(dcp::KEY_VALID), &buffer, &buflen));
		std::string header_payload = dp::HEADER;
		header_payload += dp::PAYLOAD;
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(header_payload.size()), buflen);
		CPPUNIT_ASSERT(std::equal(buffer, buffer + buflen, header_payload.c_str()));
	}

// ----------------------------------------------------------------------
// Test static method persistence_remove()
// ----------------------------------------------------------------------

	void test_persistence_remove() {
		void* handle = static_cast<void*>(&cli_per);
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_remove(handle, const_cast<char*>(dcp::KEY_INVALID)));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_remove(handle, const_cast<char*>(dcp::KEY_VALID)));
	}

// ----------------------------------------------------------------------
// Test static method persistence_keys()
// ----------------------------------------------------------------------

	void test_persistence_keys() {
		void* handle = static_cast<void*>(&cli_per);
		char** keys = nullptr;
		int nkeys = 0;
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_keys(handle, &keys, &nkeys));
		CPPUNIT_ASSERT_EQUAL(1, nkeys);
		CPPUNIT_ASSERT(std::equal(keys[0], keys[0] + strlen(keys[0]), dcp::KEY_VALID));
	}

// ----------------------------------------------------------------------
// Test static method persistence_clear()
// ----------------------------------------------------------------------

	void test_persistence_clear() {
		void* handle = static_cast<void*>(&cli_per);
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_clear(handle));
	}

// ----------------------------------------------------------------------
// Test static method persistence_containskey()
// ----------------------------------------------------------------------

	void test_persistence_containskey() {
		void* handle = static_cast<void*>(&cli_per);
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR, dcp::persistence_containskey(handle, const_cast<char*>(dcp::KEY_INVALID)));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_containskey(handle, const_cast<char*>(dcp::KEY_VALID)));
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_iclient_persistence_test_h
