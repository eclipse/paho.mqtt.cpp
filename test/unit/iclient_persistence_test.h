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
	CPPUNIT_TEST( test_persistence_put_empty_buffers );
	CPPUNIT_TEST( test_persistence_get );
	CPPUNIT_TEST( test_persistence_remove );
	CPPUNIT_TEST( test_persistence_keys );
	CPPUNIT_TEST( test_persistence_clear );

	CPPUNIT_TEST_SUITE_END();

	const char* CLIENT_ID = "clientid";
	const char* SERVER_URI = "serveruri";
	const char* KEY = "key";
	const char* INVALID_KEY = "invalid_key";

	const char* PAYLOAD = "some random data";
	const size_t PAYLOAD_LEN = strlen(PAYLOAD);

	const char* PAYLOAD2 = "some other random data";
	const size_t PAYLOAD2_LEN = strlen(PAYLOAD2);

	const char* PAYLOAD3 = "some completely random data";
	const size_t PAYLOAD3_LEN = strlen(PAYLOAD3);

	using dcp = mqtt::test::dummy_client_persistence;

	dcp per_;
	void* handle_;

public:
	void setUp() {
		void* context = static_cast<void*>(&per_);
		dcp::persistence_open(&handle_, CLIENT_ID, SERVER_URI, context);

		// Put no buffer
		int bufcount = 3;
		const char* bufs[] = { PAYLOAD, PAYLOAD2, PAYLOAD3 };
		int buflens[] = { int(PAYLOAD_LEN), int(PAYLOAD2_LEN), int(PAYLOAD3_LEN) };

		dcp::persistence_put(handle_, const_cast<char*>(KEY), bufcount,
							 const_cast<char**>(bufs), buflens);
	}
	void tearDown() {
		dcp::persistence_clear(handle_);
		dcp::persistence_close(handle_);
	}

// ----------------------------------------------------------------------
// Test static method persistence_open()
// ----------------------------------------------------------------------

	void test_persistence_open() {
		dcp per;
		void* handle = nullptr;

		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_open(&handle, CLIENT_ID, SERVER_URI, nullptr));

		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per));
		CPPUNIT_ASSERT_EQUAL(handle, static_cast<void*>(&per));
	}

// ----------------------------------------------------------------------
// Test static method persistence_close()
// ----------------------------------------------------------------------

	void test_persistence_close() {
		dcp per;

		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_close(nullptr));

		void* context = static_cast<void*>(&per);
		void* handle = nullptr;
		dcp::persistence_open(&handle, CLIENT_ID, SERVER_URI, context);

		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_close(handle));
	}

// ----------------------------------------------------------------------
// Test static method persistence_put()
// ----------------------------------------------------------------------

	void test_persistence_put_0_buffer() {
		dcp per;
		void* handle;
		dcp::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per);

		// Put no buffer
		int bufcount = 0;
		const char* bufs[] = { PAYLOAD };
		int buflens[] = { int(PAYLOAD_LEN) };

		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_put(handle, const_cast<char*>(KEY), bufcount,
												  const_cast<char**>(bufs), buflens));
	}

	void test_persistence_put_1_buffer() {
		dcp per;
		void* handle;
		dcp::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per);

		// Put no buffer
		int bufcount = 1;
		const char* bufs[] = { PAYLOAD };
		int buflens[] = { int(PAYLOAD_LEN) };

		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_put(handle, const_cast<char*>(KEY), bufcount,
												  const_cast<char**>(bufs), buflens));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_containskey(handle, const_cast<char*>(KEY)));
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_containskey(handle, const_cast<char*>(INVALID_KEY)));
	}

	void test_persistence_put_2_buffers() {
		dcp per;
		void* handle;
		dcp::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per);

		// Put no buffer
		int bufcount = 2;
		const char* bufs[] = { PAYLOAD, PAYLOAD2 };
		int buflens[] = { int(PAYLOAD_LEN), int(PAYLOAD2_LEN) };

		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_put(handle, const_cast<char*>(KEY), bufcount,
												  const_cast<char**>(bufs), buflens));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_containskey(handle, const_cast<char*>(KEY)));
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_containskey(handle, const_cast<char*>(INVALID_KEY)));
	}

	void test_persistence_put_3_buffers() {
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_containskey(handle_, const_cast<char*>(KEY)));
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_containskey(handle_, const_cast<char*>(INVALID_KEY)));
	}

	void test_persistence_put_empty_buffers() {
		dcp per;
		void* handle;
		dcp::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per);

		// Put three empty buffers
		int bufcount = 3;
		const char* buffers[] = { "", "", "" };
		int buflens[] = { 0, 0, 0 };
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_put(handle, const_cast<char*>(KEY),
												  bufcount, const_cast<char**>(buffers), buflens));
	}

// ----------------------------------------------------------------------
// Test static method persistence_get()
// ----------------------------------------------------------------------

	void test_persistence_get() {
		char* buf = nullptr;
		int buflen = 0;
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_get(handle_, const_cast<char*>(INVALID_KEY), &buf, &buflen));
		CPPUNIT_ASSERT_EQUAL(0, buflen);

		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_get(handle_, const_cast<char*>(KEY), &buf, &buflen));

		int n = PAYLOAD_LEN + PAYLOAD2_LEN + PAYLOAD3_LEN;
		string str { PAYLOAD };
		str += PAYLOAD2;
		str += PAYLOAD3;

		CPPUNIT_ASSERT_EQUAL(n, buflen);
		CPPUNIT_ASSERT(buf != nullptr);
		CPPUNIT_ASSERT(!memcmp(str.data(), buf, n));
	}

// ----------------------------------------------------------------------
// Test static method persistence_remove()
// ----------------------------------------------------------------------

	void test_persistence_remove() {
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_remove(handle_, const_cast<char*>(INVALID_KEY)));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_containskey(handle_, const_cast<char*>(KEY)));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_remove(handle_, const_cast<char*>(KEY)));
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_containskey(handle_, const_cast<char*>(KEY)));
	}

// ----------------------------------------------------------------------
// Test static method persistence_keys()
// ----------------------------------------------------------------------

	void test_persistence_keys() {
		char** keys = nullptr;
		int nkeys = 0;

		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_keys(handle_, &keys, &nkeys));
		CPPUNIT_ASSERT_EQUAL(1, nkeys);
		CPPUNIT_ASSERT_EQUAL(std::string(KEY), std::string(keys[0]));
		//CPPUNIT_ASSERT(!strcmp(KEY, keys[0]));
	}

// ----------------------------------------------------------------------
// Test static method persistence_clear()
// ----------------------------------------------------------------------

	void test_persistence_clear() {
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_containskey(handle_, const_cast<char*>(KEY)));
		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS, dcp::persistence_clear(handle_));
		CPPUNIT_ASSERT_EQUAL(MQTTCLIENT_PERSISTENCE_ERROR,
							 dcp::persistence_containskey(handle_, const_cast<char*>(KEY)));

		char** keys = nullptr;
		int nkeys = -1;

		CPPUNIT_ASSERT_EQUAL(MQTTASYNC_SUCCESS,
							 dcp::persistence_keys(handle_, &keys, &nkeys));
		CPPUNIT_ASSERT_EQUAL(0, nkeys);
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_iclient_persistence_test_h

