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
 *    Guilherme M. Ferreira - changed test framework from CppUnit to GTest
 *******************************************************************************/

#include "buffer_ref_test.h"
#include "string_collection_test.h"
#include "async_client_test.h"
#include "client_test.h"
#include "message_test.h"
#include "will_options_test.h"
#include "ssl_options_test.h"
#include "connect_options_test.h"
#include "disconnect_options_test.h"
#include "response_options_test.h"
#include "delivery_response_options_test.h"
#include "iclient_persistence_test.h"
#include "token_test.h"
#include "topic_test.h"
#include "exception_test.h"

#include <gtest/gtest.h>

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
