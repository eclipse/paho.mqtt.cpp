/////////////////////////////////////////////////////////////////////////////
/// @file callback.h
/// Declaration of MQTT callback class
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

#ifndef __mqtt_callback_h
#define __mqtt_callback_h

#include "MQTTAsync.h"
#include "mqtt/delivery_token.h"
#include "mqtt/types.h"
#include <vector>
#include <memory>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Provides a mechanism for tracking the completion of an asynchronous
 * action.
 */
class callback
{
public:
	/** Smart/shared pointer to an object of this type */
	using ptr_t = std::shared_ptr<callback>;
	/** Smart/shared pointer to a const object of this type */
	using const_ptr_t = std::shared_ptr<const callback>;

	/**
	 * Virtual destructor.
	 */
	virtual ~callback() {}

	/**
	 * This method is called when the client is connected.
	 * @param cause
	 */
	virtual void connected(const string& cause) {}
	/**
	 * This method is called when the connection to the server is lost.
	 * @param cause
	 */
	virtual void connection_lost(const string& cause) {}
	/**
	 * This method is called when a message arrives from the server.
	 * @param msg The message
	 */
	virtual void message_arrived(const_message_ptr msg) {}
	/**
	 * Called when delivery for a message has been completed, and all
	 * acknowledgments have been received.
	 * @param tok The token tracking the message delivery.
	 */
	virtual void delivery_complete(delivery_token_ptr tok) {}
};

/** Smart/shared pointer to a callback object */
using callback_ptr = callback::ptr_t;

/** Smart/shared pointer to a const callback object */
using const_callback_ptr = callback::const_ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_callback_h

