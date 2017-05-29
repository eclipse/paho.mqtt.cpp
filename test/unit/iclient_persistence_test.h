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
 *    Guilherme M. Ferreira - changed test framework from CppUnit to GTest
 *******************************************************************************/

#ifndef __mqtt_iclient_persistence_test_h
#define __mqtt_iclient_persistence_test_h

#include <algorithm>
#include <memory>
#include <stdexcept>

#include <gtest/gtest.h>

#include "mqtt/iclient_persistence.h"
#include "dummy_client_persistence.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class iclient_persistence_test : public ::testing::Test
{
protected:
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
	using cpt = iclient_persistence_test;

	dcp per_;
	void* handle_;

	static int persistence_open(void** handle, const char* clientID, const char* serverURI, void* context) {
		return iclient_persistence::persistence_open(handle, clientID, serverURI, context);
	}

	static int persistence_close(void* handle) {
		return iclient_persistence::persistence_close(handle);
	}

	static int persistence_put(void* handle, char* key, int bufcount, char* buffers[], int buflens[]) {
		return iclient_persistence::persistence_put(handle, key, bufcount, buffers, buflens);
	}

	static int persistence_get(void* handle, char* key, char** buffer, int* buflen) {
		return iclient_persistence::persistence_get(handle, key, buffer, buflen);
	}

	static int persistence_remove(void* handle, char* key) {
		return iclient_persistence::persistence_remove(handle, key);
	}

	static int persistence_keys(void* handle, char*** keys, int* nkeys) {
		return iclient_persistence::persistence_keys(handle, keys, nkeys);
	}

	static int persistence_clear(void* handle) {
		return iclient_persistence::persistence_clear(handle);
	}

	static int persistence_containskey(void* handle, char* key) {
		return iclient_persistence::persistence_containskey(handle, key);
	}

public:
	void SetUp() {
		void* context = static_cast<void*>(&per_);
		dcp::persistence_open(&handle_, CLIENT_ID, SERVER_URI, context);

		// Put no buffer
		int bufcount = 3;
		const char* bufs[] = { PAYLOAD, PAYLOAD2, PAYLOAD3 };
		int buflens[] = { int(PAYLOAD_LEN), int(PAYLOAD2_LEN), int(PAYLOAD3_LEN) };

		dcp::persistence_put(handle_, const_cast<char*>(KEY), bufcount,
							 const_cast<char**>(bufs), buflens);
	}
	void TearDown() {
		dcp::persistence_clear(handle_);
		dcp::persistence_close(handle_);
	}
};

// ----------------------------------------------------------------------
// Test static method persistence_open()
// ----------------------------------------------------------------------

TEST_F(iclient_persistence_test, test_persistence_open) {
	dcp per;
	void* handle = nullptr;

	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_open(&handle, CLIENT_ID, SERVER_URI, nullptr));

	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per));
	EXPECT_EQ(handle, static_cast<void*>(&per));
}

// ----------------------------------------------------------------------
// Test static method persistence_close()
// ----------------------------------------------------------------------

TEST_F(iclient_persistence_test, test_persistence_close) {
	dcp per;

	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_close(nullptr));

	void* context = static_cast<void*>(&per);
	void* handle = nullptr;
	cpt::persistence_open(&handle, CLIENT_ID, SERVER_URI, context);

	EXPECT_EQ(MQTTASYNC_SUCCESS, cpt::persistence_close(handle));
}

// ----------------------------------------------------------------------
// Test static method persistence_put()
// ----------------------------------------------------------------------

TEST_F(iclient_persistence_test, test_persistence_put_0_buffer) {
	dcp per;
	void* handle;
	cpt::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per);

	// Put no buffer
	int bufcount = 0;
	const char* bufs[] = { PAYLOAD };
	int buflens[] = { int(PAYLOAD_LEN) };

	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_put(handle, const_cast<char*>(KEY), bufcount,
			const_cast<char**>(bufs), buflens));
}

TEST_F(iclient_persistence_test, test_persistence_put_1_buffer) {
	dcp per;
	void* handle;
	cpt::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per);

	// Put no buffer
	int bufcount = 1;
	const char* bufs[] = { PAYLOAD };
	int buflens[] = { int(PAYLOAD_LEN) };

	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_put(handle, const_cast<char*>(KEY), bufcount,
			const_cast<char**>(bufs), buflens));
	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_containskey(handle, const_cast<char*>(KEY)));
	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_containskey(handle, const_cast<char*>(INVALID_KEY)));
}

