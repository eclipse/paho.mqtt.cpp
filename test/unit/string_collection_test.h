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
 *******************************************************************************/

#ifndef __mqtt_string_collection_test_h
#define __mqtt_string_collection_test_h

#include "mqtt/string_collection.h"
#include <vector>
#include <cstring>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>


namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class string_collection_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( string_collection_test );

	CPPUNIT_TEST( test_dflt_ctor );
	CPPUNIT_TEST( test_str_copy_ctor );
	CPPUNIT_TEST( test_str_move_ctor );
	CPPUNIT_TEST( test_vec_copy_ctor );
	CPPUNIT_TEST( test_vec_move_ctor );
	CPPUNIT_TEST( test_ini_str_ctor );
	CPPUNIT_TEST( test_ini_cstr_ctor );
	CPPUNIT_TEST( test_copy_ctor );
	CPPUNIT_TEST( test_move_ctor );
	CPPUNIT_TEST( test_copy_assignment );
	CPPUNIT_TEST( test_move_assignment );
	CPPUNIT_TEST( test_push_str );
	CPPUNIT_TEST( test_push_cstr );
	CPPUNIT_TEST( test_clear );


	CPPUNIT_TEST_SUITE_END();

	const string EMPTY_STR;

	const string STR { "Some random string" };
	const std::vector<string> VEC { "test0", "test1", "test2" };

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test the default constructor
// ----------------------------------------------------------------------

	void test_dflt_ctor() {
		string_collection sc;

		CPPUNIT_ASSERT(sc.empty());
		CPPUNIT_ASSERT_EQUAL(size_t(0), sc.size());
	}

// ----------------------------------------------------------------------
// Test the string copy constructor
// ----------------------------------------------------------------------

	void test_str_copy_ctor() {
		string_collection sc(STR);

		CPPUNIT_ASSERT_EQUAL(size_t(1), sc.size());
		CPPUNIT_ASSERT_EQUAL(STR, sc[0]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(STR.c_str(), c_arr[0]));
	}

// ----------------------------------------------------------------------
// Test the string move constructor
// ----------------------------------------------------------------------

	void test_str_move_ctor() {
		string str(STR);
		string_collection sc(std::move(str));

		CPPUNIT_ASSERT_EQUAL(size_t(1), sc.size());
		CPPUNIT_ASSERT_EQUAL(STR, sc[0]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(STR.c_str(), c_arr[0]));
	}

