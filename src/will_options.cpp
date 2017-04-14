/*******************************************************************************
 * Copyright (c) 2016 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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
 *    Guilherme M. Ferreira - initial implementation and documentation
 *    Frank Pagliughi - added copy & move operations
 *******************************************************************************/

#include "mqtt/will_options.h"
#include <utility>
#include <cstring>

namespace mqtt {

constexpr MQTTAsync_willOptions will_options::DFLT_C_STRUCT;

/////////////////////////////////////////////////////////////////////////////

will_options::will_options() : opts_(DFLT_C_STRUCT)
{
}

will_options::will_options(const std::string& top,
						   const void *payload, size_t payloadlen,
						   int qos, bool retained)
		: opts_(DFLT_C_STRUCT), topic_(top)
{
	opts_.topicName = c_str(topic_);
	opts_.qos = qos;
	opts_.retained = retained;
	set_payload(byte_buffer(static_cast<const byte*>(payload), payloadlen));
}

will_options::will_options(const topic& top,
						   const void *payload, size_t payloadlen,
						   int qos, bool retained)
		: will_options(top.get_name(), payload, payloadlen, qos, retained)
{
}


will_options::will_options(const std::string& top, byte_buffer payload,
						   int qos, bool retained)
		: opts_(DFLT_C_STRUCT), topic_(top)
{
	opts_.topicName = c_str(topic_);
	opts_.qos = qos;
	opts_.retained = retained;
	set_payload(std::move(payload));
}

will_options::will_options(const std::string& top, const std::string& payload,
						   int qos, bool retained)
		: opts_(DFLT_C_STRUCT), topic_(top)
{
	opts_.topicName = c_str(topic_);
	opts_.qos = qos;
	opts_.retained = retained;
	set_payload(payload);
}

will_options::will_options(const std::string& top, const message& msg)
	: will_options(top, msg.get_payload(), msg.get_qos(), msg.is_retained())
{
}

will_options::will_options(const will_options& opt)
		: opts_(opt.opts_), topic_(opt.topic_)
{
	opts_.topicName = c_str(topic_);
	set_payload(opt.payload_);
}

will_options::will_options(will_options&& other)
		: opts_(other.opts_), topic_(std::move(other.topic_)),
			payload_(std::move(other.payload_))
{
	opts_.topicName = c_str(topic_);
	set_payload(payload_);
	// OPTIMIZE: We probably don't need to do this, but just to be safe
	std::memcpy(&other.opts_, &DFLT_C_STRUCT, sizeof(MQTTAsync_willOptions));
	//other.opts_ = DFLT_C_STRUCT;
}

will_options& will_options::operator=(const will_options& rhs)
{
	if (&rhs != this) {
		std::memcpy(&opts_, &rhs.opts_, sizeof(MQTTAsync_willOptions));

		topic_ = rhs.topic_;
		opts_.topicName = c_str(topic_);

		set_payload(rhs.payload_);
	}
	return *this;
}

will_options& will_options::operator=(will_options&& rhs)
{
	if (&rhs != this) {
		std::memcpy(&opts_, &rhs.opts_, sizeof(MQTTAsync_willOptions));

		topic_ = std::move(rhs.topic_);
		opts_.topicName = c_str(topic_);

		payload_ = std::move(rhs.payload_);
		set_payload(payload_);

		// OPTIMIZE: We probably don't need to do any of this, but just to
		// be safe
		std::memcpy(&rhs.opts_, &DFLT_C_STRUCT, sizeof(MQTTAsync_willOptions));
	}
	return *this;
}

void will_options::set_topic(const std::string& top)
{
	topic_ = top;
	opts_.topicName = c_str(topic_);
}

void will_options::set_payload(byte_buffer msg)
{
	payload_ = std::move(msg);

	// The C struct payload must not be nullptr for will options
	opts_.payload.len = (int) payload_.size();
	opts_.payload.data = payload_.data();
}

/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt

