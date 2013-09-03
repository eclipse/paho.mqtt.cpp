/////////////////////////////////////////////////////////////////////////////
/// @file callback.h
/// Declaration of MQTT callback class
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

#ifndef __mqtt_callback_h
#define __mqtt_callback_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/delivery_token.h"
#include <string>
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
	typedef std::shared_ptr<callback> ptr_t;
	/**
	 * This method is called when the connection to the server is lost.
	 * @param cause 
	 */
	virtual void connection_lost(const std::string& cause) =0;
	/**
	 * This method is called when a message arrives from the server. 
	 * @param topic
	 * @param msg 
	 */
	virtual void message_arrived(const std::string& topic, message_ptr msg) =0;
	/**
	 * Called when delivery for a message has been completed, and all 
	 * acknowledgments have been received.
	 * @param token 
	 */
	virtual void delivery_complete(idelivery_token_ptr tok) =0;
};

typedef callback::ptr_t callback_ptr;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_callback_h

