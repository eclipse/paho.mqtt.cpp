// test_ssl_options.cpp
//
// Unit tests for the ssl_options class in the Paho MQTT C++ library.
//

/*******************************************************************************
 * Copyright (c) 2016-2020 Frank Pagliughi <fpagliughi@mindspring.com>
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
 *******************************************************************************/

#define UNIT_TESTS

#include "catch2/catch.hpp"
#include "mqtt/ssl_options.h"

using namespace std::chrono;
using namespace mqtt;

/////////////////////////////////////////////////////////////////////////////

// C struct signature/eyecatcher
static const char* CSIG = "MQTS";
static const size_t CSIG_LEN = std::strlen(CSIG);

static const bool DFLT_SERVER_CERT = true;

static const std::string EMPTY_STR;
static const std::string TRUST_STORE { "trust store" };
static const std::string KEY_STORE { "key store" };
static const std::string PRIVATE_KEY { "private key" };
static const std::string PRIVATE_KEY_PASSWORD { "private key password" };
static const std::string ENABLED_CIPHER_SUITES { "cipher suite" };
static const bool SERVER_CERT { false };

static mqtt::ssl_options orgOpts {
			TRUST_STORE,
			KEY_STORE,
			PRIVATE_KEY,
			PRIVATE_KEY_PASSWORD,
			ENABLED_CIPHER_SUITES,
			SERVER_CERT
};


// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

TEST_CASE("ssl_options dflt constructor", "[options]")
{
	mqtt::ssl_options opts;

	REQUIRE(EMPTY_STR == opts.get_trust_store());
	REQUIRE(EMPTY_STR == opts.get_key_store());
	REQUIRE(EMPTY_STR == opts.get_private_key());
	REQUIRE(EMPTY_STR == opts.get_private_key_password());
	REQUIRE(DFLT_SERVER_CERT == opts.get_enable_server_cert_auth());

	// Make sure the empty string represents a nullptr for C library
	const auto& c_struct = opts.c_struct();

	REQUIRE(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	REQUIRE(c_struct.trustStore == nullptr);
	REQUIRE(c_struct.keyStore == nullptr);
	REQUIRE(c_struct.privateKey == nullptr);
	REQUIRE(c_struct.privateKeyPassword == nullptr);
	REQUIRE(c_struct.enabledCipherSuites == nullptr);
	//REQUIRE(DFLT_SERVER_CERT == c_struct.enableServerCertAuth != 0);
}

// ----------------------------------------------------------------------
// Test the constructor that takes user arguments
// ----------------------------------------------------------------------

TEST_CASE("ssl_options user constructor", "[options]")
{
	mqtt::ssl_options opts { TRUST_STORE, KEY_STORE, PRIVATE_KEY,
		PRIVATE_KEY_PASSWORD, ENABLED_CIPHER_SUITES, SERVER_CERT };

	REQUIRE(TRUST_STORE == opts.get_trust_store());
	REQUIRE(KEY_STORE == opts.get_key_store());
	REQUIRE(PRIVATE_KEY == opts.get_private_key());
	REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
	REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
	REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());

	// Check the underlying C struct
	const auto& c_struct = opts.c_struct();

	REQUIRE(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	REQUIRE(!strcmp(c_struct.trustStore, TRUST_STORE.c_str()));
	REQUIRE(!strcmp(c_struct.keyStore, KEY_STORE.c_str()));
	REQUIRE(!strcmp(c_struct.privateKey, PRIVATE_KEY.c_str()));
	REQUIRE(!strcmp(c_struct.privateKeyPassword, PRIVATE_KEY_PASSWORD.c_str()));
	REQUIRE(!strcmp(c_struct.enabledCipherSuites, ENABLED_CIPHER_SUITES.c_str()));
	//REQUIRE(SERVER_CERT == c_struct.enableServerCertAuth != 0);
}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

TEST_CASE("ssl_options copy constructor", "[options]")
{
	mqtt::ssl_options opts{orgOpts};

	REQUIRE(TRUST_STORE == opts.get_trust_store());
	REQUIRE(KEY_STORE == opts.get_key_store());
	REQUIRE(PRIVATE_KEY == opts.get_private_key());
	REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
	REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
	REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());

	// Check the underlying C struct
	const auto& c_struct = opts.c_struct();

	REQUIRE(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	REQUIRE(!strcmp(c_struct.trustStore, TRUST_STORE.c_str()));
	REQUIRE(!strcmp(c_struct.keyStore, KEY_STORE.c_str()));
	REQUIRE(!strcmp(c_struct.privateKey, PRIVATE_KEY.c_str()));
	REQUIRE(!strcmp(c_struct.privateKeyPassword, PRIVATE_KEY_PASSWORD.c_str()));
	REQUIRE(!strcmp(c_struct.enabledCipherSuites, ENABLED_CIPHER_SUITES.c_str()));
	//REQUIRE(SERVER_CERT == c_struct.enableServerCertAuth != 0);

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_trust_store(EMPTY_STR);
	orgOpts.set_key_store(EMPTY_STR);
	orgOpts.set_private_key(EMPTY_STR);
	orgOpts.set_private_key_password(EMPTY_STR);
	orgOpts.set_enabled_cipher_suites(EMPTY_STR);
	orgOpts.set_enable_server_cert_auth(!SERVER_CERT);

	REQUIRE(TRUST_STORE == opts.get_trust_store());
	REQUIRE(KEY_STORE == opts.get_key_store());
	REQUIRE(PRIVATE_KEY == opts.get_private_key());
	REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
	REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
	REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());
}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

