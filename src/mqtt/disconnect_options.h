/////////////////////////////////////////////////////////////////////////////
/// @file disconnect_options.h
/// Implementation of the class 'disconnect_options'
/// @date 26-Aug-2016
/////////////////////////////////////////////////////////////////////////////

#ifndef __mqtt_disconnect_options_h
#define __mqtt_disconnect_options_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/token.h"

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

public:
	/**
	 * Create an empty delivery response object.
	 */
	disconnect_options();
	/**
	 * Creates disconnect options tied to the specific delivery token.
	 * @param tok A token to be used as the context.
	 */
	disconnect_options(int timeout, token* tok);
	/**
	 * Sets the timeout for disconnecting. 
	 * This allows for any remaining in-flight messages to be delivered. 
	 * @param timeout The timeout (in milliseconds).
	 */
	void set_timeout(int timeout) { opts_.timeout = timeout; }
	/**
	 * Gets the timeout used for disconnecting. 
	 * @return The timeout for disconnecting (in milliseconds).
	 */
	int get_timeout() const { return opts_.timeout; }
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

