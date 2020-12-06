/*******************************************************************************
 * Copyright (c) 2016 Guilherme Ferreira <guilherme.maciel.ferreira@gmail.com>
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
 *    Guilherme Ferreira - initial implementation and documentation
 *    Frank Pagliughi - added copy & move operations
 *******************************************************************************/

#include "mqtt/ssl_options.h"
#include <utility>
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

const MQTTAsync_SSLOptions ssl_options::DFLT_C_STRUCT = MQTTAsync_SSLOptions_initializer;

ssl_options::ssl_options() : opts_(DFLT_C_STRUCT)
{
}

ssl_options::ssl_options(const string& trustStore, const string& keyStore,
						 const string& privateKey, const string& privateKeyPassword,
						 const string& enabledCipherSuites, bool enableServerCertAuth)
			: opts_(DFLT_C_STRUCT), trustStore_(trustStore), keyStore_(keyStore),
				privateKey_(privateKey), privateKeyPassword_(privateKeyPassword),
				enabledCipherSuites_(enabledCipherSuites)
{
	update_c_struct();
	opts_.enableServerCertAuth = enableServerCertAuth;
}

ssl_options::ssl_options(const ssl_options& opt)
		: opts_(opt.opts_), trustStore_(opt.trustStore_), keyStore_(opt.keyStore_),
			privateKey_(opt.privateKey_), privateKeyPassword_(opt.privateKeyPassword_),
			enabledCipherSuites_(opt.enabledCipherSuites_)
{
	update_c_struct();
}

ssl_options::ssl_options(ssl_options&& opt)
		: opts_(opt.opts_), trustStore_(std::move(opt.trustStore_)),
			keyStore_(std::move(opt.keyStore_)), privateKey_(std::move(opt.privateKey_)),
			privateKeyPassword_(std::move(opt.privateKeyPassword_)),
			enabledCipherSuites_(std::move(opt.enabledCipherSuites_))
{
	update_c_struct();
}

void ssl_options::update_c_struct()
{
	opts_.trustStore = c_str(trustStore_);
	opts_.keyStore = c_str(keyStore_);
	opts_.privateKey = c_str(privateKey_);
	opts_.privateKeyPassword = c_str(privateKeyPassword_);
	opts_.enabledCipherSuites = c_str(enabledCipherSuites_);

	if (errHandler_) {
		opts_.ssl_error_cb = &ssl_options::on_error;
		opts_.ssl_error_context = this;
	}
	else {
		opts_.ssl_error_cb = nullptr;
		opts_.ssl_error_context = nullptr;
	}

	if (pskHandler_) {
		opts_.ssl_psk_cb = &ssl_options::on_psk;
		opts_.ssl_psk_context = this;
	}
	else {
		opts_.ssl_psk_cb = nullptr;
		opts_.ssl_psk_context = nullptr;
	}
}

// --------------------------------------------------------------------------

int ssl_options::on_error(const char *str, size_t len, void *context)
{
	try {
		if (context && str && len > 0) {
			string errMsg { str, str+len };

			ssl_options* opts = static_cast<ssl_options*>(context);
			auto& errHandler = opts->errHandler_;

			if (errHandler)
				errHandler(errMsg);

			return MQTTASYNC_SUCCESS;
		}
	}
	catch (...) {}

	return MQTTASYNC_FAILURE;
}

unsigned ssl_options::on_psk(const char *hint, char *identity, unsigned max_identity_len,
							 unsigned char *psk, unsigned max_psk_len, void *context)
{
	unsigned ret = 0;

	try {
		if (context) {
			auto hintStr = (hint) ? string(hint) : string();

			ssl_options* opts = static_cast<ssl_options*>(context);
			auto& pskHandler = opts->pskHandler_;

			if (pskHandler) {
				ret = pskHandler(hintStr, identity, size_t(max_identity_len),
								 psk, size_t(max_psk_len));
			}
		}
	}
	catch (...) {}

	return ret;
}

// --------------------------------------------------------------------------

ssl_options& ssl_options::operator=(const ssl_options& rhs)
{
	if (&rhs == this)
		return *this;

	opts_ = rhs.opts_;

	trustStore_ = rhs.trustStore_;
	keyStore_ = rhs.keyStore_;
	privateKey_ = rhs.privateKey_;
	privateKeyPassword_ = rhs.privateKeyPassword_;
	enabledCipherSuites_ = rhs.enabledCipherSuites_;

	update_c_struct();
	return *this;
}

ssl_options& ssl_options::operator=(ssl_options&& rhs)
{
	if (&rhs == this)
		return *this;

	opts_ = rhs.opts_;

	trustStore_ = std::move(rhs.trustStore_);
	keyStore_ = std::move(rhs.keyStore_);
	privateKey_ = std::move(rhs.privateKey_);
	privateKeyPassword_ = std::move(rhs.privateKeyPassword_);
	enabledCipherSuites_ = std::move(rhs.enabledCipherSuites_);

	update_c_struct();
	return *this;
}

// --------------------------------------------------------------------------

void ssl_options::set_trust_store(const string& trustStore)
{
	trustStore_ = trustStore;
	opts_.trustStore = c_str(trustStore_);
}

void ssl_options::set_key_store(const string& keyStore)
{
	keyStore_ = keyStore;
	opts_.keyStore = c_str(keyStore_);
}

void ssl_options::set_private_key(const string& privateKey)
{
	privateKey_ = privateKey;
	opts_.privateKey = c_str(privateKey_);
}

void ssl_options::set_private_key_password(const string& privateKeyPassword)
{
	privateKeyPassword_ = privateKeyPassword;
	opts_.privateKeyPassword = c_str(privateKeyPassword_);
}

void ssl_options::set_enabled_cipher_suites(const string& enabledCipherSuites)
{
	enabledCipherSuites_ = enabledCipherSuites;
	opts_.enabledCipherSuites = c_str(enabledCipherSuites_);
}

void ssl_options::set_enable_server_cert_auth(bool enableServerCertAuth)
{
	opts_.enableServerCertAuth = to_int(enableServerCertAuth);
}

void ssl_options::ca_path(const string& path)
{
	caPath_ = path;
	opts_.CApath = c_str(caPath_);
}

void ssl_options::set_error_handler(error_handler cb)
{
	errHandler_ = cb;

	if (errHandler_) {
		opts_.ssl_error_cb = &ssl_options::on_error;
		opts_.ssl_error_context = this;
	}
	else {
		opts_.ssl_error_cb = nullptr;
		opts_.ssl_error_context = nullptr;
	}
}

void ssl_options::set_psk_handler(psk_handler cb)
{
	pskHandler_ = cb;

	if (pskHandler_) {
		opts_.ssl_psk_cb = &ssl_options::on_psk;
		opts_.ssl_psk_context = this;
	}
	else {
		opts_.ssl_psk_cb = nullptr;
		opts_.ssl_psk_context = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////
} // end namespace mqtt

