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
 *    Guilherme M. Ferreira - added more tests
 *    Guilherme M. Ferreira - changed test framework from CppUnit to GTest
 *******************************************************************************/

#ifndef __mqtt_connect_options_test_h
#define __mqtt_connect_options_test_h

#include <gtest/gtest.h>

#include "mqtt/connect_options.h"

#include "dummy_async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class connect_options_test : public ::testing::Test
{
protected:
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

	MQTTAsync_connectOptions& get_c_struct(mqtt::connect_options& opts) {
		return opts.opts_;
	}

	const MQTTAsync_willOptions& get_c_struct(const mqtt::will_options& opts) {
		return opts.opts_;
	}

	const MQTTAsync_SSLOptions& get_c_struct(const mqtt::ssl_options& opts) {
		return opts.opts_;
	}

public:
	void SetUp() {}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_dflt_constructor) {
	mqtt::connect_options opts;
	EXPECT_EQ(EMPTY_STR, opts.get_user_name());
	EXPECT_EQ(EMPTY_STR, opts.get_password_str());
	EXPECT_EQ(DFLT_KEEP_ALIVE, (int) opts.get_keep_alive_interval().count());
	EXPECT_EQ(DFLT_CONNECT_TIMEOUT, (int) opts.get_connect_timeout().count());
	EXPECT_FALSE(opts.get_servers());
	EXPECT_EQ(DFLT_AUTO_RECONNECT, opts.get_automatic_reconnect());

	const auto& c_struct = get_c_struct(opts);
	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

	EXPECT_EQ(DFLT_KEEP_ALIVE, c_struct.keepAliveInterval);
	EXPECT_EQ(DFLT_CONNECT_TIMEOUT, c_struct.connectTimeout);

	EXPECT_EQ(nullptr, c_struct.username);
	EXPECT_EQ(nullptr, c_struct.password);
	EXPECT_EQ(0, c_struct.binarypwd.len);
	EXPECT_EQ(nullptr, c_struct.binarypwd.data);

	// No callbacks without a context token
	EXPECT_EQ(nullptr, c_struct.context);
	EXPECT_EQ(nullptr, c_struct.onSuccess);
	EXPECT_EQ(nullptr, c_struct.onFailure);

	// No will or SSL, for default
	EXPECT_EQ(nullptr, c_struct.will);
	EXPECT_EQ(nullptr, c_struct.ssl);

	EXPECT_EQ(0, c_struct.serverURIcount);
	EXPECT_EQ(nullptr, c_struct.serverURIs);
}

// ----------------------------------------------------------------------
// Test the constructor that takes user/password
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_user_constructor) {
	mqtt::connect_options opts { USER, PASSWD };

	const auto& c_struct = get_c_struct(opts);
	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());

	EXPECT_STRCASEEQ(USER.c_str(), c_struct.username);
	EXPECT_EQ(nullptr, c_struct.password);
	EXPECT_EQ(PASSWD.size(), size_t(c_struct.binarypwd.len));
	EXPECT_EQ(0, memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

	// No callbacks without a context token
	EXPECT_EQ(nullptr, c_struct.context);
	EXPECT_EQ(nullptr, c_struct.onSuccess);
	EXPECT_EQ(nullptr, c_struct.onFailure);
}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_copy_constructor) {
	mqtt::connect_options orgOpts { USER, PASSWD };
	mqtt::connect_options opts { orgOpts };

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());

	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

	EXPECT_STRCASEEQ(USER.c_str(), c_struct.username);
	EXPECT_EQ(nullptr, c_struct.password);
	EXPECT_EQ(PASSWD.size(), size_t(c_struct.binarypwd.len));
	EXPECT_EQ(0, memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_user_name(EMPTY_STR);
	orgOpts.set_password(EMPTY_STR);

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());
}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_move_constructor) {
	mqtt::connect_options orgOpts { USER, PASSWD };
	mqtt::connect_options opts { std::move(orgOpts) };

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());

	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

	EXPECT_STRCASEEQ(USER.c_str(), c_struct.username);
	EXPECT_EQ(nullptr, c_struct.password);
	EXPECT_EQ(PASSWD.size(), size_t(c_struct.binarypwd.len));
	EXPECT_EQ(0, memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_user_name(EMPTY_STR);
	orgOpts.set_password(EMPTY_STR);

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());

	// Check that the original was moved
	EXPECT_EQ(EMPTY_STR, orgOpts.get_user_name());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_password_str());
}

