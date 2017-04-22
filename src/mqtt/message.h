/////////////////////////////////////////////////////////////////////////////
/// @file message.h
/// Declaration of MQTT message class
/// @date May 1, 2013
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2013-2017 Frank Pagliughi <fpagliughi@mindspring.com>
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

#include "MQTTAsync.h"
#include "mqtt/buffer_ref.h"
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
	static constexpr int DFLT_QOS = 0;
	static constexpr bool DFLT_RETAINED = false;
	static constexpr MQTTAsync_message DFLT_C_STRUCT MQTTAsync_message_initializer;

	/** The underlying C message struct */
	MQTTAsync_message msg_;
	/**
	 * The message payload.
	 * Note that this is not necessarily a printable text string, but rather
	 * an arbitrary binary blob held in a string container.
	 */
	binary_ref payload_;

	/** The client has special access. */
	friend class async_client;
	friend class message_test;

	/**
	 * Set the dup flag in the underlying message
	 * @param dup
	 */
	void set_duplicate(bool dup) { msg_.dup = (dup) ? (!0) : 0; }

public:
	/** Smart/shared pointer to this class. */
	using ptr_t = std::shared_ptr<message>;
	/** Smart/shared pointer to this class. */
	using const_ptr_t = std::shared_ptr<const message>;

	/**
	 * Constructs a message with an empty payload, and all other values set
	 * to defaults.
	 */
	message();
	/**
	 * Constructs a message with the specified array as a payload, and all
	 * other values set to defaults.
	 * @param payload the bytes to use as the message payload
	 * @param len the number of bytes in the payload
	 * @param qos The quality of service for the message.
	 * @param retained Whether the message should be retained by the broker.
	 */
	message(const void* payload, size_t len, int qos, bool retained);
	/**
	 * Constructs a message with the specified array as a payload, and all
	 * other values set to defaults.
	 * @param payload the bytes to use as the message payload
	 * @param len the number of bytes in the payload
	 */
	message(const void* payload, size_t len)
		: message(payload, len, DFLT_QOS, DFLT_RETAINED) {}
	/**
	 * Constructs a message from a byte buffer.
	 * Note that the payload accepts copy or move semantics.
	 * @param payload A byte buffer to use as the message payload.
	 * @param qos The quality of service for the message.
	 * @param retained Whether the message should be retained by the broker.
	 */
	message(binary_ref payload, int qos, bool retained);
	/**
	 * Constructs a message from a byte buffer.
	 * Note that the payload accepts copy or move semantics.
	 * @param payload A byte buffer to use as the message payload.
	 */
	message(binary_ref payload) : message(std::move(payload), DFLT_QOS, DFLT_RETAINED) {}
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
	~message() {}
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
	const binary_ref& get_payload() const { return payload_; }
	/**
	 * Gets the payload as a string
	 */
	//const string& get_payload_str() const { return payload_.to_string(); }
	string get_payload_str() const { return payload_ ?  payload_.to_string() : string(); }
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
	 * Sets the payload of this message to be the specified buffer.
	 * Note that this accepts copy or move operations:
	 *   set_payload(buf);
	 *   set_payload(std::move(buf));
	 * @param payload A buffer to use as the message payload.
	 */
	void set_payload(binary_ref payload);
	/**
	 * Sets the payload of this message to be the specified byte array.
	 * @param payload the bytes to use as the message payload
	 * @param n the number of bytes in the payload
	 */
	void set_payload(const void* payload, size_t n) {
		set_payload(binary_ref(static_cast<const binary_ref::value_type*>(payload), n));
	}
	/**
	 * Sets the payload of this message to be the specified string.
	 * @param payload A string to use as the message payload.
	 */
	void set_payload(string payload) {
		set_payload(binary_ref(std::move(payload)));
	}
	/**
	 * Sets the quality of service for this message.
	 * @param qos The integer Quality of Service for the message
	 */
	void set_qos(int qos) {
		validate_qos(qos);
		msg_.qos = qos;
	}
	/**
	 * Whether or not the publish message should be retained by the broker.
	 * @param retained @em true if the message should be retained by the
	 *  			   broker, @em false if not.
	 */
	void set_retained(bool retained) { msg_.retained = (retained) ? (!0) : 0; }
	/**
	 * Returns a string representation of this messages payload.
	 * @return string
	 */
	string to_str() const { return get_payload_str(); }
	/**
	 * Determines if the QOS value is a valid one.
	 * @param qos The QOS value.
	 * @throw std::invalid_argument If the qos value is invalid.
	 */
	static void validate_qos(int qos) {
		if (qos < 0 || qos > 2)
			throw std::invalid_argument("QOS invalid");
	}
};

/** Smart/shared pointer to a message */
using message_ptr = message::ptr_t;

/** Smart/shared pointer to a const message */
using const_message_ptr = message::const_ptr_t;

/**
 * Constructs a message with the specified array as a payload, and all
 * other values set to defaults.
 * @param payload the bytes to use as the message payload
 * @param len the number of bytes in the payload
 */
inline message_ptr make_message(const void* payload, size_t len) {
	return std::make_shared<mqtt::message>(payload, len);
}

/**
 * Constructs a message with the specified array as a payload, and all
 * other values set to defaults.
 * @param payload the bytes to use as the message payload
 * @param len the number of bytes in the payload
 * @param qos The quality of service for the message.
 * @param retained Whether the message should be retained by the broker.
 */
inline message_ptr make_message(const void* payload, size_t len,
								int qos, bool retained) {
	return std::make_shared<mqtt::message>(payload, len, qos, retained);
}

/**
 * Constructs a message with the specified buffer as a payload, and
 * all other values set to defaults.
 * @param payload A string to use as the message payload.
 */
inline message_ptr make_message(binary_ref payload) {
	return std::make_shared<mqtt::message>(std::move(payload));
}

/**
 * Constructs a message with the specified values.
 * @param payload A buffer to use as the message payload.
 * @param qos The quality of service for the message.
 * @param retained Whether the message should be retained by the broker.
 */
inline message_ptr make_message(binary_ref payload, int qos, bool retained) {
	return std::make_shared<mqtt::message>(std::move(payload), qos, retained);
}

#if 0
/**
 * Constructs a message with the specified string as a payload, and
 * all other values set to defaults.
 * @param payload A string to use as the message payload.
 */
inline message_ptr make_message(string payload) {
	return std::make_shared<mqtt::message>(std::move(payload));
}
inline message_ptr make_message(const char* payload) {
	return std::make_shared<mqtt::message>(string(payload));
}

/**
 * Constructs a message with the specified string as a payload, and
 * all other values set to defaults.
 * @param payload A string to use as the message payload.
 * @param qos The quality of service for the message.
 * @param retained Whether the message should be retained by the broker.
 */
inline message_ptr make_message(const string& payload, int qos, bool retained) {
	return std::make_shared<mqtt::message>(payload, qos, retained);
}
inline message_ptr make_message(const char* payload, int qos, bool retained) {
	return std::make_shared<mqtt::message>(string(payload), qos, retained);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_message_h

