/////////////////////////////////////////////////////////////////////////////
/// @file connect_options.h 
/// Declaration of MQTT connect_options class 
/// @date May 1, 2013 
/// @author Frank Pagliughi 
/////////////////////////////////////////////////////////////////////////////  

/*******************************************************************************
 * Copyright (c) 2013 Frank Pagliughi <fpagliughi@mindspring.com>
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

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/message.h"
#include "mqtt/topic.h"
#include "mqtt/will_options.h"
#include "mqtt/ssl_options.h"
#include <string>
#include <vector>
#include <memory>

namespace mqtt {

class async_client;

/////////////////////////////////////////////////////////////////////////////  

/**
 * Holds the set of options that control how the client connects to a 
 * server. 
 */
class connect_options
{
	/** The underlying C connection options */
	MQTTAsync_connectOptions opts_;

	/** The Will Options **/
	will_options will_;

	/** The SSL Options **/
	ssl_options ssl_;

	/** The password to use for the connection. */
	std::string password_;

	/** The user name to use for the connection. */
	std::string userName_;

	/** The client has special access */
	friend class async_client;

public:
	/**
	 * Smart/shared pointer to this class.
	 */
	typedef std::shared_ptr<connect_options> ptr_t;
	/**
	 * Constructs a new object using the default values.
	 */
	connect_options();
	/**
	 * Returns the connection timeout value. 
	 * @return int 
	 */
	int get_connection_timeout() const;
	/**
	 * Returns the "keep alive" interval.
	 * @return int 
	 */
	int get_keep_alive_interval() const {
		return opts_.keepAliveInterval;
	}
	/**
	 * Returns the password to use for the connection.
	 * @return std::string 
	 */
	std::string get_password() const {
		return std::string(opts_.password);
	}
	/**
	 * Returns the user name to use for the connection.
	 * @return std::string 
	 */
	std::string get_user_name() const { 
		return std::string(opts_.username);
	}
	/**
	 * Returns the topic to be used for last will and testament (LWT).
	 * @return std::string 
	 */
	std::string get_will_topic() const;
	/**
	 * Returns the message to be sent as last will and testament (LWT).
	 * @return MqttMessage 
	 */
	message get_will_message() const;
	/**
	 * Returns whether the server should remember state for the client 
	 * across reconnects.
	 * @return bool 
	 */
	bool is_clean_session() const { return opts_.cleansession != 0; }
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
	/**
	 * Sets the SSL for the connection.
	 * @param ssl The SSL options.
	 */
	void set_ssl(const ssl_options& ssl);

	std::string to_str() const;
};

/**
 * Shared pointer to the connection options class.
 */
typedef connect_options::ptr_t connect_options_ptr;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_connect_options_h

