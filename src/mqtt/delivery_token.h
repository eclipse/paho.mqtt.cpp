/////////////////////////////////////////////////////////////////////////////
/// @file delivery_token.h 
/// Declaration of MQTT delivery_token class 
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

#ifndef __mqtt_delivery_token_h
#define __mqtt_delivery_token_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/token.h"
#include "mqtt/message.h"
#include <memory>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////  

/**
 * Provides a mechanism for tracking the delivery of a message.
 */
class idelivery_token : public virtual itoken
{
public:
	typedef std::shared_ptr<idelivery_token> ptr_t;
	/**
	 * Returns the message associated with this token. 
	 * @return The message associated with this token.
	 */
	virtual message_ptr get_message() const =0;          
};

typedef idelivery_token::ptr_t idelivery_token_ptr;

/////////////////////////////////////////////////////////////////////////////  

/**
 * Provides a mechanism to track the delivery progress of a message.
 * Used to track the the delivery progress of a message when a publish is
 * executed in a non-blocking manner (run in the background) action.
 */
class delivery_token : public virtual idelivery_token,
						public token
{
	/** The message being tracked. */
	message_ptr msg_;

	/** Client has special access. */
	friend class async_client;

	/**
	 * Sets the message that this token correspn
	 * @param msg 
	 */
	void set_message(message_ptr msg) { msg_ = msg; }

public:
	/**
	 * Smart/shared pointer to this class.
	 */
	typedef std::shared_ptr<delivery_token> ptr_t;

	delivery_token(iasync_client& cli) : token(cli) {}

	delivery_token(iasync_client& cli, const std::string& topic) : token(cli, topic) {}

	delivery_token(iasync_client& cli, const std::vector<std::string>& topics) 
					: token(cli, topics) {}

	//delivery_token(const std::string& logContext);

	/**
	 * Returns the message associated with this token.
	 * @return message 
	 */
	virtual message_ptr get_message() const { return msg_; }
};

/**
 * Shared pointer to a delivery_token.
 */
typedef delivery_token::ptr_t delivery_token_ptr;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_delivery_token_h

