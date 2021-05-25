// topic_test.h
//
// Unit tests for the topic class in the Paho MQTT C++ library.
//

/*******************************************************************************
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
 *******************************************************************************/

#define UNIT_TESTS

#include <algorithm>
#include <memory>
#include <stdexcept>
#include "catch2/catch.hpp"
#include "mqtt/topic.h"
#include "mock_async_client.h"

using namespace mqtt;

/////////////////////////////////////////////////////////////////////////////

static const int DFLT_QOS = message::DFLT_QOS;
static const bool DFLT_RETAINED = message::DFLT_RETAINED;

static const std::string TOPIC { "my/topic/name" };
static const int QOS = 1;
static const bool RETAINED = true;

static const int BAD_LOW_QOS  = -1;
static const int BAD_HIGH_QOS =  3;

static const char* BUF = "Hello there";
static const size_t N = std::strlen(BUF);
static const binary PAYLOAD { BUF };

static mqtt::mock_async_client cli;

// ----------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------

TEST_CASE("basic ctor", "[topic]")
{
    mqtt::topic topic { cli, TOPIC };

    REQUIRE(static_cast<iasync_client*>(&cli) == &topic.get_client());
    REQUIRE(TOPIC == topic.get_name());
    REQUIRE(TOPIC == topic.to_string());
    REQUIRE(DFLT_QOS == topic.get_qos());
    REQUIRE(DFLT_RETAINED == topic.get_retained());
}

// ----------------------------------------------------------------------

TEST_CASE("full ctor", "[topic]")
{
    mqtt::topic topic { cli, TOPIC, QOS, RETAINED };

    REQUIRE(static_cast<iasync_client*>(&cli) == &topic.get_client());
    REQUIRE(TOPIC == topic.get_name());
    REQUIRE(TOPIC == topic.to_string());
    REQUIRE(QOS == topic.get_qos());
    REQUIRE(RETAINED == topic.get_retained());
}

// ----------------------------------------------------------------------
// get/set
// ----------------------------------------------------------------------

TEST_CASE("get/set", "[topic]")
{
    mqtt::topic topic { cli, TOPIC };

    REQUIRE(DFLT_QOS == topic.get_qos());
    REQUIRE(DFLT_RETAINED == topic.get_retained());

    SECTION("qos") {
        topic.set_qos(QOS);
        REQUIRE(QOS == topic.get_qos());

        REQUIRE_THROWS(topic.set_qos(BAD_LOW_QOS));
        REQUIRE_THROWS(topic.set_qos(BAD_HIGH_QOS));
    }

    SECTION("retained") {
        topic.set_retained(RETAINED);
        REQUIRE(RETAINED == topic.get_retained());

        topic.set_retained(!RETAINED);
        REQUIRE(!RETAINED == topic.get_retained());
    }
}

// ----------------------------------------------------------------------
// Publish
// ----------------------------------------------------------------------

TEST_CASE("publish C str", "[topic]")
{
    mqtt::topic topic{ cli, TOPIC, QOS, RETAINED };

    auto tok = topic.publish(BUF, N);
    REQUIRE(tok);

    auto msg = tok->get_message();
    REQUIRE(msg);

    REQUIRE(TOPIC == msg->get_topic());
    REQUIRE(msg->get_payload().data());
    REQUIRE(0 == memcmp(BUF, msg->get_payload().data(), N));
    REQUIRE(QOS == msg->get_qos());
    REQUIRE(RETAINED == msg->is_retained());
}

// ----------------------------------------------------------------------

TEST_CASE("publish full C str", "[topic]")
{
    mqtt::topic topic { cli, TOPIC };

    auto tok = topic.publish(BUF, N, QOS, RETAINED);
    REQUIRE(tok);

    auto msg = tok->get_message();
    REQUIRE(msg);

    REQUIRE(TOPIC == msg->get_topic());
    REQUIRE(msg->get_payload().data());
    REQUIRE(0 == memcmp(BUF, msg->get_payload().data(), N));
    REQUIRE(QOS == msg->get_qos());
    REQUIRE(RETAINED == msg->is_retained());
}

// ----------------------------------------------------------------------

TEST_CASE("publish binary", "[topic]")
{
    mqtt::topic topic { cli, TOPIC, QOS, RETAINED };

    auto tok = topic.publish(PAYLOAD);
    REQUIRE(tok);

    auto msg = tok->get_message();
    REQUIRE(msg);

    REQUIRE(TOPIC == msg->get_topic());
    REQUIRE(PAYLOAD == msg->get_payload());
    REQUIRE(QOS == msg->get_qos());
    REQUIRE(RETAINED == msg->is_retained());
}

// ----------------------------------------------------------------------

TEST_CASE("publish full binary", "[topic]")
{
    mqtt::topic topic { cli, TOPIC };

    auto tok = topic.publish(PAYLOAD, QOS, RETAINED);
    REQUIRE(tok);

    auto msg = tok->get_message();
    REQUIRE(msg);

    REQUIRE(TOPIC == msg->get_topic());
    REQUIRE(PAYLOAD == msg->get_payload());
    REQUIRE(QOS == msg->get_qos());
    REQUIRE(RETAINED == msg->is_retained());
}

