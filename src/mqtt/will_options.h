/////////////////////////////////////////////////////////////////////////////
/// @file will_options.h
/// Declaration of MQTT will_options class
/// @date Jul 7, 2016
/// @author Guilherme M. Ferreira
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2016 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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
 *    Guilherme M. Ferreira - initial implementation and documentation
 *    Frank Pagliughi - added copy & move operations
 *******************************************************************************/

#ifndef __mqtt_will_options_h
#define __mqtt_will_options_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/message.h"
#include "mqtt/topic.h"
#include <string>
#include <memory>

namespace mqtt {

class connect_options;

/////////////////////////////////////////////////////////////////////////////

/**
 * Holds the set of options that govern the Last Will and Testament feature.
 */
class will_options
{
	/** The underlying C LWT options */
	MQTTAsync_willOptions opts_;

	/** LWT message topic **/
	std::string topic_;

	/** LWT message text */
	std::string payload_;

	/** A default C struct to support re-initializing variables */
	static const MQTTAsync_willOptions DFLT_C_WILL;

	/** The connect options has special access */
	friend class connect_options;

public:
	/** Smart/shared pointer to this class. */
	using ptr_t = std::shared_ptr<will_options>;
	/** Smart/shared pointer to a const object of this class. */
	using const_ptr_t = std::shared_ptr<const will_options>;

	/**
	 * Constructs a new object using the default values.
	 */
	will_options();
	/**
	 * Sets the "Last Will and Testament" (LWT) for the connection.
	 * @param top The LWT message is published to the this topic.
	 * @param payload The message that is published to the Will Topic.
	 * @param payload_len The message size in bytes
	 * @param qos The message Quality of Service.
	 * @param retained Tell the broker to keep the LWT message after send to subscribers.
	 */
	will_options(
		const std::string& top,
		const void *payload,
		size_t payload_len,
		QoS qos,
		bool retained
	);
	/**
	 * Sets the "Last Will and Testament" (LWT) for the connection.
	 * @param top The LWT message is published to the this topic.
	 * @param payload The message that is published to the Will Topic.
	 * @param payload_len The message size in bytes.
	 * @param qos The message Quality of Service.
	 * @param retained Tell the broker to keep the LWT message after send to subscribers.
	 */
	will_options(
		const topic& top,
		const void *payload,
		size_t payload_len,
		QoS qos,
		bool retained
	);
	/**
	 * Sets the "Last Will and Testament" (LWT) for the connection.
	 * @param top The LWT message is published to the this topic.
	 * @param payload The message payload that is published to the Will
	 *  			  Topic.
	 * @param qos The message Quality of Service.
	 * @param retained Tell the broker to keep the LWT message after send to subscribers.
	 */
	will_options(
		const std::string& top,
		const std::string& payload,
		QoS qos,
		bool retained
	);
	/**
	 * Sets the "Last Will and Testament" (LWT) for the connection.
	 * @param top The LWT message is published to the this topic.
	 * @param msg The message that is published to the Will Topic.
	 */
	will_options(
		const std::string& top,
		const message& msg
	);
	/**
	 * Copy constructor for the LWT options.
	 * @param opt The other options.
	 */
	will_options(const will_options& opt);
	/**
	 * Move constructor for the LWT options.
	 * @param opt The other options.
	 */
	will_options(will_options&& opt);
	/**
	 * Copy assignment for the LWT options.
	 * @param opt The other options.
	 */
	will_options& operator=(const will_options& opt);
	/**
	 * Move assignment for the LWT options.
	 * @param opt The other options.
	 */
	will_options& operator=(will_options&& opt);
	/**
	 * Returns the LWT message topic name.
	 * @return std::string
	 */
	std::string get_topic() const { return topic_; }
	/**
	 * Returns the LWT message payload.
	 * @return The LWT message payload.
	 */
	std::string get_payload() const { return payload_; }
	/**
	 * Gets the QoS value for the LWT message.
	 * @return The QoS value for the LWT message.
	 */
	QoS get_qos() const {
		return static_cast<QoS>(opts_.qos);
	}
	/**
	 * Gets the 'retained' flag for the LWT message.
	 * @return The 'retained' flag for the LWT message.
	 */
	int is_retained() const { return opts_.retained; }
	/**
	 * Gets the LWT message.
	 * Note that this is a const pointer to a copy of the message, not to
	 * the message itself.
	 * @return A copy of the LWT message.
	 */
	const_message_ptr get_message() const {
		return make_message(payload_, static_cast<QoS>(opts_.qos), opts_.retained);
	}
	/**
	 * Sets the LWT message topic name.
	 * @param top The topic where to sent the message
	 */
	void set_topic(const std::string& top);
	/**
	 * Sets the LWT message text.
	 * @param msg The LWT message
	 */
	void set_payload(const std::string& msg);
	/**
	 * Sets the QoS value.
	 * @param qos The LWT message QoS
	 */
	void set_qos(const QoS qos);
	/**
	 * Sets the retained flag.
	 * @param retained Tell the broker to keep the LWT message after send to
	 *  			   subscribers.
	 */
	void set_retained(const int retained) { opts_.retained = retained; }
};

/** Shared pointer to a will options object. */
using will_options_ptr = will_options::ptr_t;

/** Shared pointer to a const will options object. */
using const_will_options_ptr = will_options::const_ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_will_options_h

