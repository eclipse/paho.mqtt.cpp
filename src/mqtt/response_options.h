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

namespace mqtt {

class response_options_test;
class delivery_response_options_test;
class token_test;

/////////////////////////////////////////////////////////////////////////////
//							response_options
/////////////////////////////////////////////////////////////////////////////

/**
 * The response options for various asynchronous calls.
 */
class response_options
{
	/** The underlying C structure */
	MQTTAsync_responseOptions opts_;

	/** The token to which we are connected */
	token::weak_ptr_t tok_;

	/** The client has special access */
	friend class async_client;
	friend class response_options_test;
	friend class token_test;

public:
	/**
	 * Create an empty response object.
	 */
	response_options();
	/**
	 * Creates a response object with the specified callbacks.
	 * @param tok A token to be used as the context.
	 */
	response_options(const token_ptr& tok);
	/**
	 * Sets the callback context to a generic token.
	 * @param tok The token to be used as the callback context.
	 */
	void set_token(const token_ptr& tok) {
		tok_ = tok;
		opts_.context = tok.get();
	}
};

/////////////////////////////////////////////////////////////////////////////
//							delivery_response_options
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
	delivery_response_options();
	/**
	 * Creates a response object tied to the specific delivery token.
	 * @param dtok A delivery token to be used as the context.
	 */
	delivery_response_options(const delivery_token_ptr& dtok);
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

