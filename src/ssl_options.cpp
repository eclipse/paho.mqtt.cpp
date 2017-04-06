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

ssl_options::ssl_options() : opts_(MQTTAsync_SSLOptions_initializer)
{
}

ssl_options::ssl_options(
		const std::string& trustStore,
		const std::string& keyStore,
		const std::string& privateKey,
		const std::string& privateKeyPassword,
		const std::string& enabledCipherSuites,
		const bool enableServerCertAuth)
		: opts_(MQTTAsync_SSLOptions_initializer),
		  trustStore_(trustStore),
		  keyStore_(keyStore),
		  privateKey_(privateKey),
		  privateKeyPassword_(privateKeyPassword),
		  enabledCipherSuites_(enabledCipherSuites)
{
	opts_.trustStore = trustStore_.empty() ? nullptr : trustStore_.c_str();
	opts_.keyStore = keyStore_.empty() ? nullptr : keyStore_.c_str();
	opts_.privateKey = privateKey_.empty() ? nullptr : privateKey_.c_str();
	opts_.privateKeyPassword = privateKeyPassword_.empty() ? nullptr : privateKeyPassword_.c_str();
	opts_.enabledCipherSuites = enabledCipherSuites_.empty() ? nullptr : enabledCipherSuites_.c_str();
	opts_.enableServerCertAuth = enableServerCertAuth;
}

ssl_options::ssl_options(const ssl_options& opt)
		: opts_(opt.opts_), trustStore_(opt.trustStore_), keyStore_(opt.keyStore_),
			privateKey_(opt.privateKey_), privateKeyPassword_(opt.privateKeyPassword_),
			enabledCipherSuites_(opt.enabledCipherSuites_)
{
	// NOTE: By default, the Paho C treats nullptr char arrays as unset values,
	// so we keep that semantic and only set those char arrays if the string
	// is non-empty
	opts_.trustStore = trustStore_.empty() ? nullptr : trustStore_.c_str();
	opts_.keyStore = keyStore_.empty() ? nullptr : keyStore_.c_str();
	opts_.privateKey = privateKey_.empty() ? nullptr : privateKey_.c_str();
	opts_.privateKeyPassword = privateKeyPassword_.empty() ? nullptr : privateKeyPassword_.c_str();
	opts_.enabledCipherSuites = enabledCipherSuites_.empty() ? nullptr : enabledCipherSuites_.c_str();
}

ssl_options::ssl_options(ssl_options&& opt)
		: opts_(opt.opts_), trustStore_(std::move(opt.trustStore_)),
			keyStore_(std::move(opt.keyStore_)), privateKey_(std::move(opt.privateKey_)),
			privateKeyPassword_(std::move(opt.privateKeyPassword_)),
			enabledCipherSuites_(std::move(opt.enabledCipherSuites_))
{
	// NOTE: By default, the Paho C treats nullptr char arrays as unset values,
	// so we keep that semantic and only set those char arrays if the string
	// is non-empty
	opts_.trustStore = trustStore_.empty() ? nullptr : trustStore_.c_str();
	opts_.keyStore = keyStore_.empty() ? nullptr : keyStore_.c_str();
	opts_.privateKey = privateKey_.empty() ? nullptr : privateKey_.c_str();
	opts_.privateKeyPassword = privateKeyPassword_.empty() ? nullptr : privateKeyPassword_.c_str();
	opts_.enabledCipherSuites = enabledCipherSuites_.empty() ? nullptr : enabledCipherSuites_.c_str();

	// NOTE: leave the source object "empty" (i.e. with default values)
	MQTTAsync_SSLOptions dfltSSLOpt = MQTTAsync_SSLOptions_initializer;
	std::memcpy(&opt.opts_, &dfltSSLOpt, sizeof(opt.opts_));
}

