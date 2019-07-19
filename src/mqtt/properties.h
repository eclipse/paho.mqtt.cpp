/////////////////////////////////////////////////////////////////////////////
/// @file properties.h
/// Declaration of MQTT properties class
/// @date July 7, 2019
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2019 Frank Pagliughi <fpagliughi@mindspring.com>
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

#ifndef __mqtt_properties_h
#define __mqtt_properties_h

#include <tuple>
extern "C" {
	#include "MQTTProperties.h"
}

#include "mqtt/types.h"
#include "mqtt/buffer_ref.h"


namespace mqtt {

using string_pair = std::tuple<string, string>;

/////////////////////////////////////////////////////////////////////////////

class property
{
	/** The underlying C property struct */
	MQTTProperty prop_;

	/** The name field for string pairs, like user properties */
	string_ref name_;

	/** A string or binary value */
	string_ref value_;

    // Friends
	template <typename T>
	friend T get(const property&);

	friend class properties;

    // Fixes the pointers in the underlying C prop_
	void fixup();

public:
	enum code {
        PAYLOAD_FORMAT_INDICATOR = 1,
        MESSAGE_EXPIRY_INTERVAL = 2,
        CONTENT_TYPE = 3,
        RESPONSE_TOPIC = 8,
        CORRELATION_DATA = 9,
        SUBSCRIPTION_IDENTIFIER = 11,
        SESSION_EXPIRY_INTERVAL = 17,
        ASSIGNED_CLIENT_IDENTIFER = 18,
        SERVER_KEEP_ALIVE = 19,
        AUTHENTICATION_METHOD = 21,
        AUTHENTICATION_DATA = 22,
        REQUEST_PROBLEM_INFORMATION = 23,
        WILL_DELAY_INTERVAL = 24,
        REQUEST_RESPONSE_INFORMATION = 25,
        RESPONSE_INFORMATION = 26,
        SERVER_REFERENCE = 28,
        REASON_STRING = 31,
        RECEIVE_MAXIMUM = 33,
        TOPIC_ALIAS_MAXIMUM = 34,
        TOPIC_ALIAS = 35,
        MAXIMUM_QOS = 36,
        RETAIN_AVAILABLE = 37,
        USER_PROPERTY = 38,
        MAXIMUM_PACKET_SIZE = 39,
        WILDCARD_SUBSCRIPTION_AVAILABLE = 40,
        SUBSCRIPTION_IDENTIFIERS_AVAILABLE = 41,
        SHARED_SUBSCRIPTION_AVAILABLE = 42
	};

	/**
	 * Create a numeric property.
	 * This can be a byte, or 2-byte, 4-byte, or variable byte integer.
	 * @param c The property code
	 * @param val The integer value for the property
	 */
	property(code c, int32_t val);
	/**
	 * Create a string or binary property.
	 * @param c The property code
	 * @param val The value for the property
	 */
	property(code c, string_ref val);
	/**
	 * Create a string pair property.
	 * @param c The property code
	 * @param name The string name for the property
	 * @param val The string value for the property
	 */
	property(code c, string_ref name, string_ref val);

	property(const property& other);
	property(property&& other);

	property& operator=(const property& rhs);
	property& operator=(property&& rhs);

	/**
	 * Returns the underlying C property struct.
	 * @return The underlying C property struct.
	 */
	const MQTTProperty& prop() const {
		return prop_;
	}
	/**
	 * Gets the property type (identifier).
	 * @return The code for the property type.
	 */
	code type() const {
		return code(prop_.identifier);
	}
	/**
	 * Gets a printable name for the property type.
	 * @return A printable name for the property type.
	 */
	const char* type_name() const {
		return ::MQTTPropertyName(prop_.identifier);
	}
};

// TODO: This shoult throw an exception
template <typename T>
T get(const property&) { return T(); }

template <>
uint8_t get<uint8_t>(const property& prop) {
	return (uint8_t) prop.prop_.value.byte;
}

template <>
uint16_t get<uint16_t>(const property& prop) {
	return (uint16_t) prop.prop_.value.integer2;
}

template <>
int16_t get<int16_t>(const property& prop) {
	return (int16_t) prop.prop_.value.integer2;
}

template <>
uint32_t get<uint32_t>(const property& prop) {
	return (uint32_t) prop.prop_.value.integer4;
}

template <>
int32_t get<int32_t>(const property& prop) {
	return (int32_t) prop.prop_.value.integer4;
}

template <>
string get<string>(const property& prop) {
	static const string EMPTY_STRING;
	return prop.value_ ? prop.value_.str() : EMPTY_STRING;
}

template <>
string_pair get<string_pair>(const property& prop) {
	static const string EMPTY_STRING;
	auto name = prop.name_ ? prop.name_.str() : EMPTY_STRING;
	auto val = prop.value_ ? prop.value_.str() : EMPTY_STRING;
	return std::make_tuple(std::move(name), std::move(val));
}

/////////////////////////////////////////////////////////////////////////////

/**
 * MQTT v5 property list.
 *
 * A collection of properties that can be added to outgoing packets or
 * retrieved from incoming packets.
 */
class properties
{
	MQTTProperties props_;

public:
	properties() : props_(MQTTProperties_initializer) {}

	~properties() {}

	int add(const property& prop);
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_properties_h

