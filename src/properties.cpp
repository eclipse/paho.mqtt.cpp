// properties.cpp

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

#include "mqtt/properties.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

property::property(code c, int32_t val)
{
	prop_.identifier = MQTTPropertyCodes(c);
	switch (::MQTTProperty_getType(prop_.identifier = MQTTPropertyCodes(c))) {
		case MQTTPROPERTY_TYPE_BYTE:
			prop_.value.byte = uint8_t(val);
			break;
		case MQTTPROPERTY_TYPE_TWO_BYTE_INTEGER:
			prop_.value.integer2 = int16_t(val);
			break;
		case MQTTPROPERTY_TYPE_FOUR_BYTE_INTEGER:
		case MQTTPROPERTY_TYPE_VARIABLE_BYTE_INTEGER:
			prop_.value.integer4 = val;
			break;
		default:
			// TODO: Throw an exception
			break;
	}
}

property::property(code c, string_ref val)
	: value_(std::move(val))
{
	prop_.identifier = MQTTPropertyCodes(c);
	fixup();
}

property::property(code c, string_ref name, string_ref val)
	: name_(std::move(name)), value_(std::move(val))
{
	prop_.identifier = MQTTPropertyCodes(c);
	fixup();
}


property::property(const property& other)
	: prop_(other.prop_), name_(other.name_), value_(other.value_)
{
	fixup();
}

property::property(property&& other)
	: prop_(other.prop_), name_(std::move(other.name_)), value_(std::move(other.value_))
{
	fixup();
}

void property::fixup()
{
	switch (::MQTTProperty_getType(prop_.identifier)) {
		case MQTTPROPERTY_TYPE_BINARY_DATA:
		case MQTTPROPERTY_TYPE_UTF_8_ENCODED_STRING:
			prop_.value.data.data = const_cast<char*>(value_.data());
			prop_.value.data.len = int(value_.length());
			break;

		case MQTTPROPERTY_TYPE_UTF_8_STRING_PAIR:
			prop_.value.data.data = const_cast<char*>(name_.data());
			prop_.value.data.len = int(name_.length());
			prop_.value.value.data = const_cast<char*>(value_.data());
			prop_.value.value.len = int(value_.length());
			break;

		default:
			// Nothing necessary
			break;
	}
}

property& property::operator=(const property& rhs)
{
	if (&rhs != this) {
		prop_ = rhs.prop_;
		name_ = rhs.name_;
		value_ = rhs.value_;
		fixup();
	}
	return *this;
}

property& property::operator=(property&& rhs)
{
	if (&rhs != this) {
		prop_ = rhs.prop_;
		name_ = std::move(rhs.name_);
		value_ = std::move(rhs.value_);
		fixup();
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

int properties::add(const property& prop)
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

