// test_string_collection.cpp
//
// Unit tests for the string_collection and name_value_collection classes in
// the Paho MQTT C++ library.
//

/*******************************************************************************
 * Copyright (c) 2017-2020 Frank Pagliughi <fpagliughi@mindspring.com>
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

#include <vector>
#include <cstring>
#include "catch2/catch.hpp"
#include "mqtt/string_collection.h"

using namespace mqtt;

const string STR { "Some random string" };
const std::vector<string> VEC { "test0", "test1", "test2" };

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

TEST_CASE("test_dflt_ctor", "[collections]")
{
    string_collection sc;

    REQUIRE(sc.empty());
    REQUIRE(sc.size() == size_t(0));
}

// ----------------------------------------------------------------------
// Test the string copy constructor
// ----------------------------------------------------------------------

TEST_CASE("test_str_copy_ctor", "[collections]")
{
	string_collection sc(STR);

	REQUIRE(size_t(1) == sc.size());
	REQUIRE(STR == sc[0]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(STR.c_str(), c_arr[0]));
}

// ----------------------------------------------------------------------
// Test the string move constructor
// ----------------------------------------------------------------------

TEST_CASE("test_str_move_ctor", "[collections]")
{
	string str(STR);
	string_collection sc(std::move(str));

	REQUIRE(size_t(1) == sc.size());
	REQUIRE(STR == sc[0]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(STR.c_str(), c_arr[0]));
}

// ----------------------------------------------------------------------
// Test the vector copy constructor
// ----------------------------------------------------------------------

TEST_CASE("test_vec_copy_ctor", "[collections]")
{
	string_collection sc(VEC);

	REQUIRE(VEC.size() == sc.size());

	REQUIRE(VEC[0] == sc[0]);
	REQUIRE(VEC[1] == sc[1]);
	REQUIRE(VEC[2] == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(VEC[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(VEC[1].c_str(), c_arr[1]));
	REQUIRE(0 == strcmp(VEC[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the vector move constructor
// ----------------------------------------------------------------------

TEST_CASE("test_vec_move_ctor", "[collections]")
{
	std::vector<string> vec{ VEC };

	string_collection sc(std::move(vec));

	REQUIRE(VEC.size() == sc.size());

	REQUIRE(VEC[0] == sc[0]);
	REQUIRE(VEC[1] == sc[1]);
	REQUIRE(VEC[2] == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(VEC[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(VEC[1].c_str(), c_arr[1]));
	REQUIRE(0 == strcmp(VEC[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the ini vector constructor
// ----------------------------------------------------------------------

TEST_CASE("test_ini_str_ctor", "[collections]")
{
	string_collection sc( { VEC[0], VEC[1], VEC[2] } );

	REQUIRE(size_t(3) == sc.size());

	REQUIRE(VEC[0] == sc[0]);
	REQUIRE(VEC[1] == sc[1]);
	REQUIRE(VEC[2] == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(VEC[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(VEC[1].c_str(), c_arr[1]));
	REQUIRE(0 == strcmp(VEC[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the ini vector constructor
// ----------------------------------------------------------------------

TEST_CASE("test_ini_cstr_ctor", "[collections]")
{
	string_collection sc( { "test0", "test1", "test2" } );

	REQUIRE(size_t(3) == sc.size());

	REQUIRE(string("test0") == sc[0]);
	REQUIRE(string("test1") == sc[1]);
	REQUIRE(string("test2") == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(sc[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(sc[1].c_str(), c_arr[1]));
	REQUIRE(0 == strcmp(sc[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

TEST_CASE("test_copy_ctor", "[collections]")
{
	string_collection orgSC(VEC);
	string_collection sc(orgSC);;

	REQUIRE(VEC.size() == sc.size());

	REQUIRE(VEC[0] == sc[0]);
	REQUIRE(VEC[1] == sc[1]);
	REQUIRE(VEC[2] == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(VEC[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(VEC[1].c_str(), c_arr[1]));
	REQUIRE(0 == strcmp(VEC[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

TEST_CASE("test_move_ctor", "[collections]")
 {
	string_collection orgSC(VEC);
	string_collection sc(std::move(orgSC));

	REQUIRE(VEC.size() == sc.size());

	REQUIRE(VEC[0] == sc[0]);
	REQUIRE(VEC[1] == sc[1]);
	REQUIRE(VEC[2] == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(VEC[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(VEC[1].c_str(), c_arr[1]));
	REQUIRE(0 == strcmp(VEC[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the copy assignment
// ----------------------------------------------------------------------

TEST_CASE("test_copy_assignment", "[collections]")
{
	string_collection orgSC(VEC);
	string_collection sc;

	sc = orgSC;

	REQUIRE(VEC.size() == sc.size());

	REQUIRE(VEC[0] == sc[0]);
	REQUIRE(VEC[1] == sc[1]);
	REQUIRE(VEC[2] == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(VEC[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(VEC[1].c_str(), c_arr[1]));
	REQUIRE(0 == strcmp(VEC[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the move assignment
// ----------------------------------------------------------------------

TEST_CASE("test_move_assignment", "[collections]")
{
	string_collection orgSC(VEC);
	string_collection sc;

	sc = std::move(orgSC);

	REQUIRE(VEC.size() == sc.size());

	REQUIRE(VEC[0] == sc[0]);
	REQUIRE(VEC[1] == sc[1]);
	REQUIRE(VEC[2] == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(VEC[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(VEC[1].c_str(), c_arr[1]));
    REQUIRE(0 == strcmp(VEC[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the push back of strings
// ----------------------------------------------------------------------

TEST_CASE("test_push_str", "[collections]")
{
	string_collection sc;

	for (const auto& s : VEC)
		sc.push_back(s);

	REQUIRE(VEC.size() == sc.size());

	REQUIRE(VEC[0] == sc[0]);
	REQUIRE(VEC[1] == sc[1]);
	REQUIRE(VEC[2] == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(VEC[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(VEC[1].c_str(), c_arr[1]));
	REQUIRE(0 == strcmp(VEC[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the push back of C strings
// ----------------------------------------------------------------------

TEST_CASE("test_push_cstr", "[collections]")
{
	string_collection sc;

	for (const auto& s : VEC)
		sc.push_back(s.c_str());

	REQUIRE(VEC.size() == sc.size());

	REQUIRE(VEC[0] == sc[0]);
	REQUIRE(VEC[1] == sc[1]);
	REQUIRE(VEC[2] == sc[2]);

	auto c_arr = sc.c_arr();

	REQUIRE(0 == strcmp(VEC[0].c_str(), c_arr[0]));
	REQUIRE(0 == strcmp(VEC[1].c_str(), c_arr[1]));
	REQUIRE(0 == strcmp(VEC[2].c_str(), c_arr[2]));
}

// ----------------------------------------------------------------------
// Test the clear method
// ----------------------------------------------------------------------

TEST_CASE("test_clear", "[collections]")
{
	string_collection sc(VEC);

	REQUIRE_FALSE(sc.empty());

	sc.clear();

	REQUIRE(sc.empty());
	REQUIRE(size_t(0) == sc.size());
}

