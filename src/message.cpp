// message.cpp

/*******************************************************************************
 * Copyright (c) 2013 Frank Pagliughi <fpagliughi@mindspring.com>
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


#include "mqtt/message.h"
#include <string>
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

message::message() : msg_(MQTTAsync_message_initializer)
{
}

message::message(const void* payload, size_t len) 
						: msg_(MQTTAsync_message_initializer)
{
	copy_payload(payload, len);
}

message::message(const std::string& payload) 
						: msg_(MQTTAsync_message_initializer)
{
	copy_payload(payload.data(), payload.length());
}

message::message(const MQTTAsync_message& msg) : msg_(msg)
{
	copy_payload(msg.payload, msg.payloadlen);
}

message::message(const message& other) : msg_(other.msg_)
{
	copy_payload(other.msg_.payload, other.msg_.payloadlen);
}

message::message(message&& other) : msg_(other.msg_)
{
	other.msg_.payload = nullptr;
	other.msg_.payloadlen = 0;
}

message::~message()
{
	clear_payload();
}

void message::copy_payload(const void* payload, size_t len)
{
	if (!payload || len == 0) {
		msg_.payload = nullptr;
		msg_.payloadlen = 0;
	}
	else {
		msg_.payloadlen = len;
		msg_.payload = new char[len];
		std::memcpy(msg_.payload, payload, len);
	}
}

message& message::operator=(const message& rhs)
{
	if (&rhs == this)
		return *this;

	delete[] (char*) msg_.payload;
	msg_ = rhs.msg_;
	copy_payload(rhs.msg_.payload, rhs.msg_.payloadlen);
	return *this;
}

message& message::operator=(message&& rhs)
{
	if (&rhs == this)
		return *this;

	delete[] (char*) msg_.payload;
	msg_ = rhs.msg_;

	rhs.msg_.payload = nullptr;
	rhs.msg_.payloadlen = 0;

	return *this;
}

void message::clear_payload()
{
	delete[] (char*) msg_.payload;
	msg_.payload = nullptr;
	msg_.payloadlen = 0;
}

std::string message::get_payload() const
{
	if (!msg_.payload || msg_.payloadlen == 0)
		return std::string();

	const char *p = (const char *) msg_.payload;
	return std::string(p, p+msg_.payloadlen);
}

void message::set_payload(const void* payload, size_t len)
{
	delete[] (char*) msg_.payload;
	copy_payload(payload, len);
}

void message::set_payload(const std::string& payload)
{
	set_payload(payload.data(), payload.length());
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