ssl_options& ssl_options::operator=(const ssl_options& rhs)
{
	if (&rhs == this)
		return *this;

	std::memcpy(&opts_, &rhs.opts_, sizeof(MQTTAsync_SSLOptions));

	trustStore_ = rhs.trustStore_;
	keyStore_ = rhs.keyStore_;
	privateKey_ = rhs.privateKey_;
	privateKeyPassword_ = rhs.privateKeyPassword_;
	enabledCipherSuites_ = rhs.enabledCipherSuites_;

	// NOTE: By default, the Paho C treats nullptr char arrays as unset values,
	// so we keep that semantic and only set those char arrays if the string
	// is non-empty
	opts_.trustStore = trustStore_.empty() ? nullptr : trustStore_.c_str();
	opts_.keyStore = keyStore_.empty() ? nullptr : keyStore_.c_str();
	opts_.privateKey = privateKey_.empty() ? nullptr : privateKey_.c_str();
	opts_.privateKeyPassword = privateKeyPassword_.empty() ? nullptr : privateKeyPassword_.c_str();
	opts_.enabledCipherSuites = enabledCipherSuites_.empty() ? nullptr : enabledCipherSuites_.c_str();

	return *this;
}

ssl_options& ssl_options::operator=(ssl_options&& rhs)
{
	if (&rhs == this)
		return *this;

	std::memcpy(&opts_, &rhs.opts_, sizeof(MQTTAsync_SSLOptions));

	trustStore_ = std::move(rhs.trustStore_);
	keyStore_ = std::move(rhs.keyStore_);
	privateKey_ = std::move(rhs.privateKey_);
	privateKeyPassword_ = std::move(rhs.privateKeyPassword_);
	enabledCipherSuites_ = std::move(rhs.enabledCipherSuites_);

	// NOTE: the correct semantic is to leave the source object
	// "empty" (i.e. with default values)
	MQTTAsync_SSLOptions dfltSSLOpt = MQTTAsync_SSLOptions_initializer;
	std::memcpy(&rhs.opts_, &dfltSSLOpt, sizeof(rhs.opts_));

	// NOTE: By default, the Paho C treats nullptr char arrays as unset values,
	// so we keep that semantic and only set those char arrays if the string
	// is non-empty
	opts_.trustStore = trustStore_.empty() ? nullptr : trustStore_.c_str();
	opts_.keyStore = keyStore_.empty() ? nullptr : keyStore_.c_str();
	opts_.privateKey = privateKey_.empty() ? nullptr : privateKey_.c_str();
	opts_.privateKeyPassword = privateKeyPassword_.empty() ? nullptr : privateKeyPassword_.c_str();
	opts_.enabledCipherSuites = enabledCipherSuites_.empty() ? nullptr : enabledCipherSuites_.c_str();

	return *this;
}

void ssl_options::set_trust_store(const std::string& trustStore)
{
	trustStore_ = trustStore;
	opts_.trustStore = trustStore_.empty() ? nullptr : trustStore_.c_str();
}

void ssl_options::set_key_store(const std::string& keyStore)
{
	keyStore_ = keyStore;
	opts_.keyStore = keyStore_.empty() ? nullptr : keyStore_.c_str();
}

void ssl_options::set_private_key(const std::string& privateKey)
{
	privateKey_ = privateKey;
	opts_.privateKey = privateKey_.empty() ? nullptr : privateKey_.c_str();
}

void ssl_options::set_private_key_password(const std::string& privateKeyPassword)
{
	privateKeyPassword_ = privateKeyPassword;
	opts_.privateKeyPassword = privateKeyPassword_.empty() ? nullptr : privateKeyPassword_.c_str();
}

void ssl_options::set_enabled_cipher_suites(const std::string& enabledCipherSuites)
{
	enabledCipherSuites_ = enabledCipherSuites;
	opts_.enabledCipherSuites = enabledCipherSuites_.empty() ? nullptr : enabledCipherSuites_.c_str();
}

void ssl_options::set_enable_server_cert_auth(bool enableServerCertAuth)
{
	opts_.enableServerCertAuth = enableServerCertAuth ? (!0) : 0;
}

/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt
