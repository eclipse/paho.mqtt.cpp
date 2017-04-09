/////////////////////////////////////////////////////////////////////////////
/// @file delivery_token.h
/// Declaration of MQTT delivery_token class
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

#ifndef __mqtt_delivery_token_h
#define __mqtt_delivery_token_h

#include "MQTTAsync.h"
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
	/** Smart/shared pointer to an object of this class */
	using ptr_t = std::shared_ptr<idelivery_token>;
	/** Smart/shared pointer to a const object of this class */
	using const_ptr_t = std::shared_ptr<const idelivery_token>;
	/** Weak pointer to an object of this class */
	using weak_ptr_t = std::weak_ptr<idelivery_token>;

	/**
	 * Gets the message associated with this token.
	 * @return The message associated with this token.
	 */
	virtual const_message_ptr get_message() const =0;
};

/** Smart/shared pointer to a delivery token */
using idelivery_token_ptr = idelivery_token::ptr_t;

/** Smart/shared pointer to a const delivery token */
using const_idelivery_token_ptr = idelivery_token::const_ptr_t;

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
	const_message_ptr msg_;

	/** Client has special access. */
	friend class async_client;

	/**
	 * Sets the message to which this token corresponds.
	 * @param msg
	 */
	void set_message(const_message_ptr msg) { msg_ = msg; }

public:
	/** Smart/shared pointer to an object of this class */
	using ptr_t = std::shared_ptr<delivery_token>;
	/** Smart/shared pointer to a const object of this class */
	using const_ptr_t = std::shared_ptr<delivery_token>;
	/** Weak pointer to an object of this class */
	using weak_ptr_t = std::weak_ptr<delivery_token>;

	/**
	 * Creates an empty delivery token connected to a particular client.
	 * @param cli The asynchronous client object.
	 */
	delivery_token(iasync_client& cli) : token(cli) {}
	/**
	 * Creates a delivery token connected to a particular client.
	 * @param cli The asynchronous client object.
	 * @param topic The topic that the message is associated with.
	 */
	delivery_token(iasync_client& cli, const std::string& topic) : token(cli, topic) {}
	/**
	 * Creates a delivery token connected to a particular client.
	 * @param cli The asynchronous client object.
	 * @param topic The topic that the message is associated with.
	 * @param msg The message data.
	 */
	delivery_token(iasync_client& cli, const std::string& topic, const_message_ptr msg)
			: token(cli, topic), msg_(msg) {}
	/**
	 * Creates a delivery token connected to a particular client.
	 * @param cli The asynchronous client object.
	 * @param topics The topics that the message is associated with.
	 */
	delivery_token(iasync_client& cli, const std::vector<std::string>& topics)
					: token(cli, topics) {}
	/**
	 * Gets the message associated with this token.
	 * @return The message associated with this token.
	 */
	const_message_ptr get_message() const override { return msg_; }
};

/** Smart/shared pointer to a delivery_token */
using delivery_token_ptr = delivery_token::ptr_t;

/** Smart/shared pointer to a const delivery_token */
using const_delivery_token_ptr = delivery_token::const_ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_delivery_token_h

