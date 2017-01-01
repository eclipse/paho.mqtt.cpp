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
		: opts_(MQTTAsync_connectOptions_initializer)
{
	opts_.will = nullptr;
	opts_.ssl = nullptr;
}

connect_options::connect_options(const std::string& userName, const std::string& password)
		: opts_(MQTTAsync_connectOptions_initializer),
			userName_(userName), password_(password)
{
	opts_.will = nullptr;
	opts_.ssl = nullptr;
}

void connect_options::set_password(const std::string& password)
{
	password_ = password;
	opts_.password = password_.empty() ? nullptr : password_.c_str();
}

void connect_options::set_user_name(const std::string& userName)
{
	userName_ = userName;
	opts_.username = userName_.empty() ? nullptr : userName_.c_str();
}

void connect_options::set_will(const will_options& will)
{
	will_ = will;
	opts_.will = &will_.opts_;
}

void connect_options::set_ssl(const ssl_options& ssl)
{
	ssl_ = ssl;
	opts_.ssl = &ssl_.opts_;
}

/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt
