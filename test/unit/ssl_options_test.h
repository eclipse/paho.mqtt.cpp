// ssl_options_test.h
// Unit tests for the ssl_options class in the Paho MQTT C++ library.

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
 *    Guilherme M. Ferreira - add more test cases
 *    Guilherme M. Ferreira - changed test framework from CppUnit to GTest
 *******************************************************************************/

#ifndef __mqtt_ssl_options_test_h
#define __mqtt_ssl_options_test_h

#include <gtest/gtest.h>

#include "mqtt/ssl_options.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class ssl_options_test : public ::testing::Test
{
protected:
	// C struct signature/eyecatcher
	const char* CSIG = "MQTS";
	const size_t CSIG_LEN = std::strlen(CSIG);

	const bool DFLT_SERVER_CERT = true;

	const std::string EMPTY_STR;
	const std::string TRUST_STORE { "trust store" };
	const std::string KEY_STORE { "key store" };
	const std::string PRIVATE_KEY { "private key" };
	const std::string PRIVATE_KEY_PASSWORD { "private key password" };
	const std::string ENABLED_CIPHER_SUITES { "cipher suite" };
	const bool SERVER_CERT { false };

	mqtt::ssl_options orgOpts;

	MQTTAsync_SSLOptions& get_c_struct(mqtt::ssl_options& opts) {
		return opts.opts_;
	}

public:
	void SetUp() {
		orgOpts = mqtt::ssl_options{
			TRUST_STORE,
			KEY_STORE,
			PRIVATE_KEY,
			PRIVATE_KEY_PASSWORD,
			ENABLED_CIPHER_SUITES,
			SERVER_CERT};
	}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

TEST_F(ssl_options_test, test_dflt_constructor) {
	mqtt::ssl_options opts;

	EXPECT_EQ(EMPTY_STR, opts.get_trust_store());
	EXPECT_EQ(EMPTY_STR, opts.get_key_store());
	EXPECT_EQ(EMPTY_STR, opts.get_private_key());
	EXPECT_EQ(EMPTY_STR, opts.get_private_key_password());
	EXPECT_EQ(DFLT_SERVER_CERT, opts.get_enable_server_cert_auth());

	// Make sure the empty string represents a nullptr for C library
	const MQTTAsync_SSLOptions& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_EQ(nullptr, c_struct.trustStore);
	EXPECT_EQ(nullptr, c_struct.keyStore);
	EXPECT_EQ(nullptr, c_struct.privateKey);
	EXPECT_EQ(nullptr, c_struct.privateKeyPassword);
	EXPECT_EQ(nullptr, c_struct.enabledCipherSuites);
	EXPECT_EQ(DFLT_SERVER_CERT, c_struct.enableServerCertAuth != 0);
}

// ----------------------------------------------------------------------
// Test the constructor that takes user arguments
// ----------------------------------------------------------------------

TEST_F(ssl_options_test, test_user_constructor) {
	mqtt::ssl_options opts { TRUST_STORE, KEY_STORE, PRIVATE_KEY,
		PRIVATE_KEY_PASSWORD, ENABLED_CIPHER_SUITES, SERVER_CERT };

	EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
	EXPECT_EQ(KEY_STORE, opts.get_key_store());
	EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
	EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
	EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
	EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());

	// Check the underlying C struct
	const MQTTAsync_SSLOptions& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TRUST_STORE.c_str(), c_struct.trustStore);
	EXPECT_STRCASEEQ(KEY_STORE.c_str(), c_struct.keyStore);
	EXPECT_STRCASEEQ(PRIVATE_KEY.c_str(), c_struct.privateKey);
	EXPECT_STRCASEEQ(PRIVATE_KEY_PASSWORD.c_str(), c_struct.privateKeyPassword);
	EXPECT_STRCASEEQ(ENABLED_CIPHER_SUITES.c_str(), c_struct.enabledCipherSuites);
	EXPECT_EQ(SERVER_CERT, c_struct.enableServerCertAuth != 0);
}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

TEST_F(ssl_options_test, test_copy_constructor) {
	mqtt::ssl_options opts{orgOpts};

	EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
	EXPECT_EQ(KEY_STORE, opts.get_key_store());
	EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
	EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
	EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
	EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());

	// Check the underlying C struct
	const MQTTAsync_SSLOptions& c_struct = get_c_struct(opts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_STRCASEEQ(TRUST_STORE.c_str(), c_struct.trustStore);
	EXPECT_STRCASEEQ(KEY_STORE.c_str(), c_struct.keyStore);
	EXPECT_STRCASEEQ(PRIVATE_KEY.c_str(), c_struct.privateKey);
	EXPECT_STRCASEEQ(PRIVATE_KEY_PASSWORD.c_str(), c_struct.privateKeyPassword);
	EXPECT_STRCASEEQ(ENABLED_CIPHER_SUITES.c_str(), c_struct.enabledCipherSuites);
	EXPECT_EQ(SERVER_CERT, c_struct.enableServerCertAuth != 0);

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_trust_store(EMPTY_STR);
	orgOpts.set_key_store(EMPTY_STR);
	orgOpts.set_private_key(EMPTY_STR);
	orgOpts.set_private_key_password(EMPTY_STR);
	orgOpts.set_enabled_cipher_suites(EMPTY_STR);
	orgOpts.set_enable_server_cert_auth(!SERVER_CERT);

	EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
	EXPECT_EQ(KEY_STORE, opts.get_key_store());
	EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
	EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
	EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
	EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());
}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

