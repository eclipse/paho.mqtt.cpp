/////////////////////////////////////////////////////////////////////////////
/// @file message.h 
/// Declaration of MQTT message class 
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

#ifndef __mqtt_message_h
#define __mqtt_message_h

extern "C" {
	#include "MQTTAsync.h"
}

#include <string>
#include <memory>
#include <stdexcept> 

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////  

/**
 * An MQTT message holds the application payload and options specifying how 
 * the message is to be delivered The message includes a "payload" (the body 
 * of the message) represented as a byte array. 
 */
class message
{
	/** The underlying C message struct */
	MQTTAsync_message msg_;

	/** The client has special access. */
	friend class async_client;

	/**
	 * Set the dup flag in the underlying message 
	 * @param dup 
	 */
	void set_duplicate(bool dup) { msg_.dup = (dup) ? (!0) : 0; }
	/**
	 * Copies the specified payload into this object.
	 * @param payload 
	 * @param len 
	 */
	void copy_payload(const void* payload, size_t len);

public:
	/**
	 * Smart/shared pointer to this class.
	 */
	typedef std::shared_ptr<message> ptr_t;
	/**
	 * Constructs a message with an empty payload, and all other values set
	 * to defaults.
	 */
	message();
	/**
	 * Constructs a message with the specified array as a payload, and all 
	 * other values set to defaults. 
	 * @param payload the bytes to use as the message payload 
	 * @param n the number of bytes in the payload
	 */
	message(const void* payload, size_t len);
	/**
	 * Constructs a message with the specified string as a payload, and 
	 * all other values set to defaults.
	 * @param payload A string to use as the message payload.
	 */
	message(const std::string& payload);
	/**
	 * Constructs a message as a copy of the message structure. 
	 * @param msg A "C" MQTTAsync_message structure.
	 */
	message(const MQTTAsync_message& msg);
	/**
	 * Constructs a message as a copy of the other message. 
	 * @param other The message to copy into this one. 
	 */
	message(const message& other);
	/**
	 * Moves the other message to this one.
	 * @param other The message to move into this one. 
	 */
	message(message&& other);
	/**
	 * Destroys a message and frees all associated resources.
	 */
	~message();
	/**
	 * Copies another message to this one. 
	 * @param rhs The other message. 
	 * @return A reference to this message.
	 */
	message& operator=(const message& rhs);
	/**
	 * Moves another message to this one. 
	 * @param rhs The other message. 
	 * @return A reference to this message.
	 */
	message& operator=(message&& rhs);
	/**
	 * Clears the payload, resetting it to be empty.
	 */
	void clear_payload();
	/**
	 * Gets the payload
	 */
	std::string get_payload() const;
	/**
	 * Returns the quality of service for this message.
	 * @return The quality of service for this message.
	 */
	int get_qos() const { return msg_.qos; }
	/**
	 * Returns whether or not this message might be a duplicate of one which 
	 * has already been received. 
	 * @return true this message might be a duplicate of one which 
	 * has already been received, false otherwise
	 */
	bool is_duplicate() const { return msg_.dup != 0; }
	/**
	 * Returns whether or not this message should be/was retained by the 
	 * server.
	 * @return true if this message should be/was retained by the 
	 * server, false otherwise.
	 */
	bool is_retained() const { return msg_.retained != 0; }
	/**
	 * Sets the payload of this message to be the specified byte array. 
	 * @param payload the bytes to use as the message payload 
	 * @param n the number of bytes in the payload
	 */
	void set_payload(const void* payload, size_t n);
	/**
	 * Sets the payload of this message to be the specified string. 
	 * @param payload A string to use as the message payload.
	 */
	void set_payload(const std::string& payload);
	/**
	 * Sets the quality of service for this message.
	 * 
	 * @param qos 
	 */
	void set_qos(int qos) throw(std::invalid_argument) {
		validate_qos(qos); 
		msg_.qos = qos;
	}
	/**
	 * Whether or not the publish message should be retained by the 
	 * messaging engine. 
	 * @param retained 
	 */
	void set_retained(bool retained) { msg_.retained = (retained) ? (!0) : 0; }
	/**
	 * Determines if the QOS value is a valid one.
	 * @param qos The QOS value.
	 * @throw std::invalid_argument If the qos value is invalid.
	 */
	static void validate_qos(int qos) throw(std::invalid_argument) {
		if (qos < 0 || qos > 2)
			throw std::invalid_argument("QOS invalid");
	}
};

/**
 * Shared pointer to a message
 */
typedef message::ptr_t message_ptr;

/**
 * Constructs a message with the specified array as a payload, and all 
 * other values set to defaults. 
 * @param payload the bytes to use as the message payload 
 * @param n the number of bytes in the payload
 */
inline message_ptr make_message(const void* payload, size_t len) {
	return std::make_shared<mqtt::message>(payload, len);
}

/**
 * Constructs a message with the specified string as a payload, and 
 * all other values set to defaults.
 * @param payload A string to use as the message payload.
 */
inline message_ptr make_message(const std::string& payload) {
	return std::make_shared<mqtt::message>(payload);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

/**
 * Output stream operator for mqtt::message
 */

std::ostream& operator<<(std::ostream& os, const mqtt::message& msg);

std::ostream& operator<<(std::ostream& os, const mqtt::message_ptr msg);

#endif		// __mqtt_message_h