// ----------------------------------------------------------------------
// Test the copy assignment operator=(const&)
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_copy_assignment) {
	mqtt::connect_options orgOpts { USER, PASSWD };
	mqtt::connect_options opts;

	opts = orgOpts;

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());

	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

	EXPECT_STRCASEEQ(USER.c_str(), c_struct.username);
	EXPECT_EQ(nullptr, c_struct.password);
	EXPECT_EQ(PASSWD.size(), size_t(c_struct.binarypwd.len));
	EXPECT_EQ(0, memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_user_name(EMPTY_STR);
	orgOpts.set_password(EMPTY_STR);

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());

	// Self assignment should cause no harm
	opts = opts;

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());
}

// ----------------------------------------------------------------------
// Test the move assignment, operator=(&&)
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_move_assignment) {
	mqtt::connect_options orgOpts { USER, PASSWD };
	mqtt::connect_options opts { std::move(orgOpts) };

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());

	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));

	EXPECT_STRCASEEQ(USER.c_str(), c_struct.username);
	EXPECT_EQ(nullptr, c_struct.password);
	EXPECT_EQ(PASSWD.size(), size_t(c_struct.binarypwd.len));
	EXPECT_EQ(0, memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_user_name(EMPTY_STR);
	orgOpts.set_password(EMPTY_STR);

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());

	// Check that the original was moved
	EXPECT_EQ(EMPTY_STR, orgOpts.get_user_name());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_password_str());

	// Self assignment should cause no harm
	// (clang++ is smart enough to warn about this)
	#if !defined(__clang__)
		opts = std::move(opts);
		EXPECT_EQ(USER, opts.get_user_name());
		EXPECT_EQ(PASSWD, opts.get_password_str());
	#endif
}

// ----------------------------------------------------------------------
// Test set/get of the user and password.
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_set_user) {
	mqtt::connect_options opts;
	const auto& c_struct = get_c_struct(opts);

	opts.set_user_name(USER);
	opts.set_password(PASSWD);

	EXPECT_EQ(USER, opts.get_user_name());
	EXPECT_EQ(PASSWD, opts.get_password_str());

	EXPECT_STRCASEEQ(USER.c_str(), c_struct.username);
	EXPECT_EQ(nullptr, c_struct.password);
	EXPECT_EQ(PASSWD.size(), size_t(c_struct.binarypwd.len));
	EXPECT_EQ(0, memcmp(PASSWD.data(), c_struct.binarypwd.data, PASSWD.size()));
}

// ----------------------------------------------------------------------
// Test set/get of a long user name and password.
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_set_long_user) {
	std::string user;
	std::string passwd;
	for (int i=0; i<1053; ++i) {
		if (isprint(char(i))) user.push_back(char(i));
		passwd.push_back(byte(i));
	}

	mqtt::connect_options orgOpts;

	orgOpts.set_user_name(user);
	orgOpts.set_password(passwd);

	EXPECT_EQ(user, orgOpts.get_user_name());
	EXPECT_EQ(passwd, orgOpts.get_password_str());

	mqtt::connect_options opts;
	opts = orgOpts;

	EXPECT_EQ(user, opts.get_user_name());
	EXPECT_EQ(passwd, opts.get_password_str());

	const auto& c_struct = get_c_struct(opts);

	EXPECT_STRCASEEQ(user.c_str(), c_struct.username);
	EXPECT_EQ(nullptr, c_struct.password);
	EXPECT_EQ(passwd.size(), size_t(c_struct.binarypwd.len));
	EXPECT_EQ(0, memcmp(passwd.data(), c_struct.binarypwd.data, PASSWD.size()));
}

// ----------------------------------------------------------------------
// Test set/get of will options
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_set_will) {
	mqtt::connect_options opts;
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(nullptr, c_struct.will);
	mqtt::will_options willOpts;
	opts.set_will(willOpts);
	EXPECT_NE(nullptr, c_struct.will);
	EXPECT_EQ(&get_c_struct(opts.get_will_options()), c_struct.will);
}

// ----------------------------------------------------------------------
// Test set/get of ssl options
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_set_ssl) {
	mqtt::connect_options opts;
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(nullptr, c_struct.ssl);
	mqtt::ssl_options sslOpts;
	opts.set_ssl(sslOpts);
	EXPECT_NE(nullptr, c_struct.ssl);
	EXPECT_EQ(&get_c_struct(opts.get_ssl_options()), c_struct.ssl);
}

