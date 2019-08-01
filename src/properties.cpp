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
	prop_.identifier = ::MQTTPropertyCodes(c);

	switch (::MQTTProperty_getType(prop_.identifier)) {
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
{
	prop_.identifier = ::MQTTPropertyCodes(c);

	size_t n = val.size();
	prop_.value.data.len = int(n);
	prop_.value.data.data = (char*) malloc(n);
	std::memcpy(prop_.value.data.data, val.data(), n);
}

property::property(code c, string_ref name, string_ref val)
{
	prop_.identifier = MQTTPropertyCodes(c);

	size_t n = name.size();
	prop_.value.data.len = int(n);
	prop_.value.data.data = (char*) malloc(n);
	std::memcpy(prop_.value.data.data, name.data(), n);

	n = val.size();
	prop_.value.value.len = int(n);
	prop_.value.value.data = (char*) malloc(n);
	std::memcpy(prop_.value.value.data, val.data(), n);
}

property::property(const MQTTProperty& cprop)
{
	copy(cprop);
}

property::property(MQTTProperty&& cprop)
	:prop_(cprop)
{
	memset(&cprop, 0, sizeof(MQTTProperty));
}


property::property(const property& other)
{
	copy(other.prop_);
}

property::property(property&& other)
{
	std::memcpy(&prop_, &other.prop_, sizeof(MQTTProperty));
	memset(&other.prop_, 0, sizeof(MQTTProperty));
}

property::~property()
{
	switch (::MQTTProperty_getType(prop_.identifier)) {
		case MQTTPROPERTY_TYPE_UTF_8_STRING_PAIR:
			free(prop_.value.value.data);
			// Fall-through

		case MQTTPROPERTY_TYPE_BINARY_DATA:
		case MQTTPROPERTY_TYPE_UTF_8_ENCODED_STRING:
			free(prop_.value.data.data);
			break;

		default:
			// Nothing necessary
			break;
	}
}

void property::copy(const MQTTProperty& cprop)
{
	size_t n;

	std::memcpy(&prop_, &cprop, sizeof(MQTTProperty));

	switch (::MQTTProperty_getType(prop_.identifier)) {
		case MQTTPROPERTY_TYPE_UTF_8_STRING_PAIR:
			n = prop_.value.value.len;
			prop_.value.value.data = (char*) malloc(n);
			memcpy(prop_.value.value.data, cprop.value.value.data, n);
			// Fall-through

		case MQTTPROPERTY_TYPE_BINARY_DATA:
		case MQTTPROPERTY_TYPE_UTF_8_ENCODED_STRING:
			n = prop_.value.data.len;
			prop_.value.data.data = (char*) malloc(n);
			memcpy(prop_.value.data.data, cprop.value.data.data, n);
			break;

		default:
			// Nothing necessary
			break;
	}
}

property& property::operator=(const property& rhs)
{
	if (&rhs != this)
		copy(rhs.prop_);

	return *this;
}

property& property::operator=(property&& rhs)
{
	if (&rhs != this) {
		std::memcpy(&prop_, &rhs.prop_, sizeof(MQTTProperty));
		memset(&rhs.prop_, 0, sizeof(MQTTProperty));
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

properties::properties(std::initializer_list<property> props)
{
	std::memset(&props_, 0, sizeof(properties));
	for (const auto& prop : props)
		::MQTTProperties_add(&props_, &prop.c_struct());
}

void properties::clear()
{
	::MQTTProperties_free(&props_);
	memset(&props_, 0, sizeof(MQTTProperties));
}

property properties::get(property::code propid, size_t idx /*=0*/)
{
	MQTTProperty* prop = MQTTProperties_getPropertyAt(&props_,
									MQTTPropertyCodes(propid), int(idx));
	if (!prop)
		throw bad_cast();

	return property(*prop);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

