// string_collection_test.h
// Unit tests for the string_collection class in the Paho MQTT C++ library.

/*******************************************************************************
 * Copyright (c) 2017 Frank Pagliughi <fpagliughi@mindspring.com>
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
 *    Guilherme M. Ferreira - changed test framework from CppUnit to GTest
 *******************************************************************************/

#ifndef __mqtt_string_collection_test_h
#define __mqtt_string_collection_test_h

#include "mqtt/string_collection.h"
#include <vector>
#include <cstring>

#include <gtest/gtest.h>


namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class string_collection_test : public ::testing::Test
{
protected:
	const string EMPTY_STR;

	const string STR { "Some random string" };
	const std::vector<string> VEC { "test0", "test1", "test2" };

public:
	void SetUp() {}
	void TearDown() {}
};

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_dflt_ctor) {
	string_collection sc;

	EXPECT_TRUE(sc.empty());
	EXPECT_EQ(size_t(0), sc.size());
}

// ----------------------------------------------------------------------
// Test the string copy constructor
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_str_copy_ctor) {
	string_collection sc(STR);

	EXPECT_EQ(size_t(1), sc.size());
	EXPECT_EQ(STR, sc[0]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(STR.c_str(), c_arr[0]);
}

// ----------------------------------------------------------------------
// Test the string move constructor
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_str_move_ctor) {
	string str(STR);
	string_collection sc(std::move(str));

	EXPECT_EQ(size_t(1), sc.size());
	EXPECT_EQ(STR, sc[0]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(STR.c_str(), c_arr[0]);
}

// ----------------------------------------------------------------------
// Test the vector copy constructor
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_vec_copy_ctor) {
	string_collection sc(VEC);

	EXPECT_EQ(VEC.size(), sc.size());

	EXPECT_EQ(VEC[0], sc[0]);
	EXPECT_EQ(VEC[1], sc[1]);
	EXPECT_EQ(VEC[2], sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(VEC[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(VEC[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(VEC[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the vector move constructor
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_vec_move_ctor) {
	std::vector<string> vec{ VEC };

	string_collection sc(std::move(vec));

	EXPECT_EQ(VEC.size(), sc.size());

	EXPECT_EQ(VEC[0], sc[0]);
	EXPECT_EQ(VEC[1], sc[1]);
	EXPECT_EQ(VEC[2], sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(VEC[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(VEC[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(VEC[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the ini vector constructor
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_ini_str_ctor) {
	string_collection sc( { VEC[0], VEC[1], VEC[2] } );

	EXPECT_EQ(size_t(3), sc.size());

	EXPECT_EQ(VEC[0], sc[0]);
	EXPECT_EQ(VEC[1], sc[1]);
	EXPECT_EQ(VEC[2], sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(VEC[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(VEC[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(VEC[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the ini vector constructor
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_ini_cstr_ctor) {
	string_collection sc( { "test0", "test1", "test2" } );

	EXPECT_EQ(size_t(3), sc.size());

	EXPECT_EQ(string("test0"), sc[0]);
	EXPECT_EQ(string("test1"), sc[1]);
	EXPECT_EQ(string("test2"), sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(sc[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(sc[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(sc[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_copy_ctor) {
	string_collection orgSC(VEC);
	string_collection sc(orgSC);;

	EXPECT_EQ(VEC.size(), sc.size());

	EXPECT_EQ(VEC[0], sc[0]);
	EXPECT_EQ(VEC[1], sc[1]);
	EXPECT_EQ(VEC[2], sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(VEC[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(VEC[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(VEC[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_move_ctor) {
	string_collection orgSC(VEC);
	string_collection sc(std::move(orgSC));

	EXPECT_EQ(VEC.size(), sc.size());

	EXPECT_EQ(VEC[0], sc[0]);
	EXPECT_EQ(VEC[1], sc[1]);
	EXPECT_EQ(VEC[2], sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(VEC[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(VEC[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(VEC[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the copy assignment
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_copy_assignment) {
	string_collection orgSC(VEC);
	string_collection sc;

	sc = orgSC;

	EXPECT_EQ(VEC.size(), sc.size());

	EXPECT_EQ(VEC[0], sc[0]);
	EXPECT_EQ(VEC[1], sc[1]);
	EXPECT_EQ(VEC[2], sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(VEC[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(VEC[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(VEC[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the move assignment
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_move_assignment) {
	string_collection orgSC(VEC);
	string_collection sc;

	sc = std::move(orgSC);

	EXPECT_EQ(VEC.size(), sc.size());

	EXPECT_EQ(VEC[0], sc[0]);
	EXPECT_EQ(VEC[1], sc[1]);
	EXPECT_EQ(VEC[2], sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(VEC[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(VEC[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(VEC[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the push back of strings
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_push_str) {
	string_collection sc;

	for (const auto& s : VEC)
		sc.push_back(s);

	EXPECT_EQ(VEC.size(), sc.size());

	EXPECT_EQ(VEC[0], sc[0]);
	EXPECT_EQ(VEC[1], sc[1]);
	EXPECT_EQ(VEC[2], sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(VEC[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(VEC[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(VEC[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the push back of C strings
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_push_cstr) {
	string_collection sc;

	for (const auto& s : VEC)
		sc.push_back(s.c_str());

	EXPECT_EQ(VEC.size(), sc.size());

	EXPECT_EQ(VEC[0], sc[0]);
	EXPECT_EQ(VEC[1], sc[1]);
	EXPECT_EQ(VEC[2], sc[2]);

	auto c_arr = sc.c_arr();

	EXPECT_STRCASEEQ(VEC[0].c_str(), c_arr[0]);
	EXPECT_STRCASEEQ(VEC[1].c_str(), c_arr[1]);
	EXPECT_STRCASEEQ(VEC[2].c_str(), c_arr[2]);
}

// ----------------------------------------------------------------------
// Test the clear method
// ----------------------------------------------------------------------

TEST_F(string_collection_test, test_clear) {
	string_collection sc(VEC);

	EXPECT_FALSE(sc.empty());

	sc.clear();

	EXPECT_TRUE(sc.empty());
	EXPECT_EQ(size_t(0), sc.size());
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_string_collection_test_h