TEST_F(iclient_persistence_test, test_persistence_put_2_buffers) {
	dcp per;
	void* handle;
	cpt::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per);

	// Put no buffer
	int bufcount = 2;
	const char* bufs[] = { PAYLOAD, PAYLOAD2 };
	int buflens[] = { int(PAYLOAD_LEN), int(PAYLOAD2_LEN) };

	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_put(handle, const_cast<char*>(KEY), bufcount,
					const_cast<char**>(bufs), buflens));
	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_containskey(handle, const_cast<char*>(KEY)));
	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_containskey(handle, const_cast<char*>(INVALID_KEY)));
}

TEST_F(iclient_persistence_test, test_persistence_put_3_buffers) {
	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_containskey(handle_, const_cast<char*>(KEY)));
	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_containskey(handle_, const_cast<char*>(INVALID_KEY)));
}

TEST_F(iclient_persistence_test, test_persistence_put_empty_buffers) {
	dcp per;
	void* handle;
	cpt::persistence_open(&handle, CLIENT_ID, SERVER_URI, &per);

	// Put three empty buffers
	int bufcount = 3;
	const char* buffers[] = { "", "", "" };
	int buflens[] = { 0, 0, 0 };
	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_put(handle, const_cast<char*>(KEY),
			bufcount, const_cast<char**>(buffers), buflens));
}

// ----------------------------------------------------------------------
// Test static method persistence_get()
// ----------------------------------------------------------------------

TEST_F(iclient_persistence_test, test_persistence_get) {
	char* buf = nullptr;
	int buflen = 0;
	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_get(handle_, const_cast<char*>(INVALID_KEY), &buf, &buflen));
	EXPECT_EQ(0, buflen);

	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_get(handle_, const_cast<char*>(KEY), &buf, &buflen));

	int n = PAYLOAD_LEN + PAYLOAD2_LEN + PAYLOAD3_LEN;
	string str { PAYLOAD };
	str += PAYLOAD2;
	str += PAYLOAD3;

	EXPECT_EQ(n, buflen);
	EXPECT_NE(nullptr, buf);
	EXPECT_EQ(0, memcmp(str.data(), buf, n));
}

// ----------------------------------------------------------------------
// Test static method persistence_remove()
// ----------------------------------------------------------------------

TEST_F(iclient_persistence_test, test_persistence_remove) {
	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_remove(handle_, const_cast<char*>(INVALID_KEY)));
	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_containskey(handle_, const_cast<char*>(KEY)));
	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_remove(handle_, const_cast<char*>(KEY)));
	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_containskey(handle_, const_cast<char*>(KEY)));
}

// ----------------------------------------------------------------------
// Test static method persistence_keys()
// ----------------------------------------------------------------------

TEST_F(iclient_persistence_test, test_persistence_keys) {
		char** keys = nullptr;
		int nkeys = 0;

		EXPECT_EQ(MQTTASYNC_SUCCESS,
							 cpt::persistence_keys(handle_, &keys, &nkeys));
		EXPECT_EQ(1, nkeys);
		EXPECT_EQ(std::string(KEY), std::string(keys[0]));
		//CPPUNIT_ASSERT(!strcmp(KEY, keys[0]));
	}

// ----------------------------------------------------------------------
// Test static method persistence_clear()
// ----------------------------------------------------------------------

TEST_F(iclient_persistence_test, test_persistence_clear) {
	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_containskey(handle_, const_cast<char*>(KEY)));
	EXPECT_EQ(MQTTASYNC_SUCCESS, cpt::persistence_clear(handle_));
	EXPECT_EQ(MQTTCLIENT_PERSISTENCE_ERROR,
			cpt::persistence_containskey(handle_, const_cast<char*>(KEY)));

	char** keys = nullptr;
	int nkeys = -1;

	EXPECT_EQ(MQTTASYNC_SUCCESS,
			cpt::persistence_keys(handle_, &keys, &nkeys));
	EXPECT_EQ(0, nkeys);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_iclient_persistence_test_h

