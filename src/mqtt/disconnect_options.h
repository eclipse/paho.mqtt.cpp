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
#include "mqtt/types.h"
#include "mqtt/token.h"
#include "mqtt/properties.h"
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

	/** Disconnect message properties */
	properties props_;

	/** The client has special access */
	friend class async_client;

public:
	/**
	 * Create an empty delivery response object.
	 */
	disconnect_options();
	/**
	 * Creates disconnect options tied to the specific token.
	 * @param timeout The timeout (in milliseconds).
	 */
	disconnect_options(int timeout) : disconnect_options() {
		set_timeout(timeout);
	}
	/**
	 * Creates disconnect options tied to the specific token.
	 * @param to The timeout.
	 */
	template <class Rep, class Period>
	disconnect_options(const std::chrono::duration<Rep, Period>& to)
			: disconnect_options() {
		set_timeout(to);
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
	 * Expose the underlying C struct for the unit tests.
	 */
	#if defined(UNIT_TESTS)
		const MQTTAsync_disconnectOptions& c_struct() const { return opts_; }
	#endif
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
	 * @param mqttVersion The version of MQTT we're using for the
	 *  				  connection.
	 */
	void set_token(const token_ptr& tok, int mqttVersion);
	/**
	 * Gets the callback context to a delivery token.
	 * @return The delivery token to be used as the callback context.
	 */
	token_ptr get_token() const { return tok_; }
	/**
	 * Gets the connect properties.
	 * @return A const reference to the properties for the connect.
	 */
	const properties& get_properties() const { return props_; }
	/**
	 * Sets the properties for the connect.
	 * @param props The properties to place into the message.
	 */
	void set_properties(const properties& props) {
		props_ = props;
		opts_.properties = props_.c_struct();
	}
	/**
	 * Moves the properties for the connect.
	 * @param props The properties to move into the connect object.
	 */
	void set_properties(properties&& props) {
		props_ = std::move(props);
		opts_.properties = props_.c_struct();
	}
	/**
	 * Gets the reason code for the disconnect.
	 * @return The reason code for the disconnect.
	 */
	ReasonCode get_reason_code() const {
		return ReasonCode(opts_.reasonCode);
	}
	/**
	 * Sets the reason code for the disconnect.
	 * @param code The reason code for the disconnect.
	 */
	void set_reason_code(ReasonCode code) {
		opts_.reasonCode = MQTTReasonCodes(code);
	}
};


/////////////////////////////////////////////////////////////////////////////

/**
 * Class to build connect options.
 */
class disconnect_options_builder
{
	/** The underlying options */
	disconnect_options opts_;

public:
	/** This class */
	using self = disconnect_options_builder;
	/**
	 * Default constructor.
	 */
	disconnect_options_builder() {}
	/**
	 * Sets the properties for the disconnect message.
	 * @param props The properties for the disconnect message.
	 */
	auto properties(mqtt::properties&& props) -> self& {
		opts_.set_properties(std::move(props));
		return *this;
	}
	/**
	 * Sets the properties for the disconnect message.
	 * @param props The properties for the disconnect message.
	 */
	auto properties(const mqtt::properties& props) -> self& {
		opts_.set_properties(props);
		return *this;
	}
	/**
	 * Sets the connect timeout with a chrono duration.
	 * This is the maximum time that the underlying library will wait for a
	 * connection before failing.
	 * @param to The connect timeout in seconds.
	 */
	template <class Rep, class Period>
	auto timeout(const std::chrono::duration<Rep, Period>& to) -> self&{
		opts_.set_timeout(to);
		return *this;
	}
	/**
	 * Sets the reason code for the disconnect.
	 * @param code The reason code for the disconnect.
	 */
	auto reason_code(ReasonCode code) -> self& {
		opts_.set_reason_code(code);
		return *this;
	}
	/**
	 * Finish building the options and return them.
	 * @return The option struct as built.
	 */
	disconnect_options finalize() { return opts_; }
};


/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		// __mqtt_disconnect_options_h