TEST_F(ssl_options_test, test_move_constructor) {
	mqtt::ssl_options opts(std::move(orgOpts));

	EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
	EXPECT_EQ(KEY_STORE, opts.get_key_store());
	EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
	EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
	EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
	EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());

	// Check that the original was moved
	EXPECT_EQ(EMPTY_STR, orgOpts.get_trust_store());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_key_store());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_private_key());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_private_key_password());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_enabled_cipher_suites());
}

// ----------------------------------------------------------------------
// Test the copy assignment operator=(const&)
// ----------------------------------------------------------------------

TEST_F(ssl_options_test, test_copy_assignment) {
	mqtt::ssl_options opts;

	opts = orgOpts;

	EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
	EXPECT_EQ(KEY_STORE, opts.get_key_store());
	EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
	EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
	EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
	EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_trust_store(EMPTY_STR);
	orgOpts.set_key_store(EMPTY_STR);
	orgOpts.set_private_key(EMPTY_STR);
	orgOpts.set_private_key_password(EMPTY_STR);
	orgOpts.set_enabled_cipher_suites(EMPTY_STR);
	orgOpts.set_enable_server_cert_auth(!SERVER_CERT);

	EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
	EXPECT_EQ(KEY_STORE, opts.get_key_store());
	EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
	EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
	EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
	EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());

	// Self assignment should cause no harm
	opts = opts;

	EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
	EXPECT_EQ(KEY_STORE, opts.get_key_store());
	EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
	EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
	EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
	EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());
}

// ----------------------------------------------------------------------
// Test the move assignment, operator=(&&)
// ----------------------------------------------------------------------

TEST_F(ssl_options_test, test_move_assignment) {
	mqtt::ssl_options opts;

	opts = std::move(orgOpts);

	EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
	EXPECT_EQ(KEY_STORE, opts.get_key_store());
	EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
	EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
	EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
	EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());

	// Check that the original was moved
	EXPECT_EQ(EMPTY_STR, orgOpts.get_trust_store());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_key_store());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_private_key());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_private_key_password());
	EXPECT_EQ(EMPTY_STR, orgOpts.get_enabled_cipher_suites());

	// Self assignment should cause no harm
	// (clang++ is smart enough to warn about this)
	#if !defined(__clang__)
		opts = std::move(opts);
		EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
		EXPECT_EQ(KEY_STORE, opts.get_key_store());
		EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
		EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
		EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
		EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());
	#endif
}

// ----------------------------------------------------------------------
// Test set/get of the user and password.
// ----------------------------------------------------------------------

TEST_F(ssl_options_test, test_set_user) {
	mqtt::ssl_options opts;

	opts.set_trust_store(TRUST_STORE);
	opts.set_key_store(KEY_STORE);
	opts.set_private_key(PRIVATE_KEY);
	opts.set_private_key_password(PRIVATE_KEY_PASSWORD);
	opts.set_enabled_cipher_suites(ENABLED_CIPHER_SUITES);
	opts.set_enable_server_cert_auth(SERVER_CERT);

	EXPECT_EQ(TRUST_STORE, opts.get_trust_store());
	EXPECT_EQ(KEY_STORE, opts.get_key_store());
	EXPECT_EQ(PRIVATE_KEY, opts.get_private_key());
	EXPECT_EQ(PRIVATE_KEY_PASSWORD, opts.get_private_key_password());
	EXPECT_EQ(ENABLED_CIPHER_SUITES, opts.get_enabled_cipher_suites());
	EXPECT_EQ(SERVER_CERT, opts.get_enable_server_cert_auth());
}

// ----------------------------------------------------------------------
// Test if empty strings gives nullptr opts
// ----------------------------------------------------------------------

TEST_F(ssl_options_test, test_set_empty_strings) {
	orgOpts.set_trust_store(EMPTY_STR);
	orgOpts.set_key_store(EMPTY_STR);
	orgOpts.set_private_key(EMPTY_STR);
	orgOpts.set_private_key_password(EMPTY_STR);
	orgOpts.set_enabled_cipher_suites(EMPTY_STR);

	// Make sure the empty string represents a nullptr for C library
	const MQTTAsync_SSLOptions& c_struct = get_c_struct(orgOpts);

	EXPECT_EQ(0, memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	EXPECT_EQ(nullptr, c_struct.trustStore);
	EXPECT_EQ(nullptr, c_struct.keyStore);
	EXPECT_EQ(nullptr, c_struct.privateKey);
	EXPECT_EQ(nullptr, c_struct.privateKeyPassword);
	EXPECT_EQ(nullptr, c_struct.enabledCipherSuites);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_ssl_options_test_h

