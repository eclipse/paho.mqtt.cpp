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

#if __cplusplus < 201703L
	constexpr int message::DFLT_QOS;
	constexpr bool message::DFLT_RETAINED;
#endif

const MQTTAsync_message message::DFLT_C_STRUCT = MQTTAsync_message_initializer;

// --------------------------------------------------------------------------

message::message() : msg_(DFLT_C_STRUCT)
{
}

message::message(string_ref topic, const void* payload, size_t len, int qos, bool retained)
						: msg_(DFLT_C_STRUCT), topic_(std::move(topic))
{
	set_payload(payload, len);
	set_qos(qos);
	set_retained(retained);
}

message::message(string_ref topic, binary_ref payload, int qos, bool retained)
						: msg_(DFLT_C_STRUCT), topic_(std::move(topic))
{
	set_payload(std::move(payload));
	set_qos(qos);
	set_retained(retained);
}

message::message(string_ref topic, const MQTTAsync_message& msg)
						: msg_(msg), topic_(std::move(topic)), props_(msg.properties)
{
	msg_.properties = props_.c_struct();
	set_payload(msg.payload, msg.payloadlen);
}

message::message(const message& other) : msg_(other.msg_), topic_(other.topic_)
{
	set_payload(other.payload_);
}

message::message(message&& other) : msg_(other.msg_), topic_(std::move(other.topic_))
{
	set_payload(std::move(other.payload_));
	other.msg_.payloadlen = 0;
	other.msg_.payload = nullptr;
}

message& message::operator=(const message& rhs)
{
	if (&rhs != this) {
		msg_ = rhs.msg_;
		topic_ = rhs.topic_;
		set_payload(rhs.payload_);
	}
	return *this;
}

message& message::operator=(message&& rhs)
{
	if (&rhs != this) {
		msg_ = rhs.msg_;
		topic_ = std::move(rhs.topic_);
		set_payload(std::move(rhs.payload_));
		rhs.msg_.payloadlen = 0;
		rhs.msg_.payload = nullptr;
	}
	return *this;
}

void message::clear_payload()
{
	payload_.reset();
	msg_.payload = nullptr;
	msg_.payloadlen = 0;
}

void message::set_payload(binary_ref payload)
{
	payload_ = std::move(payload);

	if (payload_.empty()) {
		msg_.payload = nullptr;
		msg_.payloadlen = 0;
	}
	else {
		msg_.payload = const_cast<binary_ref::value_type*>(payload_.data());
		msg_.payloadlen = payload_.length();
	}
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

