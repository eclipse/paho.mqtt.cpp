/////////////////////////////////////////////////////////////////////////////
/// @file response_options.h
/// Implementation of the class 'response_options'
/// @date 26-Aug-2016
/////////////////////////////////////////////////////////////////////////////

#ifndef __mqtt_response_options_h
#define __mqtt_response_options_h

#include "MQTTAsync.h"
#include "mqtt/token.h"
#include "mqtt/delivery_token.h"
#include "subscribe_options.h"

namespace mqtt {

class response_options_test;
class delivery_response_options_test;
class token_test;

/////////////////////////////////////////////////////////////////////////////
//							response_options
/////////////////////////////////////////////////////////////////////////////

/**
 * The response options for various asynchronous calls.
 *
 * This is an internal data structure, only used within the library.
   Therefor it is not totally fleshed out, but rather only exposes the
   functionality currently required by the library.
 */
class response_options
{
	/** The underlying C structure */
	MQTTAsync_responseOptions opts_;

	/** The token to which we are connected */
	token::weak_ptr_t tok_;

	/** A list of subscription options for subscribe-many */
	std::vector<MQTTSubscribe_options> subOpts_;

	/** The client has special access */
	friend class async_client;
	friend class response_options_test;
	friend class token_test;

public:
	/**
	 * Create an empty response object.
	 */
	explicit response_options(int mqttVersion=MQTTVERSION_DEFAULT);
	/**
	 * Creates a response object with the specified callbacks.
	 * @param tok A token to be used as the context.
	 */
	response_options(const token_ptr& tok, int mqttVersion=MQTTVERSION_DEFAULT);
	/**
	 * Sets the callback context to a generic token.
	 * @param tok The token to be used as the callback context.
	 */
	void set_token(const token_ptr& tok);
	/**
	 * Sets the options for a single topic subscription.
	 * @param opts The subscribe options.
	 */
	void set_subscribe_options(const subscribe_options& opts);
	/**
	 * Sets the options for a multi-topic subscription.
	 * @param opts A vector of the subscribe options.
	 */
	void set_subscribe_options(const std::vector<subscribe_options>& opts);
};

/////////////////////////////////////////////////////////////////////////////
//						delivery_response_options
/////////////////////////////////////////////////////////////////////////////

/**
 * The response options for asynchronous calls targeted at delivery.
 * Each of these objects is tied to a specific delivery_token.
 */
class delivery_response_options
{
	/** The underlying C structure */
	MQTTAsync_responseOptions opts_;

	/** The delivery token to which we are connected */
	delivery_token::weak_ptr_t dtok_;

	/** The client has special access */
	friend class async_client;
	friend class delivery_response_options_test;

public:
	/**
	 * Create an empty delivery response object.
	 */
	delivery_response_options(int mqttVersion=MQTTVERSION_DEFAULT);
	/**
	 * Creates a response object tied to the specific delivery token.
	 * @param dtok A delivery token to be used as the context.
	 */
	delivery_response_options(const delivery_token_ptr& dtok,
							  int mqttVersion=MQTTVERSION_DEFAULT);
	/**
	 * Sets the callback context to a delivery token.
	 * @param dtok The delivery token to be used as the callback context.
	 */
	void set_token(const delivery_token_ptr& dtok) {
		dtok_ = dtok;
		opts_.context = dtok.get();
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		// __mqtt_response_options_h

