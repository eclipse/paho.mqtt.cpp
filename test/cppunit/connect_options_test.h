// connect_options_test.h
// Unit tests for the connect_options class in the Paho MQTT C++ library.

/*******************************************************************************
 * Copyright (c) 2016 Frank Pagliughi <fpagliughi@mindspring.com>
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
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_connect_options_test_h
#define __mqtt_connect_options_test_h

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "mqtt/connect_options.h"

#include "dummy_async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class connect_options_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( connect_options_test );

	CPPUNIT_TEST( test_dflt_constructor );
	CPPUNIT_TEST( test_user_constructor );
	CPPUNIT_TEST( test_copy_constructor );
	CPPUNIT_TEST( test_move_constructor );
	CPPUNIT_TEST( test_copy_assignment );
	CPPUNIT_TEST( test_move_assignment );
	CPPUNIT_TEST( test_set_user );
	CPPUNIT_TEST( test_set_long_user );
	CPPUNIT_TEST( test_set_will );
	CPPUNIT_TEST( test_set_ssl );
	CPPUNIT_TEST( test_set_token );
	CPPUNIT_TEST( test_set_keep_alive );
	CPPUNIT_TEST( test_set_connect_timeout );
	CPPUNIT_TEST( test_set_servers );
	CPPUNIT_TEST( test_set_auto_reconnect );

	CPPUNIT_TEST_SUITE_END();

	// C struct signature/eyecatcher
	const char* CSIG = "MQTC";
	const size_t CSIG_LEN = std::strlen(CSIG);

	// These must match the C init struct
	const int DFLT_KEEP_ALIVE = 60;
	const int DFLT_CONNECT_TIMEOUT = 30;
	const bool DFLT_AUTO_RECONNECT = false;

	const std::string USER { "wally" };
	const std::string PASSWD { "xyzpdq" };

	const std::string EMPTY_STR;

	const std::vector<string> URIsVec = {
		"tcp://server1:1883",
		"tcp://server2:1883",
		"ssl://server3:8883"
	};
	const const_string_collection_ptr URIs = std::make_shared<const string_collection>(URIsVec);

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

	void test_dflt_constructor() {
		mqtt::connect_options opts;
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, opts.get_password_str());
		CPPUNIT_ASSERT_EQUAL(DFLT_KEEP_ALIVE, (int) opts.get_keep_alive_interval().count());
		CPPUNIT_ASSERT_EQUAL(DFLT_CONNECT_TIMEOUT, (int) opts.get_connect_timeout().count());
		CPPUNIT_ASSERT(!opts.get_servers());
		CPPUNIT_ASSERT_EQUAL(DFLT_AUTO_RECONNECT, opts.get_automatic_reconnect());

		const auto& c_struct = opts.opts_;
		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT_EQUAL(DFLT_KEEP_ALIVE, c_struct.keepAliveInterval);
		CPPUNIT_ASSERT_EQUAL(DFLT_CONNECT_TIMEOUT, c_struct.connectTimeout);

		CPPUNIT_ASSERT(c_struct.username == nullptr);
		CPPUNIT_ASSERT(c_struct.password == nullptr);
		CPPUNIT_ASSERT_EQUAL(0, c_struct.binarypwd.len);
		CPPUNIT_ASSERT(c_struct.binarypwd.data == nullptr);

		// No callbacks without a context token
		CPPUNIT_ASSERT(c_struct.context == nullptr);
		CPPUNIT_ASSERT(c_struct.onSuccess == nullptr);
		CPPUNIT_ASSERT(c_struct.onFailure == nullptr);

		// No will or SSL, for default
		CPPUNIT_ASSERT(c_struct.will == nullptr);
		CPPUNIT_ASSERT(c_struct.ssl == nullptr);

		CPPUNIT_ASSERT_EQUAL(0, c_struct.serverURIcount);
		CPPUNIT_ASSERT(nullptr == c_struct.serverURIs);
	}

// ----------------------------------------------------------------------
// Test the constructor that takes user/password
// ----------------------------------------------------------------------

	void test_user_constructor() {
		mqtt::connect_options opts { USER, PASSWD };

		const auto& c_struct = opts.opts_;
		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(c_struct.password == nullptr);
		CPPUNIT_ASSERT_EQUAL(PASSWD.size(), size_t(c_struct.binarypwd.len));
		CPPUNIT_ASSERT(!memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

		// No callbacks without a context token
		CPPUNIT_ASSERT(c_struct.context == nullptr);
		CPPUNIT_ASSERT(c_struct.onSuccess == nullptr);
		CPPUNIT_ASSERT(c_struct.onFailure == nullptr);
	}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

	void test_copy_constructor() {
		mqtt::connect_options orgOpts { USER, PASSWD };
		mqtt::connect_options opts { orgOpts };

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(c_struct.password == nullptr);
		CPPUNIT_ASSERT_EQUAL(PASSWD.size(), size_t(c_struct.binarypwd.len));
		CPPUNIT_ASSERT(!memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_user_name(EMPTY_STR);
		orgOpts.set_password(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());
	}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

	void test_move_constructor() {
		mqtt::connect_options orgOpts { USER, PASSWD };
		mqtt::connect_options opts { std::move(orgOpts) };

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(c_struct.password == nullptr);
		CPPUNIT_ASSERT_EQUAL(PASSWD.size(), size_t(c_struct.binarypwd.len));
		CPPUNIT_ASSERT(!memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_user_name(EMPTY_STR);
		orgOpts.set_password(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_password_str());
	}

// ----------------------------------------------------------------------
// Test the copy assignment operator=(const&)
// ----------------------------------------------------------------------

	void test_copy_assignment() {
		mqtt::connect_options orgOpts { USER, PASSWD };
		mqtt::connect_options opts;

		opts = orgOpts;

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(c_struct.password == nullptr);
		CPPUNIT_ASSERT_EQUAL(PASSWD.size(), size_t(c_struct.binarypwd.len));
		CPPUNIT_ASSERT(!memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_user_name(EMPTY_STR);
		orgOpts.set_password(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());

		// Self assignment should cause no harm
		opts = opts;

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());
	}

// ----------------------------------------------------------------------
// Test the move assignment, operator=(&&)
// ----------------------------------------------------------------------

	void test_move_assignment() {
		mqtt::connect_options orgOpts { USER, PASSWD };
		mqtt::connect_options opts { std::move(orgOpts) };

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(c_struct.password == nullptr);
		CPPUNIT_ASSERT_EQUAL(PASSWD.size(), size_t(c_struct.binarypwd.len));
		CPPUNIT_ASSERT(!memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

		// Make sure it's a true copy, not linked to the original
		orgOpts.set_user_name(EMPTY_STR);
		orgOpts.set_password(EMPTY_STR);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());

		// Check that the original was moved
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, orgOpts.get_password_str());

		// Self assignment should cause no harm
		// (clang++ is smart enough to warn about this)
		#if !defined(__clang__)
			opts = std::move(opts);
			CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
			CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());
		#endif
	}


// ----------------------------------------------------------------------
// Test set/get of the user and password.
// ----------------------------------------------------------------------

	void test_set_user() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		opts.set_user_name(USER);
		opts.set_password(PASSWD);

		CPPUNIT_ASSERT_EQUAL(USER, opts.get_user_name());
		CPPUNIT_ASSERT_EQUAL(PASSWD, opts.get_password_str());

		CPPUNIT_ASSERT(!strcmp(USER.c_str(), c_struct.username));
		CPPUNIT_ASSERT(c_struct.password == nullptr);
		CPPUNIT_ASSERT_EQUAL(PASSWD.size(), size_t(c_struct.binarypwd.len));
		CPPUNIT_ASSERT(!memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));
	}

// ----------------------------------------------------------------------
// Test set/get of a long user name and password.
// ----------------------------------------------------------------------

	void test_set_long_user() {
		std::string user;
		std::string passwd;
		for (int i=0; i<1053; ++i) {
			if (isprint(char(i))) user.push_back(char(i));
			passwd.push_back(byte(i));
		}

		mqtt::connect_options orgOpts;

		orgOpts.set_user_name(user);
		orgOpts.set_password(passwd);

		CPPUNIT_ASSERT_EQUAL(user, orgOpts.get_user_name());
		CPPUNIT_ASSERT(passwd == orgOpts.get_password_str());

		mqtt::connect_options opts;
		opts = orgOpts;

		CPPUNIT_ASSERT_EQUAL(user, opts.get_user_name());
		CPPUNIT_ASSERT(passwd == opts.get_password_str());

		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(!strcmp(user.c_str(), c_struct.username));
		CPPUNIT_ASSERT(c_struct.password == nullptr);
		CPPUNIT_ASSERT_EQUAL(passwd.size(), size_t(c_struct.binarypwd.len));
		CPPUNIT_ASSERT(!memcmp(passwd.data(), c_struct.binarypwd.data, PASSWD.size()));
	}

// ----------------------------------------------------------------------
// Test set/get of will options
// ----------------------------------------------------------------------

	void test_set_will() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(nullptr == c_struct.will);
		mqtt::will_options willOpts;
		opts.set_will(willOpts);
		CPPUNIT_ASSERT(nullptr != c_struct.will);
		CPPUNIT_ASSERT_EQUAL(&opts.will_.opts_, c_struct.will);
	}

// ----------------------------------------------------------------------
// Test set/get of ssl options
// ----------------------------------------------------------------------

	void test_set_ssl() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(nullptr == c_struct.ssl);
		mqtt::ssl_options sslOpts;
		opts.set_ssl(sslOpts);
		CPPUNIT_ASSERT(nullptr != c_struct.ssl);
		CPPUNIT_ASSERT_EQUAL(&opts.ssl_.opts_, c_struct.ssl);
	}

// ----------------------------------------------------------------------
// Test set/get of context token
// ----------------------------------------------------------------------

	void test_set_token() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		CPPUNIT_ASSERT(nullptr == c_struct.context);
		mqtt::test::dummy_async_client ac;
		auto tok = token::create(ac);
		opts.set_token(tok);
		CPPUNIT_ASSERT_EQUAL(tok, opts.get_token());
		CPPUNIT_ASSERT(tok.get() == c_struct.context);
	}

// ----------------------------------------------------------------------
// Test set/get of keep alive interval
// ----------------------------------------------------------------------

	void test_set_keep_alive() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		// Set as an int
		const int KEEP_ALIVE_SEC = 30;
		opts.set_keep_alive_interval(KEEP_ALIVE_SEC);

		CPPUNIT_ASSERT_EQUAL(KEEP_ALIVE_SEC, (int) opts.get_keep_alive_interval().count());
		CPPUNIT_ASSERT_EQUAL(KEEP_ALIVE_SEC, c_struct.keepAliveInterval);

		// Set as an chrono
		opts.set_keep_alive_interval(std::chrono::milliseconds(2*KEEP_ALIVE_SEC*1000));

		CPPUNIT_ASSERT_EQUAL(2*KEEP_ALIVE_SEC, (int) opts.get_keep_alive_interval().count());
		CPPUNIT_ASSERT_EQUAL(2*KEEP_ALIVE_SEC, c_struct.keepAliveInterval);
	}

// ----------------------------------------------------------------------
// Test set/get of connect timeout
// ----------------------------------------------------------------------

	void test_set_connect_timeout() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		// Set as an int
		const int TIMEOUT_SEC = 10;
		opts.set_connect_timeout(TIMEOUT_SEC);

		CPPUNIT_ASSERT_EQUAL(TIMEOUT_SEC, (int) opts.get_connect_timeout().count());
		CPPUNIT_ASSERT_EQUAL(TIMEOUT_SEC, c_struct.connectTimeout);

		// Set as an chrono
		opts.set_connect_timeout(std::chrono::milliseconds(2*TIMEOUT_SEC*1000));

		CPPUNIT_ASSERT_EQUAL(2*TIMEOUT_SEC, (int) opts.get_connect_timeout().count());
		CPPUNIT_ASSERT_EQUAL(2*TIMEOUT_SEC, c_struct.connectTimeout);
	}

// ----------------------------------------------------------------------
// Test set/get of server URIs
// ----------------------------------------------------------------------

	void test_set_servers() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		opts.set_servers(URIs);

		CPPUNIT_ASSERT_EQUAL(URIs.get(), opts.get_servers().get());

		// Check the C struct
		CPPUNIT_ASSERT_EQUAL((int) URIsVec.size(), c_struct.serverURIcount);
		CPPUNIT_ASSERT(!strcmp(URIsVec[0].c_str(), c_struct.serverURIs[0]));
		CPPUNIT_ASSERT(!strcmp(URIsVec[1].c_str(), c_struct.serverURIs[1]));
		CPPUNIT_ASSERT(!strcmp(URIsVec[2].c_str(), c_struct.serverURIs[2]));
	}

// ----------------------------------------------------------------------
// Test set/get of the auto reconnect values
// ----------------------------------------------------------------------

	void test_set_auto_reconnect() {
		mqtt::connect_options opts;
		const auto& c_struct = opts.opts_;

		// Set as an int
		const int TIMEOUT_SEC = 10;
		opts.set_automatic_reconnect(TIMEOUT_SEC, 2*TIMEOUT_SEC);

		CPPUNIT_ASSERT_EQUAL(true, opts.get_automatic_reconnect());
		CPPUNIT_ASSERT_EQUAL(TIMEOUT_SEC, (int) opts.get_min_retry_interval().count());
		CPPUNIT_ASSERT_EQUAL(2*TIMEOUT_SEC, (int) opts.get_max_retry_interval().count());

		CPPUNIT_ASSERT_EQUAL(TIMEOUT_SEC, c_struct.minRetryInterval);
		CPPUNIT_ASSERT_EQUAL(2*TIMEOUT_SEC, c_struct.maxRetryInterval);

		// Set as an chrono
		opts.set_automatic_reconnect(std::chrono::milliseconds(2000*TIMEOUT_SEC),
									 std::chrono::milliseconds(4000*TIMEOUT_SEC));

		CPPUNIT_ASSERT_EQUAL(true, opts.get_automatic_reconnect());
		CPPUNIT_ASSERT_EQUAL(2*TIMEOUT_SEC, (int) opts.get_min_retry_interval().count());
		CPPUNIT_ASSERT_EQUAL(4*TIMEOUT_SEC, (int) opts.get_max_retry_interval().count());

		CPPUNIT_ASSERT_EQUAL(2*TIMEOUT_SEC, c_struct.minRetryInterval);
		CPPUNIT_ASSERT_EQUAL(4*TIMEOUT_SEC, c_struct.maxRetryInterval);

		// Turn it off
		opts.set_automatic_reconnect(false);
		CPPUNIT_ASSERT_EQUAL(false, opts.get_automatic_reconnect());
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_connect_options_test_h

