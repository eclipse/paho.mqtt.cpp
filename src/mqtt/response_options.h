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
#include "mqtt/delivery_token.h"

namespace mqtt {

class response_options_test;
class delivery_response_options_test;

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

	/** The client has special access */
	friend class async_client;
	friend class response_options_test;

public:
	/**
	 * Create an empty response object.
	 */
	response_options();
	/**
	 * Creates a response object with the specified callbacks. 
	 * @param tok A token to be used as the context.
	 */
	response_options(token* tok);

	#if 0
	MQTTAsync_responseOptions* c_ptr() { return &opts_; }
	const MQTTAsync_responseOptions* c_ptr() const { return &opts_; }
	#endif

	/**
	 * Sets the callback context to a generic token. 
	 * @param tok The token to be used as the callback context.
	 */
	void set_context(token* tok) {
		opts_.context = tok;
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
	delivery_response_options(delivery_token* dtok);
	/**
	 * Sets the callback context to a delivery token. 
	 * @param dtok The delivery token to be used as the callback context.
	 */
	void set_context(delivery_token* dtok) { 
		opts_.context = dtok;
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		// __mqtt_response_options_h

