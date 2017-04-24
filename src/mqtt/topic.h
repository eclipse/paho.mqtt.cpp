/////////////////////////////////////////////////////////////////////////////
/// @file topic.h
/// Declaration of MQTT topic class
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

#ifndef __mqtt_topic_h
#define __mqtt_topic_h

#include "MQTTAsync.h"
#include "mqtt/delivery_token.h"
#include "mqtt/message.h"
#include "mqtt/types.h"
#include <vector>

namespace mqtt {

class async_client;

/////////////////////////////////////////////////////////////////////////////

/**
 * Represents a topic destination, used for publish/subscribe messaging.
 */
class topic
{
	/** The topic name */
	string name_;

	/** The client to which this topic is connected */
	iasync_client& cli_;

public:
	/** A smart/shared pointer to this class. */
	using ptr_t = std::shared_ptr<topic>;
	/** A smart/shared pointer to this class. */
	using const_ptr_t = std::shared_ptr<const topic>;

	/**
	 * Construct an MQTT topic destination for messages.
	 * @param name The topic string
	 * @param cli Client to which the topic should be attached
	 */
	topic(const string& name, iasync_client& cli) : name_{name}, cli_{cli} {}
	/**
	 * Returns the name of the topic.
	 * @return The name of the topic.
	 */
	const string& get_name() const { return name_; }
	/**
	 * Publishes a message on the topic.
	 * @param payload the bytes to use as the message payload
	 * @param n the number of bytes in the payload
	 * @param qos the Quality of Service to deliver the message at. Valid
	 *  		  values are 0, 1 or 2.
	 * @param retained whether or not this message should be retained by the
	 *  			   server.
	 * @return The delivery token used to track and wait for the publish to
	 *  	   complete.
	 */
	delivery_token_ptr publish(const void* payload, size_t n, int qos, bool retained);
	/**
	 * Publishes a message on the topic.
	 * @param payload the bytes to use as the message payload
	 * @param n the number of bytes in the payload
	 * @param qos the Quality of Service to deliver the message at. Valid
	 *  		  values are 0, 1 or 2.
	 * @param retained whether or not this message should be retained by the
	 *  			   server.
	 * @return The delivery token used to track and wait for the publish to
	 *  	   complete.
	 */
	delivery_token_ptr publish(binary_ref payload, int qos, bool retained);
	/**
	 * Publishes the specified message to this topic, but does not wait for
	 * delivery of the message to complete.
	 * @param msg
	 * @return delivery_token
	 */
	delivery_token_ptr publish(const_message_ptr msg);
	/**
	 * Returns a string representation of this topic.
	 * @return The name of the topic
	 */
	string to_string() const { return name_; }
};

/** A smart/shared pointer to a topic object. */
using topic_ptr = topic::ptr_t ;

/** A smart/shared pointer to a const topic object. */
using const_topic_ptr = topic::const_ptr_t ;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_topic_h

