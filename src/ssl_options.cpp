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
 *******************************************************************************/

#include "mqtt/ssl_options.h"

namespace mqtt {


/////////////////////////////////////////////////////////////////////////////

ssl_options::ssl_options()
: opts_( MQTTAsync_SSLOptions_initializer )
{
}

std::string ssl_options::get_trust_store() const
{
	return std::string(opts_.trustStore);
}

std::string ssl_options::get_key_store() const
{
	return std::string(opts_.keyStore);
}

std::string ssl_options::get_private_key() const
{
	return std::string(opts_.privateKey);
}

std::string ssl_options::get_private_key_password() const
{
	return std::string(opts_.privateKeyPassword);
}

std::string ssl_options::get_enabled_cipher_suites() const
{
	return std::string(opts_.enabledCipherSuites);
}

bool ssl_options::get_enable_server_cert_auth() const
{
	return opts_.enableServerCertAuth;
}

void ssl_options::set_trust_store(const std::string& trustStore)
{
	trustStore_ = trustStore;
	opts_.trustStore = trustStore_.c_str();
}

void ssl_options::set_key_store(const std::string& keyStore)
{
	keyStore_ = keyStore;
	opts_.keyStore = keyStore_.c_str();
}

void ssl_options::set_private_key(const std::string& privateKey)
{
	privateKey_ = privateKey;
	opts_.privateKey = privateKey_.c_str();
}

void ssl_options::set_private_key_password(const std::string& privateKeyPassword)
{
	privateKeyPassword_ = privateKeyPassword;
	opts_.privateKeyPassword = privateKeyPassword_.c_str();
}

void ssl_options::set_enabled_cipher_suites(const std::string& enabledCipherSuites)
{
	enabledCipherSuites_ = enabledCipherSuites;
	opts_.enabledCipherSuites = enabledCipherSuites_.c_str();
}

void ssl_options::set_enable_server_cert_auth(bool enableServerCertAuth)
{
	opts_.enableServerCertAuth = enableServerCertAuth;
}

/////////////////////////////////////////////////////////////////////////////

} // end namespace mqtt
