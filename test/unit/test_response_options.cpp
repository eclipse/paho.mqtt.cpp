// test_response_options.cpp
//
// Unit tests for the response_options class in the Paho MQTT C++ library.
//

/*******************************************************************************
 * Copyright (c) 2016 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
 * Copyright (c) 2020 Frank Pagliughi <fpagliughi@mindspring.com>
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
 *    Guilherme M. Ferreira
 *        - initial implementation and documentation
 *    Frank Pagliughi
 *        - converted to use Catch2
 *        - Merged in delivery response options
 *******************************************************************************/

#define UNIT_TESTS

#include <cstring>
#include "catch2/catch.hpp"
#include "mqtt/response_options.h"
#include "mock_async_client.h"

using namespace mqtt;

/////////////////////////////////////////////////////////////////////////////

static constexpr token::Type TOKEN_TYPE = token::Type::CONNECT;

static mock_async_client cli;

// ----------------------------------------------------------------------
// Test default constructor
// ----------------------------------------------------------------------

TEST_CASE("response_options dflt constructor", "[options]")
{
	mqtt::response_options opts;
	const auto& c_struct = opts.c_struct();

	REQUIRE(c_struct.context == nullptr);

	// Make sure the callback functions are set during object construction
	REQUIRE(c_struct.onSuccess != nullptr);
	REQUIRE(c_struct.onFailure != nullptr);
}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

TEST_CASE("response_options user constructor", "[options]")
{
	mqtt::token_ptr token { mqtt::token::create(TOKEN_TYPE, cli) };
	mqtt::response_options opts { token };
	const auto& c_struct = opts.c_struct();

	REQUIRE(c_struct.context == token.get());

	// Make sure the callback functions are set during object construction
	REQUIRE(c_struct.onSuccess != nullptr);
	REQUIRE(c_struct.onFailure != nullptr);
}

// ----------------------------------------------------------------------
// Test set context
// ----------------------------------------------------------------------

TEST_CASE("response_options set token", "[options]")
{
	mqtt::response_options opts;
	const auto& c_struct = opts.c_struct();

	REQUIRE(c_struct.context == nullptr);
	mqtt::token_ptr token { mqtt::token::create(TOKEN_TYPE, cli) };
	opts.set_token( token );
	REQUIRE(c_struct.context == token.get());
}

/////////////////////////////////////////////////////////////////////////////
// Delivery Response Options
/////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------
// Test default constructor
// ----------------------------------------------------------------------

TEST_CASE("delivery_response_options dflt constructor", "[options]")
{
	mqtt::delivery_response_options opts;
	const auto& c_struct = opts.c_struct();

	REQUIRE(c_struct.context == nullptr);

	// Make sure the callback functions are set during object construction
	REQUIRE(c_struct.onSuccess != nullptr);
	REQUIRE(c_struct.onFailure != nullptr);
}

// ----------------------------------------------------------------------
// Test user constructor
// ----------------------------------------------------------------------

TEST_CASE("delivery_response_options user constructor", "[options]")
{
	mock_async_client cli;

	mqtt::delivery_token_ptr token { new mqtt::delivery_token{ cli } };
	mqtt::delivery_response_options opts { token };
	const auto& c_struct = opts.c_struct();

	REQUIRE(c_struct.context == token.get());

	// Make sure the callback functions are set during object construction
	REQUIRE(c_struct.onSuccess != nullptr);
	REQUIRE(c_struct.onFailure != nullptr);
}

// ----------------------------------------------------------------------
// Test set context
// ----------------------------------------------------------------------

TEST_CASE("delivery_response_options set token", "[options]")
{
	mqtt::delivery_response_options opts;
	const auto& c_struct = opts.c_struct();

	REQUIRE(c_struct.context == nullptr);

	mock_async_client cli;
	mqtt::delivery_token_ptr token { new mqtt::delivery_token{ cli } };
	opts.set_token( token );
	REQUIRE(c_struct.context == token.get());
}

