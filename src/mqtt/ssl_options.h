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
 *    Frank Pagliughi - added copy & move operations
 *******************************************************************************/

#ifndef __mqtt_ssl_options_h
#define __mqtt_ssl_options_h

#include "MQTTAsync.h"
#include "mqtt/message.h"
#include "mqtt/topic.h"
#include "mqtt/types.h"
#include <vector>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Holds the set of SSL options for connection.
 */
class ssl_options
{
	/** The default C struct */
	static const MQTTAsync_SSLOptions DFLT_C_STRUCT ;

	/** The underlying C SSL options */
	MQTTAsync_SSLOptions opts_;

	/**
	 * The file containing the public digital certificates trusted by
	 * the client.
	 */
	string trustStore_;

	/** The file containing the public certificate chain of the client. */
	string keyStore_;

	/** The file containing the client's private key. */
	string privateKey_;

	/** The password to load the client's privateKey if encrypted. */
	string privateKeyPassword_;

	/**
	 * The list of cipher suites that the client will present to the
	 * server during the SSL handshake.
	 */
	string enabledCipherSuites_;

	/** The connect options has special access */
	friend class connect_options;
	friend class connect_options_test;
	friend class ssl_options_test;

	/**
	 * Gets a pointer to the C-language NUL-terminated strings for the
	 * struct.
	 * @note In the SSL options, by default, the Paho C treats nullptr char
	 * arrays as unset values, so we keep that semantic and only set those
	 * char arrays if the string is non-empty.
	 * @param str The C++ string object.
	 * @return Pointer to a NUL terminated string. This is only valid until
	 *  	   the next time the string is updated.
	 */
	const char* c_str(const string& str) {
		return str.empty() ? nullptr : str.c_str();
	}
	/**
	 * Updates the underlying C structure to match our strings.
	 */
	void update_c_struct();

public:
	/** Smart/shared pointer to an object of this class. */
	using ptr_t = std::shared_ptr<ssl_options>;
	/** Smart/shared pointer to a const object of this class. */
	using const_ptr_t = std::shared_ptr<const ssl_options>;

	/**
	 * Constructs a new MqttConnectOptions object using the default values.
	 */
	ssl_options();
	/**
	 * Argument constructor.
	 * @param trustStore The file containing the public digital certificates
	 * trusted by the client.
	 * @param keyStore The file containing the public certificate chain of the
	 * client.
	 * @param privateKey The file containing the client's private key.
	 * @param privateKeyPassword The password to load the client's privateKey
	 * if encrypted.
	 * @param enabledCipherSuites The list of cipher suites that the client
	 * will present to the server during the SSL handshake.
	 * @param enableServerCertAuth True/False option to enable verification of
	 * the server certificate
	 */
	ssl_options(const string& trustStore, const string& keyStore,
				const string& privateKey, const string& privateKeyPassword,
				const string& enabledCipherSuites, bool enableServerCertAuth);
	/**
	 * Copy constructor.
	 * @param opt The other options to copy.
	 */
	ssl_options(const ssl_options& opt);
	/**
	 * Move constructor.
	 * @param opt The other options to move to this one.
	 */
	ssl_options(ssl_options&& opt);
	/**
	 * Copy assignment.
	 * @param opt The other options to copy.
	 * @return A reference to this object.
	 */
	ssl_options& operator=(const ssl_options& opt);
	/**
	 * Move assignment.
	 * @param opt The other options to move to this one.
	 * @return A reference to this object.
	 */
	ssl_options& operator=(ssl_options&& opt);
	/**
	 * Returns the file containing the public digital certificates trusted by
	 * the client.
	 * @return string
	 */
	string get_trust_store() const { return trustStore_; }
	/**
	 * Returns the file containing the public certificate chain of the client.
	 * @return string
	 */
	string get_key_store() const { return keyStore_; }
	/**
	 * Returns the file containing the client's private key.
	 * @return string
	 */
	string get_private_key() const { return privateKey_; }
	/**
	 * Returns the password to load the client's privateKey if encrypted.
	 * @return string
	 */
	string get_private_key_password() const { return privateKeyPassword_; }
	/**
	 * Returns the list of cipher suites that the client will present to the
	 * server during the SSL handshake.
	 * @return string
	 */
	string get_enabled_cipher_suites() const { return enabledCipherSuites_; }
	/**
	 * Returns the true/false to enable verification of the server certificate .
	 * @return bool
	 */
	bool get_enable_server_cert_auth() const {
		return to_bool(opts_.enableServerCertAuth);
	}
	/**
	 * Sets the file containing the public digital certificates trusted by
	 * the client.
	 * @param trustStore The file in PEM format containing the public
	 *  				 digital certificates trusted by the client.
	 */
	void set_trust_store(const string& trustStore);
	/**
	 * Sets the file containing the public certificate chain of the client.
	 * @param keyStore The file in PEM format containing the public
	 *  			   certificate chain of the client. It may also include
	 *				   the client's private key.
	 */
	void set_key_store(const string& keyStore);
	/**
	 * Sets the file containing the client's private key.
	 * @param privateKey If not included in the sslKeyStore, this is the
	 *  				 file in PEM format containing the client's private
	 *  				 key.
	 */
	void set_private_key(const string& privateKey);
	/**
	 * Sets the password to load the client's privateKey if encrypted.
	 * @param privateKeyPassword The password to load the privateKey if
	 *  						 encrypted.
	 */
	void set_private_key_password(const string& privateKeyPassword);
	/**
	 * Sets the list of cipher suites that the client will present to the server
	 * during the SSL handshake.
	 * @param enabledCipherSuites The list of cipher suites that the client
	 *  						  will present to the server during the SSL
	 *  						  handshake. For a  full explanation of the
	 *  						  cipher list format, please see the OpenSSL
	 *  						  on-line documentation:
	 *  						  http://www.openssl.org/docs/apps/ciphers.html#CIPHER_LIST_FORMAT
	 *  						  If this setting is ommitted, its default
	 *  						  value will be "ALL", that is, all the
	 *  						  cipher suites -excluding those offering no
	 *  						  encryption- will be considered. This
	 *  						  setting can be used to set an SSL
	 *  						  anonymous connection (empty string value,
	 *  						  for instance).
	 */
	void set_enabled_cipher_suites(const string& enabledCipherSuites);
	/**
	 * Enables or disables verification of the server certificate.
	 * @param enablServerCertAuth enable/disable verification of the server
	 *  						  certificate
	 */
	void set_enable_server_cert_auth(bool enablServerCertAuth);
};

/**
 * Shared pointer to the ssl options class.
 */
using ssl_options_ptr = ssl_options::ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_ssl_options_h

