/////////////////////////////////////////////////////////////////////////////
/// @file disconnect_options.h
/// Implementation of the class 'disconnect_options'
/// @date 26-Aug-2016
/////////////////////////////////////////////////////////////////////////////

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
	MQTTAsync_disconnectOptions opts_;

	/** The client has special access */
	friend class async_client;
	friend class disconnect_options_test;
	friend class token_test;

public:
	/**
	 * Create an empty delivery response object.
	 */
	disconnect_options();
	/**
	 * Creates disconnect options tied to the specific delivery token.
	 * @param timeout The timeout (in milliseconds).
	 * @param tok A token to be used as the context.
	 */
	disconnect_options(int timeout, token* tok);
	/**
	 * Gets the timeout used for disconnecting.
	 * @return The timeout for disconnecting (in milliseconds).
	 */
	std::chrono::milliseconds get_timeout() const {
		return  std::chrono::milliseconds(opts_.timeout);
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
		set_timeout((int) std::chrono::duration_cast<std::chrono::milliseconds>(to).count());
	}
	/**
	 * Sets the callback context to a delivery token.
	 * @param tok The delivery token to be used as the callback context.
	 */
	void set_context(token* tok);
	/**
	 * Gets the callback context to a delivery token.
	 * @return The delivery token to be used as the callback context.
	 */
	token* get_context();
};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		// __mqtt_disconnect_options_h