TEST_CASE("ssl_options move constructor", "[options]")
{
    mqtt::ssl_options orgOpts {
			TRUST_STORE,
			KEY_STORE,
			PRIVATE_KEY,
			PRIVATE_KEY_PASSWORD,
			ENABLED_CIPHER_SUITES,
			SERVER_CERT
    };

	mqtt::ssl_options opts(std::move(orgOpts));

	REQUIRE(TRUST_STORE == opts.get_trust_store());
	REQUIRE(KEY_STORE == opts.get_key_store());
	REQUIRE(PRIVATE_KEY == opts.get_private_key());
	REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
	REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
	REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());

	// Check that the original was moved
	REQUIRE(EMPTY_STR == orgOpts.get_trust_store());
	REQUIRE(EMPTY_STR == orgOpts.get_key_store());
	REQUIRE(EMPTY_STR == orgOpts.get_private_key());
	REQUIRE(EMPTY_STR == orgOpts.get_private_key_password());
	REQUIRE(EMPTY_STR == orgOpts.get_enabled_cipher_suites());
}

// ----------------------------------------------------------------------
// Test the copy assignment operator=(const&)
// ----------------------------------------------------------------------

TEST_CASE("ssl_options copy assignment", "[options]")
{
    mqtt::ssl_options orgOpts {
			TRUST_STORE,
			KEY_STORE,
			PRIVATE_KEY,
			PRIVATE_KEY_PASSWORD,
			ENABLED_CIPHER_SUITES,
			SERVER_CERT
    };

	mqtt::ssl_options opts;

	opts = orgOpts;

	REQUIRE(TRUST_STORE == opts.get_trust_store());
	REQUIRE(KEY_STORE == opts.get_key_store());
	REQUIRE(PRIVATE_KEY == opts.get_private_key());
	REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
	REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
	REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());

	// Make sure it's a true copy, not linked to the original
	orgOpts.set_trust_store("");
	orgOpts.set_key_store("");
	orgOpts.set_private_key("");
	orgOpts.set_private_key_password("");
	orgOpts.set_enabled_cipher_suites("");
	orgOpts.set_enable_server_cert_auth(!SERVER_CERT);

	REQUIRE(TRUST_STORE == opts.get_trust_store());
	REQUIRE(KEY_STORE == opts.get_key_store());
	REQUIRE(PRIVATE_KEY == opts.get_private_key());
	REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
	REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
	REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());

	// Self assignment should cause no harm
	opts = opts;

	REQUIRE(TRUST_STORE == opts.get_trust_store());
	REQUIRE(KEY_STORE == opts.get_key_store());
	REQUIRE(PRIVATE_KEY == opts.get_private_key());
	REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
	REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
	REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());
}

