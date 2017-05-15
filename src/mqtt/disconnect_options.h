/////////////////////////////////////////////////////////////////////////////
/// @file disconnect_options.h
/// Implementation of the class 'disconnect_options'
/// @date 26-Aug-2016
/////////////////////////////////////////////////////////////////////////////

/****************************************************************************
 * Copyright (c) 2016-2017 Frank Pagliughi <fpagliughi@mindspring.com>
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
 ***************************************************************************/

#ifndef __mqtt_disconnect_options_h
#define __mqtt_disconnect_options_h

#include "MQTTAsync.h"
#include "mqtt/token.h"
#include <chrono>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Options for disconnecting from an MQTT broker.
 */
class disconnect_options
{
	/** The default C struct */
	static const MQTTAsync_disconnectOptions DFLT_C_STRUCT;

	/** The underlying C disconnect options */
	MQTTAsync_disconnectOptions opts_;

	/** Shared token pointer for context, if any */
	token_ptr tok_;

	/** The client has special access */
	friend class async_client;
	friend class disconnect_options_test;

public:
	/**
	 * Create an empty delivery response object.
	 */
	disconnect_options();
	/**
	 * Creates disconnect options tied to the specific token.
	 * @param timeout The timeout (in milliseconds).
	 * @param tok A token to be used as the context.
	 */
	disconnect_options(int timeout, const token_ptr& tok);
	/**
	 * Creates disconnect options tied to the specific token.
	 * @param to The timeout.
	 * @param tok A token to be used as the context.
	 */
	template <class Rep, class Period>
	disconnect_options(const std::chrono::duration<Rep, Period>& to,
					   const token_ptr& tok) : disconnect_options() {
		set_timeout(to);
		set_token(tok);
	}
	/**
	 * Copy constructor.
	 * @param opt Another object to copy.
	 */
	disconnect_options(const disconnect_options& opt);
	/**
	 * Move constructor.
	 * @param opt Another object to move into this new one.
	 */
	disconnect_options(disconnect_options&& opt);
	/**
	 * Copy assignment.
	 * @param opt Another object to copy.
	 */
	disconnect_options& operator=(const disconnect_options& opt);
	/**
	 * Move assignment.
	 * @param opt Another object to move into this new one.
	 */
	disconnect_options& operator=(disconnect_options&& opt);
	/**
	 * Gets the timeout used for disconnecting.
	 * @return The timeout for disconnecting (in milliseconds).
	 */
	std::chrono::milliseconds get_timeout() const {
		return std::chrono::milliseconds(opts_.timeout);
	}
	/**
	 * Sets the timeout for disconnecting.
	 * This allows for any remaining in-flight messages to be delivered.
	 * @param timeout The timeout (in milliseconds).
	 */
	void set_timeout(int timeout) { opts_.timeout = timeout; }
	/**
	 * Sets the connect timeout with a chrono duration.
	 * This is the maximum time that the underlying library will wait for a
	 * connection before failing.
	 * @param to The connect timeout in seconds.
	 */
	template <class Rep, class Period>
	void set_timeout(const std::chrono::duration<Rep, Period>& to) {
		// TODO: check range
		set_timeout((int) to_milliseconds_count(to));
	}
	/**
	 * Sets the callback context to a delivery token.
	 * @param tok The delivery token to be used as the callback context.
	 */
	void set_token(const token_ptr& tok);
	/**
	 * Gets the callback context to a delivery token.
	 * @return The delivery token to be used as the callback context.
	 */
	token_ptr get_token() const { return tok_; }
};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		// __mqtt_disconnect_options_h
