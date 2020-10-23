/////////////////////////////////////////////////////////////////////////////
/// @file create_options.h
/// Declaration of MQTT create_options class
/// @date Oct 17, 2020
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2020 Frank Pagliughi <fpagliughi@mindspring.com>
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

#ifndef __mqtt_create_options_h
#define __mqtt_create_options_h

#include "MQTTAsync.h"
#include "mqtt/types.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Options for creating a client object.
 */
class create_options
{
	/** The default C struct */
	static const MQTTAsync_createOptions DFLT_C_STRUCT;

	/** The underlying C options */
	MQTTAsync_createOptions opts_;

	/** The client and tests have special access */
	friend class async_client;
	friend class create_options_builder;

public:
	/** Smart/shared pointer to an object of this class. */
	using ptr_t = std::shared_ptr<create_options>;
	/** Smart/shared pointer to a const object of this class. */
	using const_ptr_t = std::shared_ptr<const create_options>;

	/**
	 * Default set of client create options.
	 */
	create_options() : opts_(DFLT_C_STRUCT) {}
	/**
	 * Gets whether the client will accept message to publish while
	 * disconnected.
	 */
	bool get_send_while_disconnected() const {
		return to_bool(opts_.sendWhileDisconnected);
	}
	/**
	 * Sets whether the client will accept message to publish while
	 * disconnected.
	 *
	 * @param on @em true to allow the application to publish messages while
	 *  		 disconnected, @false returns an error on publish if
	 *  		 disconnected.
	 * @param anyTime If @em true, allows you to publish messages before the
	 *  			  first successful connection.
	 */
	void set_send_while_disconnected(bool on, bool anyTime=false) {
		opts_.sendWhileDisconnected = to_int(on);
		opts_.allowDisconnectedSendAtAnyTime = to_int(anyTime);
	}
	/**
	 * Gets the maximum number of offline buffered messages.
	 * @return The maximum number of offline buffered messages.
	 */
	int get_max_buffered_messages() const {
		return opts_.maxBufferedMessages;
	}
	/**
	 * Sets the maximum number of offline buffered messages.
	 * @param n The maximum number of offline buffered messages.
	 */
	void set_max_buffered_messages(int n) {
		opts_.maxBufferedMessages = n;
	}
	/**
	 * Gets the MQTT version used to create the client.
	 * @return The MQTT version used to create the client.
	 */
	int mqtt_version() const { return opts_.MQTTVersion; }
	/**
	 * Sets the MQTT version used to create the client.
	 * @param ver The MQTT version used to create the client.
	 */
	void set_mqtt_verison(int ver) { opts_.MQTTVersion = ver; }
	/**
	 * Whether the oldest messages are deleted when the output buffer is
	 * full.
	 *
	 * @return @em true if the oldest messages should be deleted when the
	 *  	   output buffer is full, @em false if the new messages should
	 *  	   be dropped when the buffer is full.
	 */
	bool get_delete_oldest_messages() const {
		return to_bool(opts_.deleteOldestMessages);
	}
	/**
	 * Determines what to do when the maximum number of buffered messages is
	 * reached: delete the oldest messages rather than the newest
	 * @param on @em true When the output queue is full, delete the oldest
	 *  		 message, @em false drop the newest message being added.
	 */
	void set_delete_oldest_messages(bool on) {
		opts_.deleteOldestMessages = to_int(on);
	}
	/**
	 * Whether the messages will be restored from persistence or the store
	 * will be cleared.
	 *
	 * @return @em true if the messages will be restored from persistence,
	 *  	   @em false if the persistence store will be cleared.
	 */
	bool get_restore_messages() const {
		return to_bool(opts_.restoreMessages);
	}
	/**
	 * Determine whether to restore messages from persistence or clear the
	 * persistence store.
	 *
	 * @param on @true to restore messages from persistence, @false to clear
	 *  		 the persistence store.
	 */
	void set_restore_messages(bool on) {
		opts_.restoreMessages = to_int(on);
	}
	/**
	 * Whether to persist QoS 0 messages.
	 *
	 * @return @em true if QoS 0 messages are persisted, @em false if not.
	 */
	bool get_persist_qos0() const {
		return to_bool(opts_.persistQoS0);
	}
	/**
	 * Determeine whether to persist QoS 0 messages.
	 *
	 * @param on @em true if QoS 0 messages are persisted, @em false if not.
	 */
	void set_persist_qos0(bool on) {
		opts_.persistQoS0 = to_int(on);
	}
};

/** Smart/shared pointer to a connection options object. */
using create_options_ptr = create_options::ptr_t;

/////////////////////////////////////////////////////////////////////////////

/**
 * Builder class to generate the create options.
 */
class create_options_builder
{
	/** The underlying options */
	create_options opts_;

public:
	/** This class */
	using self = create_options_builder;
	/**
	 * Default constructor.
	 */
	create_options_builder() {}
	/**
	 *
	 * Sets whether the client will accept message to publish while
	 * disconnected.
	 *
	 * @param on @em true to allow the application to publish messages while
	 *  		 disconnected, @false returns an error on publish if
	 *  		 disconnected.
	 * @param anyTime If @em true, allows you to publish messages before the
	 *  			  first successful connection.
	 * @return A reference to this object.
	 */
	auto send_while_disconnected(bool on=true, bool anyTime=false) -> self& {
		opts_.opts_.sendWhileDisconnected = to_int(on);
		opts_.opts_.allowDisconnectedSendAtAnyTime = to_int(anyTime);
		return *this;
	}
	/**
	 * Sets the maximum number of offline buffered messages.
	 * @param n The maximum number of offline buffered messages.
	 * @return A reference to this object.
	 */
	auto max_buffered_messages(int n) -> self& {
		opts_.opts_.maxBufferedMessages = n;
		return *this;
	}
	/**
	 * Sets the MQTT version used to create the client.
	 * @param ver The MQTT version used to create the client.
	 */
	auto mqtt_version(int ver) -> self& {
		opts_.opts_.MQTTVersion = ver;
		return *this;
	}
	/**
	 * Determines what to do when the maximum number of buffered messages is
	 * reached: delete the oldest messages rather than the newest.
	 * @param on @em true When the output queue is full, delete the oldest
	 *  		 message, @em false drop the newest message being added.
	 * @return A reference to this object.
	 */
	auto delete_oldest_messages(bool on=true) -> self& {
		opts_.opts_.deleteOldestMessages = to_int(on);
		return *this;
	}
	/**
	 * Determines whether to restore persisted messsages or clear the
	 * persistence store. (Defaults true)
	 *
	 * @param on @em true to retore persisted messages, @em false to clear
	 *  		 the persistence store.
	 * @return A reference to this object.
	 */
	auto restore_messages(bool on=true) -> self& {
		opts_.opts_.restoreMessages = to_int(on);
		return *this;
	}
	/**
	 * Whether to persist QoS 0 messages. (Defaults true)
	 *
	 * @param on @em true persist QoS 0 messages, @em false, don't.
	 * @return A reference to this object
	 */
	auto persist_qos0(bool on=true) -> self& {
		opts_.opts_.persistQoS0 = to_int(on);
		return *this;
	}
	/**
	 * Finish building the options and return them.
	 * @return The option struct as built.
	 */
	create_options finalize() { return opts_; }
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_create_options_h

