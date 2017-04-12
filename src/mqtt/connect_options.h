/////////////////////////////////////////////////////////////////////////////
/// @file connect_options.h
/// Declaration of MQTT connect_options class
/// @date May 1, 2013
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2013-2016 Frank Pagliughi <fpagliughi@mindspring.com>
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
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_connect_options_h
#define __mqtt_connect_options_h

#include "MQTTAsync.h"
#include "mqtt/message.h"
#include "mqtt/topic.h"
#include "mqtt/will_options.h"
#if defined(OPENSSL)
#include "mqtt/ssl_options.h"
#endif
#include "mqtt/token.h"
#include <string>
#include <vector>
#include <memory>

namespace mqtt {

class async_client;
class connect_options_test;
class token_test;

/////////////////////////////////////////////////////////////////////////////

/**
 * Holds the set of options that control how the client connects to a
 * server.
 */
class connect_options
{
	/** The underlying C connection options */
	MQTTAsync_connectOptions opts_;

	/** The LWT options */
	will_options will_;

#if defined(OPENSSL)
	/** The SSL options  */
	ssl_options ssl_;
#endif

	/** The user name to use for the connection. */
	std::string userName_;

	/** The password to use for the connection. */
	std::string password_;

	/** Shared token pointer for context, if any */
	const_token_ptr tok_;

	/** The client has special access */
	friend class async_client;
	friend class connect_options_test;
	friend class token_test;

	/**
	 * Gets a pointer to the C-language NUL-terminated strings for the 
	 * struct. 
	 * @note In the connect options, by default, the Paho C treats 
	 * nullptr char arrays as unset values, so we keep that semantic and 
	 * only set those char arrays if the string is non-empty. 
	 * @param str The C++ string object. 
	 * @return Pointer to a NUL terminated string. This is only valid until 
	 *  	   the next time the string is updated.
	 */
	const char* c_str(const std::string& str) {
		return str.empty() ? nullptr : str.c_str();
	}

public:
	/** Smart/shared pointer to an object of this class. */
	using ptr_t = std::shared_ptr<connect_options>;
	/** Smart/shared pointer to a const object of this class. */
	using const_ptr_t = std::shared_ptr<const connect_options>;

	/**
	 * Constructs a new object using the default values.
	 */
	connect_options();
	/**
	 * Constructs a new object using the specified values.
	 */
	connect_options(const std::string& userName,
					const std::string& password);
	/**
	 * Copy constructor.
	 * @param opt Another object to copy.
	 */
	connect_options(const connect_options& opt);
	/**
	 * Move constructor.
	 * @param opt Another object to move into this new one.
	 */
	connect_options(connect_options&& opt);
	/**
	 * Copy assignment.
	 * @param opt Another object to copy.
	 */
	connect_options& operator=(const connect_options& opt);
	/**
	 * Move assignment.
	 * @param opt Another object to move into this new one.
	 */
	connect_options& operator=(connect_options&& opt);
	/**
	 * Returns the "keep alive" interval.
	 * @return int
	 */
	int get_keep_alive_interval() const {
		return opts_.keepAliveInterval;
	}
	/**
	 * Gets the user name to use for the connection.
	 * @return The user name to use for the connection.
	 */
	const std::string& get_user_name() const { return userName_; }
	/**
	 * Gets the password to use for the connection.
	 * @return The password to use for the connection.
	 */
	const std::string& get_password() const { return password_; }
	/**
	 * Gets the topic to be used for last will and testament (LWT).
	 * @return The topic to be used for last will and testament (LWT).
	 */
	std::string get_will_topic() const {
		return will_.get_topic();
	}
	/**
	 * Gets the message to be sent as last will and testament (LWT).
	 * @return The message to be sent as last will and testament (LWT).
	 */
	const_message_ptr get_will_message() const {
		return will_.get_message();
	}
	/**
	 * Get the LWT options to use for the connection.
	 * @return The LWT options to use for the connection.
	 */
	const will_options& get_will_options() const { return will_; }
#if defined(OPENSSL)
	/**
	 * Get the SSL options to use for the connection.
	 * @return The SSL options to use for the connection.
	 */
	const ssl_options& get_ssl_options() const { return ssl_; }
#endif
	/**
	 * Returns whether the server should remember state for the client
	 * across reconnects.
	 * @return bool
	 */
	bool is_clean_session() const { return opts_.cleansession != 0; }
	/**
      * Gets the version of MQTT to be used on the connect.
	  * @return
	  * @li MQTTVERSION_DEFAULT (0) = default: start with 3.1.1, and if that
	  *     fails, fall back to 3.1
	  * @li MQTTVERSION_3_1 (3) = only try version 3.1
	  * @li MQTTVERSION_3_1_1 (4) = only try version 3.1.1
	  */
	int get_mqtt_version() const { return opts_.MQTTVersion; }
	/**
	 * Sets whether the server should remember state for the client across
	 * reconnects.
	 * @param cleanSession
	 */
	void set_clean_session(bool cleanSession) {
		opts_.cleansession = (cleanSession) ? (!0) : 0;
	}
	/**
	 * Sets the connection timeout value.
	 * @param timeout
	 */
	void set_connection_timeout(int timeout) {
		opts_.connectTimeout = timeout;
	}
	/**
	 * Sets the "keep alive" interval.
	 * @param keepAliveInterval
	 */
	void set_keep_alive_interval(int keepAliveInterval) {
		opts_.keepAliveInterval = keepAliveInterval;
	}
	/**
	 * Sets the password to use for the connection.
	 */
	void set_password(const std::string& password);
	/**
	 * Sets the user name to use for the connection.
	 * @param userName
	 */
	void set_user_name(const std::string& userName);
	/**
	 * Sets the "Last Will and Testament" (LWT) for the connection.
	 * @param will The LWT options.
	 */
	void set_will(const will_options& will);
#if defined(OPENSSL)
	/**
	 * Sets the SSL for the connection.
	 * @param ssl The SSL options.
	 */
	void set_ssl(const ssl_options& ssl);
#endif
	/**
	 * Sets the callback context to a delivery token.
	 * @param tok The delivery token to be used as the callback context.
	 */
	void set_token(const_token_ptr tok);
	/**
      * Sets the version of MQTT to be used on the connect.
	  * @param mqttVersion The MQTT version to use for the connection:
	  *   @li MQTTVERSION_DEFAULT (0) = default: start with 3.1.1, and if
	  *       that fails, fall back to 3.1
	  *   @li MQTTVERSION_3_1 (3) = only try version 3.1
	  *   @li MQTTVERSION_3_1_1 (4) = only try version 3.1.1
	  */
	void set_mqtt_version(int mqttVersion) { opts_.MQTTVersion = mqttVersion; }
	/**
	 * Gets a string representation of the object.
	 * @return
	 */
	std::string to_str() const;
};

/** Smart/shared pointer to a connection options object. */
using connect_options_ptr = connect_options::ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_connect_options_h