// ----------------------------------------------------------------------
// Test the vector copy constructor
// ----------------------------------------------------------------------

	void test_vec_copy_ctor() {
		string_collection sc(VEC);

		CPPUNIT_ASSERT_EQUAL(VEC.size(), sc.size());

		CPPUNIT_ASSERT_EQUAL(VEC[0], sc[0]);
		CPPUNIT_ASSERT_EQUAL(VEC[1], sc[1]);
		CPPUNIT_ASSERT_EQUAL(VEC[2], sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(VEC[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(VEC[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(VEC[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the vector move constructor
// ----------------------------------------------------------------------

	void test_vec_move_ctor() {
		std::vector<string> vec{ VEC };

		string_collection sc(std::move(vec));

		CPPUNIT_ASSERT_EQUAL(VEC.size(), sc.size());

		CPPUNIT_ASSERT_EQUAL(VEC[0], sc[0]);
		CPPUNIT_ASSERT_EQUAL(VEC[1], sc[1]);
		CPPUNIT_ASSERT_EQUAL(VEC[2], sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(VEC[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(VEC[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(VEC[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the ini vector constructor
// ----------------------------------------------------------------------

	void test_ini_str_ctor() {
		string_collection sc( { VEC[0], VEC[1], VEC[2] } );

		CPPUNIT_ASSERT_EQUAL(size_t(3), sc.size());

		CPPUNIT_ASSERT_EQUAL(VEC[0], sc[0]);
		CPPUNIT_ASSERT_EQUAL(VEC[1], sc[1]);
		CPPUNIT_ASSERT_EQUAL(VEC[2], sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(VEC[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(VEC[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(VEC[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the ini vector constructor
// ----------------------------------------------------------------------

	void test_ini_cstr_ctor() {
		string_collection sc( { "test0", "test1", "test2" } );

		CPPUNIT_ASSERT_EQUAL(size_t(3), sc.size());

		CPPUNIT_ASSERT_EQUAL(string("test0"), sc[0]);
		CPPUNIT_ASSERT_EQUAL(string("test1"), sc[1]);
		CPPUNIT_ASSERT_EQUAL(string("test2"), sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(sc[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(sc[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(sc[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the copy constructor
// ----------------------------------------------------------------------

	void test_copy_ctor() {
		string_collection orgSC(VEC);
		string_collection sc(orgSC);;

		CPPUNIT_ASSERT_EQUAL(VEC.size(), sc.size());

		CPPUNIT_ASSERT_EQUAL(VEC[0], sc[0]);
		CPPUNIT_ASSERT_EQUAL(VEC[1], sc[1]);
		CPPUNIT_ASSERT_EQUAL(VEC[2], sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(VEC[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(VEC[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(VEC[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the move constructor
// ----------------------------------------------------------------------

	void test_move_ctor() {
		string_collection orgSC(VEC);
		string_collection sc(std::move(orgSC));

		CPPUNIT_ASSERT_EQUAL(VEC.size(), sc.size());

		CPPUNIT_ASSERT_EQUAL(VEC[0], sc[0]);
		CPPUNIT_ASSERT_EQUAL(VEC[1], sc[1]);
		CPPUNIT_ASSERT_EQUAL(VEC[2], sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(VEC[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(VEC[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(VEC[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the copy assignment
// ----------------------------------------------------------------------

	void test_copy_assignment() {
		string_collection orgSC(VEC);
		string_collection sc;

		sc = orgSC;

		CPPUNIT_ASSERT_EQUAL(VEC.size(), sc.size());

		CPPUNIT_ASSERT_EQUAL(VEC[0], sc[0]);
		CPPUNIT_ASSERT_EQUAL(VEC[1], sc[1]);
		CPPUNIT_ASSERT_EQUAL(VEC[2], sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(VEC[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(VEC[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(VEC[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the move assignment
// ----------------------------------------------------------------------

	void test_move_assignment() {
		string_collection orgSC(VEC);
		string_collection sc;

		sc = std::move(orgSC);

		CPPUNIT_ASSERT_EQUAL(VEC.size(), sc.size());

		CPPUNIT_ASSERT_EQUAL(VEC[0], sc[0]);
		CPPUNIT_ASSERT_EQUAL(VEC[1], sc[1]);
		CPPUNIT_ASSERT_EQUAL(VEC[2], sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(VEC[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(VEC[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(VEC[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the push back of strings
// ----------------------------------------------------------------------

	void test_push_str() {
		string_collection sc;

		for (const auto& s : VEC)
			sc.push_back(s);

		CPPUNIT_ASSERT_EQUAL(VEC.size(), sc.size());

		CPPUNIT_ASSERT_EQUAL(VEC[0], sc[0]);
		CPPUNIT_ASSERT_EQUAL(VEC[1], sc[1]);
		CPPUNIT_ASSERT_EQUAL(VEC[2], sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(VEC[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(VEC[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(VEC[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the push back of C strings
// ----------------------------------------------------------------------

	void test_push_cstr() {
		string_collection sc;

		for (const auto& s : VEC)
			sc.push_back(s.c_str());

		CPPUNIT_ASSERT_EQUAL(VEC.size(), sc.size());

		CPPUNIT_ASSERT_EQUAL(VEC[0], sc[0]);
		CPPUNIT_ASSERT_EQUAL(VEC[1], sc[1]);
		CPPUNIT_ASSERT_EQUAL(VEC[2], sc[2]);

		auto c_arr = sc.c_arr();

		CPPUNIT_ASSERT(!strcmp(VEC[0].c_str(), c_arr[0]));
		CPPUNIT_ASSERT(!strcmp(VEC[1].c_str(), c_arr[1]));
		CPPUNIT_ASSERT(!strcmp(VEC[2].c_str(), c_arr[2]));
	}

// ----------------------------------------------------------------------
// Test the clear method
// ----------------------------------------------------------------------

	void test_clear() {
		string_collection sc(VEC);

		CPPUNIT_ASSERT(!sc.empty());

		sc.clear();

		CPPUNIT_ASSERT(sc.empty());
		CPPUNIT_ASSERT_EQUAL(size_t(0), sc.size());
	}


};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_string_collection_test_h

