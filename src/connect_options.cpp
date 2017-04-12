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
 *    Frannk Pagliughi - Copy and move operations. Bug fixes.
 *******************************************************************************/

#include "mqtt/connect_options.h"
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

connect_options::connect_options() : opts_(MQTTAsync_connectOptions_initializer)
{
}

connect_options::connect_options(const std::string& userName, const std::string& password)
		: connect_options()
{
	set_user_name(userName);
	set_password(password);
}

connect_options::connect_options(const connect_options& opt) : opts_(opt.opts_)
{
	if (opts_.will)
		set_will(opt.will_);

#if defined(OPENSSL)
	if (opts_.ssl)
		set_ssl(opt.ssl_);
#endif

	set_user_name(opt.userName_);
	set_password(opt.password_);
}

connect_options::connect_options(connect_options&& opt) : opts_(opt.opts_),
						will_(std::move(opt.will_)),
#if defined(OPENSSL)
						ssl_(std::move(opt.ssl_)),
#endif
						userName_(std::move(opt.userName_)),
						password_(std::move(opt.password_))
{
	if (opts_.will)
		opts_.will = &will_.opts_;

#if defined(OPENSSL)
	if (opts_.ssl)
		opts_.ssl = &ssl_.opts_;
#endif

	opts_.username = c_str(userName_);
	opts_.password = c_str(password_);
}

connect_options& connect_options::operator=(const connect_options& opt)
{
	std::memcpy(&opts_, &opt.opts_, sizeof(MQTTAsync_connectOptions));

	if (opts_.will)
		set_will(opt.will_);

#if defined(OPENSSL)
	if (opts_.ssl)
		set_ssl(opt.ssl_);
#endif

	set_user_name(opt.userName_);
	set_password(opt.password_);

	return *this;
}

connect_options& connect_options::operator=(connect_options&& opt)
{
	std::memcpy(&opts_, &opt.opts_, sizeof(MQTTAsync_connectOptions));

	will_ = std::move(opt.will_);
	userName_ = std::move(opt.userName_);
	password_ = std::move(opt.password_);

	if (opts_.will)
		opts_.will = &will_.opts_;

#if defined(OPENSSL)
	ssl_ = std::move(opt.ssl_);
	if (opts_.ssl)
		opts_.ssl = &ssl_.opts_;
#endif

	opts_.username = c_str(userName_);
	opts_.password = c_str(password_);

	return *this;
}


void connect_options::set_password(const std::string& password)
{
	password_ = password;
	opts_.password = c_str(password_);
}

void connect_options::set_user_name(const std::string& userName)
{
	userName_ = userName;
	opts_.username = c_str(userName_);
}

void connect_options::set_will(const will_options& will)
{
	will_ = will;
	opts_.will = &will_.opts_;
}

#if defined(OPENSSL)
void connect_options::set_ssl(const ssl_options& ssl)
{
	ssl_ = ssl;
	opts_.ssl = &ssl_.opts_;
}
#endif

void connect_options::set_token(const_token_ptr tok)
{
	tok_ = tok;
	opts_.context = const_cast<token*>(tok.get());

	if (tok) {
		opts_.onSuccess = &token::on_success;
		opts_.onFailure = &token::on_failure;
	}
	else {
		opts_.onSuccess = nullptr;
		opts_.onFailure = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt

