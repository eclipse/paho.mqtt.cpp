// message.cpp

/*******************************************************************************
 * Copyright (c) 2013-2017 Frank Pagliughi <fpagliughi@mindspring.com>
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

constexpr int message::DFLT_QOS;
constexpr bool message::DFLT_RETAINED;
constexpr MQTTAsync_message message::DFLT_C_STRUCT;

// --------------------------------------------------------------------------

message::message() : msg_(DFLT_C_STRUCT)
{
}

message::message(const void* payload, size_t len, int qos/*=DFLT_QOS*/, bool retained/*=DFLT_RETAINED*/)
						: msg_(DFLT_C_STRUCT)
{
	set_payload(payload, len);
	set_qos(qos);
	set_retained(retained);
}

message::message(byte_buffer payload, int qos/*=DFLT_QOS*/, bool retained/*=DFLT_RETAINED*/)
						: msg_(DFLT_C_STRUCT)
{
	set_payload(std::move(payload));
	set_qos(qos);
	set_retained(retained);
}

message::message(const std::string& payload, int qos/*=DFLT_QOS*/, bool retained/*=DFLT_RETAINED*/)
						: msg_(DFLT_C_STRUCT)
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

message::message(message&& other) : msg_(other.msg_)
{
	set_payload(std::move(other.payload_));
	other.msg_ = DFLT_C_STRUCT;
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
		set_payload(std::move(rhs.payload_));
		rhs.msg_ = DFLT_C_STRUCT;
	}
	return *this;
}

void message::clear_payload()
{
	payload_.clear();
	msg_.payload = nullptr;
	msg_.payloadlen = 0;
}

void message::set_payload(byte_buffer payload)
{
	payload_ = std::move(payload);

	if (payload.empty()) {
		msg_.payload = nullptr;
		msg_.payloadlen = 0;
	}
	else {
		msg_.payload = const_cast<byte*>(payload_.data());
		msg_.payloadlen = payload_.length();
	}
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

