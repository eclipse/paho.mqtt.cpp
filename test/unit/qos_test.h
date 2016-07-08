// qos_test.h
// Unit tests for the qos class in the Paho MQTT C++ library.

/*******************************************************************************
 * Copyright (c) 2016 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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
 *    Guilherme M. Ferreira - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_qos_test_h
#define __mqtt_qos_test_h

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cstring>

#include "mqtt/qos.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

class qos_test : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( qos_test );

	CPPUNIT_TEST( test_validate_qos );

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

// ----------------------------------------------------------------------
// Test the validate_qos()
// ----------------------------------------------------------------------

	void test_validate_qos() {
		bool pass_lower = false;
		try {
			mqtt::validate_qos(-1);
		} catch (std::invalid_argument& ex) {
			pass_lower = true;
		}
		CPPUNIT_ASSERT(pass_lower);

		bool pass_0 = true;
		try {
			mqtt::validate_qos(0);
		} catch (std::invalid_argument& ex) {
			pass_0 = false;
		}
		CPPUNIT_ASSERT(pass_0);

		bool pass_1 = true;
		try {
			mqtt::validate_qos(1);
		} catch (std::invalid_argument& ex) {
			pass_1 = false;
		}
		CPPUNIT_ASSERT(pass_1);

		bool pass_2 = true;
		try {
			mqtt::validate_qos(2);
		} catch (std::invalid_argument& ex) {
			pass_2 = false;
		}
		CPPUNIT_ASSERT(pass_2);

		bool pass_higher = false;
		try {
			mqtt::validate_qos(3);
		} catch (std::invalid_argument& ex) {
			pass_higher = true;
		}
		CPPUNIT_ASSERT(pass_higher);
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_qos_test_h
