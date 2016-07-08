// message.cpp

/*******************************************************************************
 * Copyright (c) 2013-2016 Frank Pagliughi <fpagliughi@mindspring.com>
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
#include <cstring>
#include <utility>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

message::message() : msg_(MQTTAsync_message_initializer)
{
}

message::message(const void* payload, size_t len)
						: msg_(MQTTAsync_message_initializer)
{
	set_payload(payload, len);
}

message::message(const void* payload, size_t len, QoS qos, bool retained)
						: msg_(MQTTAsync_message_initializer)
{
	set_payload(payload, len);
	set_qos(qos);
	set_retained(retained);
}

message::message(const std::string& payload)
						: msg_(MQTTAsync_message_initializer)
{
	set_payload(payload);
}

message::message(const std::string& payload, QoS qos, bool retained)
						: msg_(MQTTAsync_message_initializer)
{
	set_payload(payload);
	set_qos(qos);
	set_retained(retained);
}

message::message(const MQTTAsync_message& msg) : msg_(msg)
{
	set_payload(msg.payload, msg.payloadlen);
}

message::message(const message& other) : msg_(other.msg_)
{
	set_payload(other.payload_);
}

message::message(message&& other)
		: msg_(other.msg_), payload_(std::move(other.payload_))
{
	msg_.payload = const_cast<char*>(payload_.data());
	msg_.payloadlen = payload_.length();

	other.msg_ = MQTTAsync_message(MQTTAsync_message_initializer);
}

message::~message()
{
	clear_payload();
}

message& message::operator=(const message& rhs)
{
	if (&rhs != this) {
		msg_ = rhs.msg_;
		set_payload(rhs.payload_);
	}
	return *this;
}

message& message::operator=(message&& rhs)
{
	if (&rhs != this) {
		msg_ = rhs.msg_;
		payload_ = std::move(rhs.payload_);

		msg_.payload = const_cast<char*>(payload_.data());
		msg_.payloadlen = payload_.length();

		rhs.msg_ = MQTTAsync_message(MQTTAsync_message_initializer);
	}
	return *this;
}

void message::clear_payload()
{
	payload_.clear();
	msg_.payload = nullptr;
	msg_.payloadlen = 0;
}

void message::set_payload(const void* payload, size_t len)
{
	payload_ = std::string(static_cast<const char*>(payload), len);
	msg_.payload = const_cast<char*>(payload_.data());
	msg_.payloadlen = len;
}

void message::set_payload(const std::string& payload)
{
	payload_ = payload;
	msg_.payload = const_cast<char*>(payload_.data());
	msg_.payloadlen = payload_.length();
}

void message::set_qos(QoS qos)
{
	validate_qos(qos);
	msg_.qos = static_cast<int>(qos);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

