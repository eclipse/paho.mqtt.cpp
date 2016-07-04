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
 *******************************************************************************/

#include "mqtt/will_options.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

will_options::will_options()
: opts_( MQTTAsync_willOptions_initializer )
{
}

will_options::will_options(
	const topic& top,
	const void *payload,
	const size_t payload_len,
	const int qos,
	const bool retained)
: opts_( MQTTAsync_willOptions_initializer ),
  topic_(top.get_name()),
  message_(static_cast<const char *>(payload), payload_len)
{
	opts_.topicName = topic_.c_str();
	opts_.message = message_.c_str();
	opts_.qos = qos;
	opts_.retained = retained;
}

will_options::will_options(
	const std::string& top,
	const void *payload,
	const size_t payload_len,
	const int qos,
	const bool retained)
: opts_( MQTTAsync_willOptions_initializer ),
  topic_(top),
  message_(static_cast<const char *>(payload), payload_len)
{
	opts_.topicName = topic_.c_str();
	opts_.message = message_.c_str();
	opts_.qos = qos;
	opts_.retained = retained;
}


will_options::will_options(
	const std::string& top,
	const message_ptr msg,
	const int qos,
	const bool retained)
: opts_( MQTTAsync_willOptions_initializer ),
  topic_(top),
  message_(msg->get_payload())
{
	opts_.topicName = topic_.c_str();
	opts_.message = message_.c_str();
	opts_.qos = qos;
	opts_.retained = retained;
}

will_options::~will_options()
{
}

void will_options::set_topic(const std::string& top)
{
	topic_ = top;
	opts_.topicName = topic_.c_str();
}

void will_options::set_message(const std::string& msg)
{
	message_ = msg;
	opts_.message = message_.c_str();
}

/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt
