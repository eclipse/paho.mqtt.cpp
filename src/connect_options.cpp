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
 *    Frank Pagliughi - Copy and move operations. Bug fixes.
 *******************************************************************************/

#include "mqtt/connect_options.h"
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

const MQTTAsync_connectOptions connect_options::DFLT_C_STRUCT = MQTTAsync_connectOptions_initializer;

connect_options::connect_options() : opts_(DFLT_C_STRUCT)
{
}

connect_options::connect_options(string_ref userName, binary_ref password)
		: connect_options()
{
	set_user_name(userName);
	set_password(password);
}

connect_options::connect_options(const connect_options& opt) : opts_(opt.opts_)
{
	if (opts_.will)
		set_will(opt.will_);

	if (opts_.ssl)
		set_ssl(opt.ssl_);

	set_user_name(opt.userName_);
	set_password(opt.password_);
}

connect_options::connect_options(connect_options&& opt) : opts_(opt.opts_),
						will_(std::move(opt.will_)),
						ssl_(std::move(opt.ssl_)),
						userName_(std::move(opt.userName_)),
						password_(std::move(opt.password_))
{
	if (opts_.will) {
		opts_.will = &will_.opts_;
		opts_.willProperties = const_cast<MQTTProperties*>(&will_.props_.c_struct());
	}

	if (opts_.ssl)
		opts_.ssl = &ssl_.opts_;

	opts_.username = c_str(userName_);
	set_password(password_);
}

connect_options& connect_options::operator=(const connect_options& opt)
{
	opts_ = opt.opts_;

	if (opts_.will)
		set_will(opt.will_);

	if (opts_.ssl)
		set_ssl(opt.ssl_);

	set_user_name(opt.userName_);
	set_password(opt.password_);

	return *this;
}

connect_options& connect_options::operator=(connect_options&& opt)
{
	opts_ = opt.opts_;

	if (opts_.will)
		set_will(std::move(opt.will_));

	if (opts_.ssl)
		set_ssl(std::move(opt.ssl_));

	userName_ = std::move(opt.userName_);
	opts_.username = c_str(userName_);

	password_ = std::move(opt.password_);
	set_password(password_);

	return *this;
}

void connect_options::set_will(const will_options& will)
{
	will_ = will;
	opts_.will = &will_.opts_;
	opts_.willProperties = will_.get_properties().empty()
		? nullptr : const_cast<MQTTProperties*>(&will_.props_.c_struct());
}

void connect_options::set_will(will_options&& will)
{
	will_ = will;
	opts_.will = &will_.opts_;
	opts_.willProperties = will_.get_properties().empty()
		? nullptr : const_cast<MQTTProperties*>(&will_.props_.c_struct());
}

void connect_options::set_user_name(string_ref userName)
{
	userName_ = std::move(userName);
	opts_.username = c_str(userName_);
}

void connect_options::set_password(binary_ref password)
{
	password_ = std::move(password);

	if (password_.empty()) {
		opts_.binarypwd.len = 0;
		opts_.binarypwd.data = nullptr;
	}
	else {
		opts_.binarypwd.len = (int) password_.size();
		opts_.binarypwd.data = password_.data();
	}
}

void connect_options::set_ssl(const ssl_options& ssl)
{
	ssl_ = ssl;
	opts_.ssl = &ssl_.opts_;
}

void connect_options::set_ssl(ssl_options&& ssl)
{
	ssl_ = ssl;
	opts_.ssl = &ssl_.opts_;
}

void connect_options::set_token(const token_ptr& tok)
{
	tok_ = tok;
	opts_.context = tok_.get();

	opts_.onSuccess = nullptr;
	opts_.onFailure = nullptr;

	opts_.onSuccess5 = nullptr;
	opts_.onFailure5 = nullptr;

	if (tok) {
		if (opts_.MQTTVersion < MQTTVERSION_5) {
			opts_.onSuccess = &token::on_success;
			opts_.onFailure = &token::on_failure;
		}
		else {
			opts_.onSuccess5 = &token::on_success5;
			opts_.onFailure5 = &token::on_failure5;
		}
	}
}

void connect_options::set_servers(const_string_collection_ptr serverURIs)
{
	if (serverURIs) {
		serverURIs_ = std::move(serverURIs);
		opts_. serverURIcount = (int) serverURIs_->size();
		opts_.serverURIs = serverURIs_->c_arr();
	}
	else {
		serverURIs_.reset();
		opts_.serverURIcount = 0;
		opts_.serverURIs = nullptr;
	}
}

void connect_options::set_mqtt_version(int mqttVersion) {
	opts_.MQTTVersion = mqttVersion;

	if (mqttVersion < MQTTVERSION_5)
		opts_.cleanstart = 0;
	else
		opts_.cleansession = 0;
}

void connect_options::set_automatic_reconnect(int minRetryInterval,
											  int maxRetryInterval)
{
	opts_.automaticReconnect = to_int(true);
	opts_.minRetryInterval = minRetryInterval;
	opts_.maxRetryInterval = maxRetryInterval;
}


/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt

