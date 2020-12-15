// connect_options.cpp

/*******************************************************************************
 * Copyright (c) 2017-2020 Frank Pagliughi <fpagliughi@mindspring.com>
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

const MQTTAsync_connectOptions connect_options::DFLT_C_STRUCT =
		MQTTAsync_connectOptions_initializer;

connect_options::connect_options() : opts_(DFLT_C_STRUCT)
{
}

connect_options::connect_options(string_ref userName, binary_ref password)
		: connect_options()
{
	set_user_name(userName);
	set_password(password);
}

connect_options::connect_options(const connect_options& opt) : opts_(opt.opts_),
						userName_(opt.userName_),
						password_(opt.password_),
						tok_(opt.tok_),
						serverURIs_(opt.serverURIs_),
						props_(opt.props_),
						httpHeaders_(opt.httpHeaders_),
						httpProxy_(opt.httpProxy_),
						httpsProxy_(opt.httpsProxy_)
{
	if (opts_.will)
		set_will(opt.will_);

	if (opts_.ssl)
		set_ssl(opt.ssl_);

	update_c_struct();
}

connect_options::connect_options(connect_options&& opt) : opts_(opt.opts_),
						will_(std::move(opt.will_)),
						ssl_(std::move(opt.ssl_)),
						userName_(std::move(opt.userName_)),
						password_(std::move(opt.password_)),
						tok_(std::move(opt.tok_)),
						serverURIs_(std::move(opt.serverURIs_)),
						props_(std::move(opt.props_)),
						httpHeaders_(std::move(opt.httpHeaders_)),
						httpProxy_(std::move(opt.httpProxy_)),
						httpsProxy_(std::move(opt.httpsProxy_))
{
	if (opts_.will)
		opts_.will = &will_.opts_;

	if (opts_.willProperties)
		opts_.willProperties = const_cast<MQTTProperties*>(&will_.props_.c_struct());

	if (opts_.ssl)
		opts_.ssl = &ssl_.opts_;

	update_c_struct();
}

// Unfortunately, with the existing implementation, there's no way to know
// if the will and ssl options were set by looking at the C++ structs.
// In a major update, we can consider using a pointer or optional<> to
// indicate that they were set.
// But, for now, the copy and assignment operations must handle it manually
// by looking to see if the source C options pointer was set.
void connect_options::update_c_struct()
{
	opts_.username = c_str(userName_);

	// Password

	if (password_.empty()) {
		opts_.binarypwd.len = 0;
		opts_.binarypwd.data = nullptr;
	}
	else {
		opts_.binarypwd.len = (int) password_.size();
		opts_.binarypwd.data = password_.data();
	}

	// Token

	opts_.onSuccess = nullptr;
	opts_.onFailure = nullptr;

	opts_.onSuccess5 = nullptr;
	opts_.onFailure5 = nullptr;

	if (tok_) {
		if (opts_.MQTTVersion < MQTTVERSION_5) {
			opts_.onSuccess = &token::on_success;
			opts_.onFailure = &token::on_failure;
		}
		else {
			opts_.onSuccess5 = &token::on_success5;
			opts_.onFailure5 = &token::on_failure5;
		}
	}

	// Server URIs

	if (!serverURIs_ || serverURIs_->empty()) {
		opts_.serverURIcount = 0;
		opts_.serverURIs = nullptr;
	}
	else {
		opts_. serverURIcount = (int) serverURIs_->size();
		opts_.serverURIs = serverURIs_->c_arr();
	}

	// HTTP & Proxy

	opts_.httpProxy = c_str(httpProxy_);
	opts_.httpsProxy = c_str(httpsProxy_);
}

connect_options& connect_options::operator=(const connect_options& opt)
{
	opts_ = opt.opts_;

	if (opts_.will)
		set_will(opt.will_);

	if (opts_.ssl)
		set_ssl(opt.ssl_);

	userName_ = opt.userName_;
	password_ = opt.password_;

	tok_ = opt.tok_;
	serverURIs_ = opt.serverURIs_;
	props_ = opt.props_;

	httpHeaders_ = opt.httpHeaders_;
	httpProxy_ = opt.httpProxy_;
	httpsProxy_ = opt.httpsProxy_;

	update_c_struct();
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
	password_ = std::move(opt.password_);

	tok_ = std::move(opt.tok_);
	serverURIs_ = std::move(opt.serverURIs_);
	props_ = std::move(opt.props_);

	httpHeaders_ = std::move(opt.httpHeaders_);
	httpProxy_ = std::move(opt.httpProxy_);
	httpsProxy_ = std::move(opt.httpsProxy_);

	update_c_struct();
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

void connect_options::set_http_proxy(const string& httpProxy)
{
	httpProxy_ = httpProxy;
	opts_.httpProxy = c_str(httpProxy_);
}

void connect_options::set_https_proxy(const string& httpsProxy)
{
	httpsProxy_ = httpsProxy;
	opts_.httpsProxy = c_str(httpsProxy_);
}

/////////////////////////////////////////////////////////////////////////////
// connect_data

const MQTTAsync_connectData connect_data::DFLT_C_STRUCT =
		MQTTAsync_connectData_initializer;

connect_data::connect_data() : data_(DFLT_C_STRUCT)
{
}

connect_data::connect_data(string_ref userName)
		: data_(DFLT_C_STRUCT), userName_(userName)
{
	update_c_struct();
}

connect_data::connect_data(string_ref userName, binary_ref password)
		: data_(DFLT_C_STRUCT), userName_(userName), password_(password)
{
	update_c_struct();
}

connect_data::connect_data(const MQTTAsync_connectData& cdata)
		: data_(DFLT_C_STRUCT),
			password_((char*)cdata.binarypwd.data, size_t(cdata.binarypwd.len))
{
	if (cdata.username)
		userName_ = string_ref(cdata.username, strlen(cdata.username));
	update_c_struct();
}

void connect_data::update_c_struct()
{
	data_.username = userName_.empty() ? nullptr : userName_.c_str();

	if (password_.empty()) {
		data_.binarypwd.len = 0;
		data_.binarypwd.data = nullptr;
	}
	else {
		data_.binarypwd.len = (int) password_.size();
		data_.binarypwd.data = password_.data();
	}
}

connect_data& connect_data::operator=(const connect_data& rhs)
{
	if (&rhs != this) {
		userName_ = rhs.userName_;
		password_ = rhs.password_;
		update_c_struct();
	}
	return *this;
}

void connect_data::set_user_name(string_ref userName)
{
	userName_ = std::move(userName);
	update_c_struct();
}

void connect_data::set_password(binary_ref password)
{
	password_ = std::move(password);
	update_c_struct();
}

/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt

