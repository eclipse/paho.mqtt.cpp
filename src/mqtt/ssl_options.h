/////////////////////////////////////////////////////////////////////////////
/// @file ssl_options.h
/// Declaration of MQTT ssl_options class
/// @date Jul 7, 2016
/// @author Guilherme Ferreira
/////////////////////////////////////////////////////////////////////////////  

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

#ifndef __mqtt_ssl_options_h
#define __mqtt_ssl_options_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/message.h"
#include "mqtt/topic.h"
#include <string>
#include <vector>
#include <memory>

namespace mqtt {

class connect_options;

/////////////////////////////////////////////////////////////////////////////

/**
 * Holds the set of SSL options for connection.
 */
class ssl_options
{
public:
	/**
	 * Smart/shared pointer to this class.
	 */
	typedef std::shared_ptr<ssl_options> ptr_t;
	/**
	 * Constructs a new MqttConnectOptions object using the default values.
	 */
	ssl_options();

	/**
	 * Returns the file containing the public digital certificates trusted by
	 * the client.
	 * @return std::string
	 */
	std::string get_trust_store() const;

	/**
	 * Returns the file containing the public certificate chain of the client.
	 * @return std::string
	 */
	std::string get_key_store() const;

	/**
	 * Returns the file containing the client's private key.
	 * @return std::string
	 */
	std::string get_private_key() const;

	/**
	 * Returns the password to load the client's privateKey if encrypted.
	 * @return std::string
	 */
	std::string get_private_key_password() const;

	/**
	 * Returns the list of cipher suites that the client will present to the
	 * server during the SSL handshake.
	 * @return std::string
	 */
	std::string get_enabled_cipher_suites() const;

	/**
	 * Returns the true/false to enable verification of the server certificate .
	 * @return bool
	 */
	bool get_enable_server_cert_auth() const;

	/**
	 * Sets the file containing the public digital certificates trusted by
	 * the client.
	 * @param trustStore
	 */
	void set_trust_store(const std::string& trustStore);

	/**
	 * Sets the file containing the public certificate chain of the client.
	 * @param keyStore
	 */
	void set_key_store(const std::string& keyStore);

	/**
	 * Sets the file containing the client's private key.
	 * @param privateKey
	 */
	void set_private_key(const std::string& privateKey) ;

	/**
	 * Sets the password to load the client's privateKey if encrypted.
	 * @param privateKeyPassword
	 */
	void set_private_key_password(const std::string& privateKeyPassword) ;

	/**
	 * Sets the list of cipher suites that the client will present to the server
	 * during the SSL handshake.
	 * @param enabledCipherSuites
	 */
	void set_enabled_cipher_suites(const std::string& enabledCipherSuites);

	/**
	 * Sets the if it's to enable verification of the server certificate.
	 * @param enablServerCertAuth
	 */
	void set_enable_server_cert_auth(bool enablServerCertAuth);

private:
	/** The underlying C SSL options */
	MQTTAsync_SSLOptions opts_;

	/**
	 * The file containing the public digital certificates trusted by
	 * the client.
	 */
	std::string trustStore_;

	/** The file containing the public certificate chain of the client. */
	std::string keyStore_;

	/** The file containing the client's private key. */
	std::string privateKey_;

	/** The password to load the client's privateKey if encrypted. */
	std::string privateKeyPassword_;

	/**
	 * The list of cipher suites that the client will present to the
	 * server during the SSL handshake.
	 */
	std::string enabledCipherSuites_;

	/** The connect options has special access */
	friend class connect_options;
};

/**
 * Shared pointer to the ssl options class.
 */
typedef ssl_options::ptr_t ssl_options_ptr;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_ssl_options_h

