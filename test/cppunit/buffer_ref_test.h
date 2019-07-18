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
 *******************************************************************************/

#ifndef __mqtt_buffer_ref_test_h
#define __mqtt_buffer_ref_test_h

#include "mqtt/buffer_ref.h"
#include <cstring>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class buffer_ref_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( buffer_ref_test );

	CPPUNIT_TEST( test_dflt_ctor );
	CPPUNIT_TEST( test_str_copy_ctor );
	CPPUNIT_TEST( test_str_move_ctor );
	CPPUNIT_TEST( test_cstr_ctor );
	CPPUNIT_TEST( test_ptr_copy_ctor );
	CPPUNIT_TEST( test_ptr_move_ctor );
	CPPUNIT_TEST( test_copy_ctor );
	CPPUNIT_TEST( test_move_ctor );
	CPPUNIT_TEST( test_copy_assignment );
	CPPUNIT_TEST( test_move_assignment );
	CPPUNIT_TEST( test_str_copy_assignment );
	CPPUNIT_TEST( test_str_move_assignment );
	CPPUNIT_TEST( test_cstr_assignment );
	CPPUNIT_TEST( test_ptr_copy_assignment );
	CPPUNIT_TEST( test_ptr_move_assignment );
	CPPUNIT_TEST( test_reset );


	CPPUNIT_TEST_SUITE_END();

	const string EMPTY_STR;

	const string STR { "Some random string" };
	const binary BIN { "\x0\x1\x2\x3\x4\x5\x6\x7" };

	const char* CSTR = "Another random string";
	const size_t CSTR_LEN = strlen(CSTR);

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

	void test_dflt_ctor() {
		string_ref sr;

		CPPUNIT_ASSERT(!sr);
		CPPUNIT_ASSERT(sr.empty());
	}

// ----------------------------------------------------------------------
// Test the string copy constructor
// ----------------------------------------------------------------------

	void test_str_copy_ctor() {
		string_ref sr(STR);
		CPPUNIT_ASSERT_EQUAL(STR, sr.str());
	}

// ----------------------------------------------------------------------
// Test the string move constructor
// ----------------------------------------------------------------------

	void test_str_move_ctor() {
		string str(STR);
		string_ref sr(std::move(str));

		CPPUNIT_ASSERT_EQUAL(STR, sr.str());
		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, str);
	}

// ----------------------------------------------------------------------
// Test the c-string constructor
// ----------------------------------------------------------------------

	void test_cstr_ctor() {
		string_ref sr(CSTR);

		CPPUNIT_ASSERT_EQUAL(CSTR_LEN, strlen(sr.c_str()));
		CPPUNIT_ASSERT(!strcmp(CSTR, sr.c_str()));
	}

// ----------------------------------------------------------------------
// Test the pointer copy constructor
// ----------------------------------------------------------------------

	void test_ptr_copy_ctor() {
		string_ptr sp(new string(STR));
		string_ref sr(sp);

		CPPUNIT_ASSERT_EQUAL(STR, sr.str());
	}

// ----------------------------------------------------------------------
// Test the pointer move constructor
// ----------------------------------------------------------------------

	void test_ptr_move_ctor() {
		string_ptr sp(new string(STR));
		string_ref sr(std::move(sp));

		CPPUNIT_ASSERT_EQUAL(STR, sr.str());

		CPPUNIT_ASSERT(!sp);
	}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

	void test_copy_ctor() {
		string_ref orgSR(STR);
		string_ref sr(orgSR);

		CPPUNIT_ASSERT_EQUAL(STR, sr.str());
		CPPUNIT_ASSERT_EQUAL(orgSR.ptr().get(), sr.ptr().get());
		CPPUNIT_ASSERT_EQUAL(2L, sr.ptr().use_count());
	}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

	void test_move_ctor() {
		string_ref orgSR(STR);
		string_ref sr(std::move(orgSR));

		CPPUNIT_ASSERT_EQUAL(STR, sr.str());

		CPPUNIT_ASSERT(!orgSR);
		CPPUNIT_ASSERT_EQUAL(1L, sr.ptr().use_count());
	}

// ----------------------------------------------------------------------
// Test the copy assignment
// ----------------------------------------------------------------------

	void test_copy_assignment() {
		string_ref sr, orgSR(STR);

		sr = orgSR;

		CPPUNIT_ASSERT_EQUAL(STR, sr.str());
		CPPUNIT_ASSERT_EQUAL(orgSR.ptr().get(), sr.ptr().get());
		CPPUNIT_ASSERT_EQUAL(2L, sr.ptr().use_count());

		// Test for true copy
		orgSR = EMPTY_STR;
		CPPUNIT_ASSERT_EQUAL(STR, sr.str());
	}

// ----------------------------------------------------------------------
// Test the move assignment
// ----------------------------------------------------------------------

	void test_move_assignment() {
		string_ref sr, orgSR(STR);

		sr = std::move(orgSR);

		CPPUNIT_ASSERT_EQUAL(STR, sr.str());

		CPPUNIT_ASSERT(!orgSR);
		CPPUNIT_ASSERT_EQUAL(1L, sr.ptr().use_count());
	}

// ----------------------------------------------------------------------
// Test the string copy assignment
// ----------------------------------------------------------------------

	void test_str_copy_assignment() {
		string str(STR);
		string_ref sr;

		sr = str;
		CPPUNIT_ASSERT_EQUAL(STR, sr.str());

		str = EMPTY_STR;
		CPPUNIT_ASSERT_EQUAL(STR, sr.str());
	}

// ----------------------------------------------------------------------
// Test the string move assignment
// ----------------------------------------------------------------------

	void test_str_move_assignment() {
		string str(STR);
		string_ref sr;

		sr = std::move(str);
		CPPUNIT_ASSERT_EQUAL(STR, sr.str());

		CPPUNIT_ASSERT_EQUAL(EMPTY_STR, str);
		CPPUNIT_ASSERT_EQUAL(1L, sr.ptr().use_count());
	}

// ----------------------------------------------------------------------
// Test the c-string assignment
// ----------------------------------------------------------------------

	void test_cstr_assignment() {
		string_ref sr;
		sr = CSTR;

		CPPUNIT_ASSERT_EQUAL(CSTR_LEN, strlen(sr.c_str()));
		CPPUNIT_ASSERT(!strcmp(CSTR, sr.c_str()));
	}

// ----------------------------------------------------------------------
// Test the pointer copy assignment
// ----------------------------------------------------------------------

	void test_ptr_copy_assignment() {
		string_ptr sp(new string(STR));
		string_ref sr;

		sr = sp;

		CPPUNIT_ASSERT_EQUAL(STR, sr.str());
	}

// ----------------------------------------------------------------------
// Test the pointer move assignment
// ----------------------------------------------------------------------

	void test_ptr_move_assignment() {
		string_ptr sp(new string(STR));
		string_ref sr;

		sr = std::move(sp);

		CPPUNIT_ASSERT_EQUAL(STR, sr.str());

		CPPUNIT_ASSERT(!sp);
	}

// ----------------------------------------------------------------------
// Test the reset
// ----------------------------------------------------------------------

	void test_reset() {
		string_ref sr(STR);

		sr.reset();
		CPPUNIT_ASSERT(!sr);
		CPPUNIT_ASSERT(sr.empty());
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_buffer_ref_test_h

