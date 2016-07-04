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

#include "mqtt/connect_options.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

connect_options::connect_options()
: opts_( MQTTAsync_connectOptions_initializer )
{
	opts_.will = nullptr;
}

std::string connect_options::get_will_topic() const
{
	return will_.get_topic();
}

message connect_options::get_will_message() const
{
	return message(will_.get_message());
}

void connect_options::set_password(const std::string& password)
{
}

void connect_options::set_user_name(const std::string& userName)
{
}

void connect_options::set_will(const will_options& will)
{
	will_.set_topic(will.get_topic());
	will_.set_message(will.get_message());
	will_.set_qos(will.get_qos());
	will_.set_retained(will.get_retained());

	opts_.will = &will_.opts_;
}

/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt
