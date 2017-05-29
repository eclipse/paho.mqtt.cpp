// buffer_ref_test.h
// Unit tests for the buffer_ref class in the Paho MQTT C++ library.

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

#ifndef __mqtt_buffer_ref_test_h
#define __mqtt_buffer_ref_test_h

#include "mqtt/buffer_ref.h"
#include <cstring>

#include <gtest/gtest.h>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class buffer_ref_test : public ::testing::Test
{
protected:
	const string EMPTY_STR;

	const string STR { "Some random string" };
	const binary BIN { "\x0\x1\x2\x3\x4\x5\x6\x7" };

	const char* CSTR = "Another random string";
	const size_t CSTR_LEN = strlen(CSTR);

public:
	void setUp() {}
	void tearDown() {}
};

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_dflt_ctor) {
	string_ref sr;

	EXPECT_FALSE(sr);
	EXPECT_TRUE(sr.empty());
}

// ----------------------------------------------------------------------
// Test the string copy constructor
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_str_copy_ctor) {
	string_ref sr(STR);
	EXPECT_EQ(STR, sr.str());
}

// ----------------------------------------------------------------------
// Test the string move constructor
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_str_move_ctor) {
	string str(STR);
	string_ref sr(std::move(str));

	EXPECT_EQ(STR, sr.str());
	EXPECT_EQ(EMPTY_STR, str);
}

// ----------------------------------------------------------------------
// Test the c-string constructor
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_cstr_ctor) {
	string_ref sr(CSTR);

	EXPECT_EQ(CSTR_LEN, strlen(sr.c_str()));
	EXPECT_STRCASEEQ(CSTR, sr.c_str());
}

// ----------------------------------------------------------------------
// Test the pointer copy constructor
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_ptr_copy_ctor) {
	string_ptr sp(new string(STR));
	string_ref sr(sp);

	EXPECT_EQ(STR, sr.str());
}

// ----------------------------------------------------------------------
// Test the pointer move constructor
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_ptr_move_ctor) {
	string_ptr sp(new string(STR));
	string_ref sr(std::move(sp));

	EXPECT_EQ(STR, sr.str());

	EXPECT_FALSE(sp);
}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_copy_ctor) {
	string_ref orgSR(STR);
	string_ref sr(orgSR);

	EXPECT_EQ(STR, sr.str());
	EXPECT_EQ(orgSR.ptr().get(), sr.ptr().get());
	EXPECT_EQ(2L, sr.ptr().use_count());
}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_move_ctor) {
	string_ref orgSR(STR);
	string_ref sr(std::move(orgSR));

	EXPECT_EQ(STR, sr.str());

	EXPECT_FALSE(orgSR);
	EXPECT_EQ(1L, sr.ptr().use_count());
}

// ----------------------------------------------------------------------
// Test the copy assignment
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_copy_assignment) {
	string_ref sr, orgSR(STR);

	sr = orgSR;

	EXPECT_EQ(STR, sr.str());
	EXPECT_EQ(orgSR.ptr().get(), sr.ptr().get());
	EXPECT_EQ(2L, sr.ptr().use_count());

	// Test for true copy
	orgSR = EMPTY_STR;
	EXPECT_EQ(STR, sr.str());
}

// ----------------------------------------------------------------------
// Test the move assignment
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_move_assignment) {
	string_ref sr, orgSR(STR);

	sr = std::move(orgSR);

	EXPECT_EQ(STR, sr.str());

	EXPECT_FALSE(orgSR);
	EXPECT_EQ(1L, sr.ptr().use_count());
}

// ----------------------------------------------------------------------
// Test the string copy assignment
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_str_copy_assignment) {
	string str(STR);
	string_ref sr;

	sr = str;
	EXPECT_EQ(STR, sr.str());

	str = EMPTY_STR;
	EXPECT_EQ(STR, sr.str());
}

// ----------------------------------------------------------------------
// Test the string move assignment
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_str_move_assignment) {
	string str(STR);
	string_ref sr;

	sr = std::move(str);
	EXPECT_EQ(STR, sr.str());

	EXPECT_EQ(EMPTY_STR, str);
	EXPECT_EQ(1L, sr.ptr().use_count());
}

// ----------------------------------------------------------------------
// Test the c-string assignment
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_cstr_assignment) {
	string_ref sr;
	sr = CSTR;

	EXPECT_EQ(CSTR_LEN, strlen(sr.c_str()));
	EXPECT_STRCASEEQ(CSTR, sr.c_str());
}

// ----------------------------------------------------------------------
// Test the pointer copy assignment
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_ptr_copy_assignment) {
	string_ptr sp(new string(STR));
	string_ref sr;

	sr = sp;

	EXPECT_EQ(STR, sr.str());
}

// ----------------------------------------------------------------------
// Test the pointer move assignment
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_ptr_move_assignment) {
	string_ptr sp(new string(STR));
	string_ref sr;

	sr = std::move(sp);

	EXPECT_EQ(STR, sr.str());

	EXPECT_FALSE(sp);
}

// ----------------------------------------------------------------------
// Test the reset
// ----------------------------------------------------------------------

TEST_F(buffer_ref_test, test_reset) {
	string_ref sr(STR);

	sr.reset();
	EXPECT_FALSE(sr);
	EXPECT_TRUE(sr.empty());
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_buffer_ref_test_h

