// test_topic_matcher.cpp
//
// Unit tests for the topic_matcher class in the Paho MQTT C++ library.
//

/*******************************************************************************
 * Copyright (c) 2022 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v20.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 *******************************************************************************/

#define UNIT_TESTS

#include "catch2_version.h"
#include "mqtt/topic_matcher.h"

using namespace mqtt;

/////////////////////////////////////////////////////////////////////////////

TEST_CASE("insert/get", "[topic_matcher]")
{
	topic_matcher<int> matcher;

	matcher.insert({"some/random/topic", 42});

	auto it = matcher.find("some/random/topic");

	REQUIRE(it != matcher.end());
	REQUIRE(it->first == "some/random/topic");
	REQUIRE(it->second == 42);
}

TEST_CASE("matcher matches", "[topic_matcher]")
{
	topic_matcher<int> matcher {
		{ "some/random/topic", 42 },
		{ "some/#", 99 },
		{ "some/other/topic", 55 },
		{ "some/+/topic", 33 }
	};

	auto it = matcher.matches("some/random/topic");

	for (; it != matcher.end(); ++it) {
		bool ok = (
		    (it->first == "some/random/topic" && it->second == 42) ||
			(it->first == "some/#" &&  it->second == 99) ||
			(it-> first == "some/+/topic" && it->second == 33)
		);
		REQUIRE(ok);
	}
}
