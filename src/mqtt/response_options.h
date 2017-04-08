/////////////////////////////////////////////////////////////////////////////
/// @file response_options.h
/// Implementation of the class 'response_options'
/// @date 26-Aug-2016
/////////////////////////////////////////////////////////////////////////////

#ifndef __mqtt_response_options_h
#define __mqtt_response_options_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/token.h"

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
// end namespace 'mqtt'
}

#endif		// __mqtt_response_options_h

