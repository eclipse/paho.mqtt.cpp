// test_properties.cpp
//
// Catch2 unit tests for the 'property' and 'properties' classes in the
// Eclipse Paho MQTT C++ library.
//

#include <cstring>
#include "catch2/catch.hpp"
#include "mqtt/properties.h"

using namespace mqtt;

TEST_CASE("int property constructor", "[property]") {
    // This is a known byte property
    SECTION("creating a byte property") {
		property::code typ = property::PAYLOAD_FORMAT_INDICATOR;

		property prop { typ, 42 };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_PAYLOAD_FORMAT_INDICATOR);
		REQUIRE(prop.prop().value.byte == 42);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<uint8_t>(prop) == uint8_t(42));
    }

	SECTION("creating a bad byte property") {
		// TODO: Test constructor for out of range input
	}

    // This is a known 2-byte integer property
    SECTION("creating an int2 property") {
		property::code typ = property::TOPIC_ALIAS;

		property prop { typ, 512 };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_TOPIC_ALIAS);
		REQUIRE(prop.prop().value.integer2 == 512);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<int16_t>(prop) == int16_t(512));
    }

	SECTION("creating a bad int2 property") {
		// TODO: Test constructor for out of range input
	}

    // This is a known 4-byte integer property
    SECTION("creating an int4 property") {
		property::code typ = property::MESSAGE_EXPIRY_INTERVAL;

		property prop { typ, 70000 };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_MESSAGE_EXPIRY_INTERVAL);
		REQUIRE(prop.prop().value.integer4 == 70000);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<int32_t>(prop) == int32_t(70000));
    }

	SECTION("creating a bad int2 property") {
		// TODO: Test constructor for out of range input
	}
}

TEST_CASE("string property constructor", "[property]") {
	property::code typ = property::RESPONSE_TOPIC;

	SECTION("property from string") {
		string topic { "replies/bubba" };

		property prop { typ, topic };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_RESPONSE_TOPIC);
		REQUIRE(prop.prop().value.data.len == int(topic.length()));
		REQUIRE(std::strcmp(prop.prop().value.data.data, topic.c_str()) == 0);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<string>(prop) == topic);
	}

	SECTION("property from c-string") {
		const char* topic = "replies/bubba";

		property prop { typ, topic };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_RESPONSE_TOPIC);
		REQUIRE(prop.prop().value.data.len == strlen(topic));
		REQUIRE(std::strcmp(prop.prop().value.data.data, topic) == 0);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<string>(prop) == string(topic));
	}
}


TEST_CASE("binary property constructor", "[property]") {
	SECTION("property from binary") {
		property::code typ = property::CORRELATION_DATA;
		const size_t LEN = 5;
		binary corr_id { "\x00\x01\x02\x03\x04", LEN };

		property prop { property::CORRELATION_DATA, corr_id };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_CORRELATION_DATA);
		REQUIRE(prop.prop().value.data.len == int(corr_id.length()));
		REQUIRE(std::memcmp(prop.prop().value.data.data, corr_id.data(), LEN) == 0);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<binary>(prop) == corr_id);
	}
}


TEST_CASE("string pair property constructor", "[property]") {
	property::code typ = property::USER_PROPERTY;

	SECTION("property from strings") {
		string	name { "bubba" },
				value { "some val" };

		property prop { typ, name, value };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_USER_PROPERTY);

		REQUIRE(prop.prop().value.data.len == int(name.length()));
		REQUIRE(std::strcmp(prop.prop().value.data.data, name.c_str()) == 0);

		REQUIRE(prop.prop().value.value.len == int(value.length()));
		REQUIRE(std::strcmp(prop.prop().value.value.data, value.c_str()) == 0);

		REQUIRE(prop.type() == typ);

		auto usr = get<string_pair>(prop);
		REQUIRE(std::get<0>(usr) == name);
		REQUIRE(std::get<1>(usr) == value);
	}

	SECTION("property from c-strings") {
		const char* name = "bubba";
		const char* value = "some val";

		property prop { typ, name, value };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_USER_PROPERTY);

		REQUIRE(prop.prop().value.data.len == strlen(name));
		REQUIRE(std::strcmp(prop.prop().value.data.data, name) == 0);

		REQUIRE(prop.prop().value.value.len == strlen(value));
		REQUIRE(std::strcmp(prop.prop().value.value.data, value) == 0);

		REQUIRE(prop.type() == typ);

		auto usr = get<string_pair>(prop);
		REQUIRE(std::get<0>(usr) == string(name));
		REQUIRE(std::get<1>(usr) == string(value));
	}
}