// ----------------------------------------------------------------------
// Test set/get of context token
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_set_token) {
	mqtt::connect_options opts;
	const auto& c_struct = get_c_struct(opts);

	EXPECT_EQ(nullptr, c_struct.context);
	mqtt::test::dummy_async_client ac;
	auto tok = token::create(ac);
	opts.set_token(tok);
	EXPECT_EQ(tok, opts.get_token());
	EXPECT_EQ(tok.get(), c_struct.context);
}

// ----------------------------------------------------------------------
// Test set/get of keep alive interval
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_set_keep_alive) {
	mqtt::connect_options opts;
	const auto& c_struct = get_c_struct(opts);

	// Set as an int
	const int KEEP_ALIVE_SEC = 30;
	opts.set_keep_alive_interval(KEEP_ALIVE_SEC);

	EXPECT_EQ(KEEP_ALIVE_SEC, (int) opts.get_keep_alive_interval().count());
	EXPECT_EQ(KEEP_ALIVE_SEC, c_struct.keepAliveInterval);

	// Set as an chrono
	opts.set_keep_alive_interval(std::chrono::milliseconds(2*KEEP_ALIVE_SEC*1000));

	EXPECT_EQ(2*KEEP_ALIVE_SEC, (int) opts.get_keep_alive_interval().count());
	EXPECT_EQ(2*KEEP_ALIVE_SEC, c_struct.keepAliveInterval);
}

// ----------------------------------------------------------------------
// Test set/get of connect timeout
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_set_connect_timeout) {
	mqtt::connect_options opts;
	const auto& c_struct = get_c_struct(opts);

	// Set as an int
	const int TIMEOUT_SEC = 10;
	opts.set_connect_timeout(TIMEOUT_SEC);

	EXPECT_EQ(TIMEOUT_SEC, (int) opts.get_connect_timeout().count());
	EXPECT_EQ(TIMEOUT_SEC, c_struct.connectTimeout);

	// Set as an chrono
	opts.set_connect_timeout(std::chrono::milliseconds(2*TIMEOUT_SEC*1000));

	EXPECT_EQ(2*TIMEOUT_SEC, (int) opts.get_connect_timeout().count());
	EXPECT_EQ(2*TIMEOUT_SEC, c_struct.connectTimeout);
}

// ----------------------------------------------------------------------
// Test set/get of server URIs
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_set_servers) {
	mqtt::connect_options opts;
	const auto& c_struct = get_c_struct(opts);

	opts.set_servers(URIs);

	EXPECT_EQ(URIs.get(), opts.get_servers().get());

	// Check the C struct
	EXPECT_EQ((int) URIsVec.size(), c_struct.serverURIcount);
	EXPECT_STRCASEEQ(URIsVec[0].c_str(), c_struct.serverURIs[0]);
	EXPECT_STRCASEEQ(URIsVec[1].c_str(), c_struct.serverURIs[1]);
	EXPECT_STRCASEEQ(URIsVec[2].c_str(), c_struct.serverURIs[2]);
}

// ----------------------------------------------------------------------
// Test set/get of the auto reconnect values
// ----------------------------------------------------------------------

TEST_F(connect_options_test, test_set_auto_reconnect) {
	mqtt::connect_options opts;
	const auto& c_struct = get_c_struct(opts);

	// Set as an int
	const int TIMEOUT_SEC = 10;
	opts.set_automatic_reconnect(TIMEOUT_SEC, 2*TIMEOUT_SEC);

	EXPECT_TRUE(opts.get_automatic_reconnect());
	EXPECT_EQ(TIMEOUT_SEC, (int) opts.get_min_retry_interval().count());
	EXPECT_EQ(2*TIMEOUT_SEC, (int) opts.get_max_retry_interval().count());

	EXPECT_EQ(TIMEOUT_SEC, c_struct.minRetryInterval);
	EXPECT_EQ(2*TIMEOUT_SEC, c_struct.maxRetryInterval);

	// Set as an chrono
	opts.set_automatic_reconnect(std::chrono::milliseconds(2000*TIMEOUT_SEC),
								 std::chrono::milliseconds(4000*TIMEOUT_SEC));

	EXPECT_TRUE(opts.get_automatic_reconnect());
	EXPECT_EQ(2*TIMEOUT_SEC, (int) opts.get_min_retry_interval().count());
	EXPECT_EQ(4*TIMEOUT_SEC, (int) opts.get_max_retry_interval().count());

	EXPECT_EQ(2*TIMEOUT_SEC, c_struct.minRetryInterval);
	EXPECT_EQ(4*TIMEOUT_SEC, c_struct.maxRetryInterval);

	// Turn it off
	opts.set_automatic_reconnect(false);
	EXPECT_FALSE(opts.get_automatic_reconnect());
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_connect_options_test_h
