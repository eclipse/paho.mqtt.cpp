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
#include "mqtt/subscribe_options.h"
#include "mqtt/message.h"
#include "mqtt/types.h"
#include <vector>

namespace mqtt {

class iasync_client;

/////////////////////////////////////////////////////////////////////////////

/**
 * Represents a topic destination, used for publish/subscribe messaging.
 */
class topic
{
	/** The client to which this topic is connected */
	iasync_client& cli_;
	/** The topic name */
	string name_;
	/** The default QoS */
	int qos_;
	/** The default retined flag */
	bool retained_;

public:
	/** A smart/shared pointer to this class. */
	using ptr_t = std::shared_ptr<topic>;
	/** A smart/shared pointer to this class. */
	using const_ptr_t = std::shared_ptr<const topic>;

	/**
	 * Construct an MQTT topic destination for messages.
	 * @param cli Client to which the topic is attached
	 * @param name The topic string
	 * @param qos The default QoS for publishing.
	 * @param retained The default retained flag for the topic.
	 */
	topic(iasync_client& cli, const string& name,
		  int qos=message::DFLT_QOS, bool retained=message::DFLT_RETAINED)
		: cli_(cli), name_(name), qos_(qos), retained_(retained) {}
	/**
	 * Creates a new topic
	 * @param cli Client to which the topic is attached
	 * @param name The topic string
	 * @param qos The default QoS for publishing.
	 * @param retained The default retained flag for the topic.
	 * @return A shared pointer to the topic.
	 */
	static ptr_t create(iasync_client& cli, const string& name,
						int qos=message::DFLT_QOS,
						bool retained=message::DFLT_RETAINED) {
		return std::make_shared<topic>(cli, name, qos, retained);
	}
	/**
	 * Gets a reference to the MQTT client used by this topic
	 * @return The MQTT client used by this topic
	 */
	iasync_client& get_client() { return cli_; }
	/**
	 * Gets the name of the topic.
	 * @return The name of the topic.
	 */
	const string& get_name() const { return name_; }
	/**
	 * Gets the default quality of service for this topic.
	 * @return The default quality of service for this topic.
	 */
	int get_qos() const { return qos_; }
	/**
	 * Gets the default retained flag used for this topic.
	 * @return The default retained flag used for this topic.
	 */
	bool get_retained() const { return retained_; }
	/**
	 * Sets the default quality of service for this topic.
	 * @param qos The default quality of service for this topic.
	 */
	void set_qos(int qos) { 
		message::validate_qos(qos);
		qos_ = qos; 
	}
	/**
	 * Sets the default retained flag used for this topic.
	 * @param retained The default retained flag used for this topic.
	 */
	void set_retained(bool retained) { retained_ = retained; }
	/**
	 * Publishes a message on the topic using the default QoS and retained
	 * flag.
	 * @param payload the bytes to use as the message payload
	 * @param n the number of bytes in the payload
	 * @return The delivery token used to track and wait for the publish to
	 *  	   complete.
	 */
	delivery_token_ptr publish(const void* payload, size_t n);
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
	delivery_token_ptr publish(const void* payload, size_t n,
							   int qos, bool retained);
	/**
	 * Publishes a message on the topic using the default QoS and retained
	 * flag.
	 * @param payload the bytes to use as the message payload
	 * @return The delivery token used to track and wait for the publish to
	 *  	   complete.
	 */
	delivery_token_ptr publish(binary_ref payload);
	/**
	 * Publishes a message on the topic.
	 * @param payload the bytes to use as the message payload
	 * @param qos the Quality of Service to deliver the message at. Valid
	 *  		  values are 0, 1 or 2.
	 * @param retained whether or not this message should be retained by the
	 *  			   server.
	 * @return The delivery token used to track and wait for the publish to
	 *  	   complete.
	 */
	delivery_token_ptr publish(binary_ref payload, int qos, bool retained);
	/**
	 * Subscribe to the topic.
	 * @return A token used to track the progress of the operation.
	 */
	token_ptr subscribe(const subscribe_options& opts=subscribe_options());
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

