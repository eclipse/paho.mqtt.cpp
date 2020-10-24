// test.cpp
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
 *******************************************************************************/

#include "async_client_test.h"
#include "async_client_v3_test.h"
#include "client_test.h"
#include "iclient_persistence_test.h"

using namespace CppUnit;

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	CPPUNIT_TEST_SUITE_REGISTRATION( mqtt::iclient_persistence_test );

	CPPUNIT_TEST_SUITE_REGISTRATION( mqtt::async_client_test );
	CPPUNIT_TEST_SUITE_REGISTRATION( mqtt::async_client_v3_test );
	CPPUNIT_TEST_SUITE_REGISTRATION( mqtt::client_test );

	TextUi::TestRunner runner;
	TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry();

	runner.addTest(registry.makeTest());
	return runner.run() ? 0 : 1;
}