TEST_CASE("int property copy constructor", "[property]") {
    SECTION("copy an int4 property") {
		property::code typ = property::MESSAGE_EXPIRY_INTERVAL;

		property org_prop { typ, 70000 };
		property prop { org_prop };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_MESSAGE_EXPIRY_INTERVAL);
		REQUIRE(prop.prop().value.integer4 == 70000);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<int32_t>(prop) == int32_t(70000));
    }
}

TEST_CASE("int property move constructor", "[property]") {
    SECTION("move an int4 property") {
		property::code typ = property::MESSAGE_EXPIRY_INTERVAL;

		property org_prop { typ, 70000 };
		property prop { std::move(org_prop) };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_MESSAGE_EXPIRY_INTERVAL);
		REQUIRE(prop.prop().value.integer4 == 70000);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<int32_t>(prop) == int32_t(70000));

		// Make sure the old value was moved
		REQUIRE(org_prop.prop().identifier == 0);
		REQUIRE(org_prop.prop().value.integer4 == 0);
    }
}

TEST_CASE("string property copy constructor", "[property]") {
	SECTION("copy a string property") {
		property::code typ = property::RESPONSE_TOPIC;
		string topic { "replies/bubba" };

		property org_prop { typ, topic };
		property prop { org_prop };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_RESPONSE_TOPIC);

		REQUIRE(prop.prop().value.data.len == int(topic.length()));
		REQUIRE(std::strcmp(prop.prop().value.data.data, topic.c_str()) == 0);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<string>(prop) == topic);
	}
}

TEST_CASE("string property move constructor", "[property]") {
	SECTION("copy a string property") {
		property::code typ = property::RESPONSE_TOPIC;
		string topic { "replies/bubba" };

		property org_prop { typ, topic };
		property prop { std::move(org_prop) };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_RESPONSE_TOPIC);

		REQUIRE(prop.prop().value.data.len == int(topic.length()));
		REQUIRE(std::strcmp(prop.prop().value.data.data, topic.c_str()) == 0);

		REQUIRE(prop.type() == typ);
		REQUIRE(get<string>(prop) == topic);

		// Make sure the old value was moved
		REQUIRE(org_prop.prop().identifier == 0);
		REQUIRE(org_prop.prop().value.data.len == 0);
		REQUIRE(org_prop.prop().value.data.data == nullptr);
	}
}

TEST_CASE("string pair property copy constructor", "[property]") {
	property::code typ = property::USER_PROPERTY;

	SECTION("property from strings") {
		string	name { "bubba" },
				value { "some val" };

		property org_prop { typ, name, value };
		property prop { org_prop };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_USER_PROPERTY);

		REQUIRE(prop.prop().value.data.len == int(name.length()));
		REQUIRE(std::strcmp(prop.prop().value.data.data, name.c_str()) == 0);

		REQUIRE(prop.prop().value.value.len == int(value.length()));
		REQUIRE(std::strcmp(prop.prop().value.value.data, value.c_str()) == 0);

		REQUIRE(prop.type() == typ);

		auto usr = get<string_pair>(prop);
		REQUIRE(std::get<0>(usr) == name);
		REQUIRE(std::get<1>(usr) == value);
	}
}


TEST_CASE("string pair property move constructor", "[property]") {
	property::code typ = property::USER_PROPERTY;

	SECTION("property from strings") {
		string	name { "bubba" },
				value { "some val" };

		property org_prop { typ, name, value };
		property prop { std::move(org_prop) };

		REQUIRE(prop.prop().identifier == MQTTPROPERTY_CODE_USER_PROPERTY);

		REQUIRE(prop.prop().value.data.len == int(name.length()));
		REQUIRE(std::strcmp(prop.prop().value.data.data, name.c_str()) == 0);

		REQUIRE(prop.prop().value.value.len == int(value.length()));
		REQUIRE(std::strcmp(prop.prop().value.value.data, value.c_str()) == 0);

		REQUIRE(prop.type() == typ);

		auto usr = get<string_pair>(prop);
		REQUIRE(std::get<0>(usr) == name);
		REQUIRE(std::get<1>(usr) == value);

		// Make sure the old value was moved
		REQUIRE(org_prop.prop().identifier == 0);
		REQUIRE(org_prop.prop().value.data.len == 0);
		REQUIRE(org_prop.prop().value.data.data == nullptr);
		REQUIRE(org_prop.prop().value.value.len == 0);
		REQUIRE(org_prop.prop().value.value.data == nullptr);
	}
}