// ----------------------------------------------------------------------
// Test the move assignment, operator=(&&)
// ----------------------------------------------------------------------

TEST_CASE("ssl_options move assignment", "[options]")
{
    mqtt::ssl_options orgOpts {
			TRUST_STORE,
			KEY_STORE,
			PRIVATE_KEY,
			PRIVATE_KEY_PASSWORD,
			ENABLED_CIPHER_SUITES,
			SERVER_CERT
    };

	mqtt::ssl_options opts;

	opts = std::move(orgOpts);

	REQUIRE(TRUST_STORE == opts.get_trust_store());
	REQUIRE(KEY_STORE == opts.get_key_store());
	REQUIRE(PRIVATE_KEY == opts.get_private_key());
	REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
	REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
	REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());

	// Check that the original was moved
	REQUIRE(EMPTY_STR == orgOpts.get_trust_store());
	REQUIRE(EMPTY_STR == orgOpts.get_key_store());
	REQUIRE(EMPTY_STR == orgOpts.get_private_key());
	REQUIRE(EMPTY_STR == orgOpts.get_private_key_password());
	REQUIRE(EMPTY_STR == orgOpts.get_enabled_cipher_suites());

	// Self assignment should cause no harm
	// (clang++ is smart enough to warn about this)
	#if !defined(__clang__)
		opts = std::move(opts);
		REQUIRE(TRUST_STORE == opts.get_trust_store());
		REQUIRE(KEY_STORE == opts.get_key_store());
		REQUIRE(PRIVATE_KEY == opts.get_private_key());
		REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
		REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
		REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());
	#endif
}

// ----------------------------------------------------------------------
// Test set/get of the user and password.
// ----------------------------------------------------------------------

TEST_CASE("ssl_options set user", "[options]")
{
	mqtt::ssl_options opts;

	opts.set_trust_store(TRUST_STORE);
	opts.set_key_store(KEY_STORE);
	opts.set_private_key(PRIVATE_KEY);
	opts.set_private_key_password(PRIVATE_KEY_PASSWORD);
	opts.set_enabled_cipher_suites(ENABLED_CIPHER_SUITES);
	opts.set_enable_server_cert_auth(SERVER_CERT);

	REQUIRE(TRUST_STORE == opts.get_trust_store());
	REQUIRE(KEY_STORE == opts.get_key_store());
	REQUIRE(PRIVATE_KEY == opts.get_private_key());
	REQUIRE(PRIVATE_KEY_PASSWORD == opts.get_private_key_password());
	REQUIRE(ENABLED_CIPHER_SUITES == opts.get_enabled_cipher_suites());
	REQUIRE(SERVER_CERT == opts.get_enable_server_cert_auth());
}

// ----------------------------------------------------------------------
// Test if empty strings gives nullptr opts
// ----------------------------------------------------------------------

TEST_CASE("ssl_options set empty strings", "[options]")
{
	orgOpts.set_trust_store(EMPTY_STR);
	orgOpts.set_key_store(EMPTY_STR);
	orgOpts.set_private_key(EMPTY_STR);
	orgOpts.set_private_key_password(EMPTY_STR);
	orgOpts.set_enabled_cipher_suites(EMPTY_STR);

	// Make sure the empty string represents a nullptr for C library
	const auto& c_struct = orgOpts.c_struct();

	REQUIRE(!memcmp(&c_struct.struct_id, CSIG, CSIG_LEN));
	REQUIRE(c_struct.trustStore == nullptr);
	REQUIRE(c_struct.keyStore == nullptr);
	REQUIRE(c_struct.privateKey == nullptr);
	REQUIRE(c_struct.privateKeyPassword == nullptr);
	REQUIRE(c_struct.enabledCipherSuites == nullptr);
}

