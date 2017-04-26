/////////////////////////////////////////////////////////////////////////////
/// @file client.h
/// Declaration of MQTT client class
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

#ifndef __mqtt_client_h
#define __mqtt_client_h

#include "mqtt/async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Lightweight client for talking to an MQTT server using methods that block
 * until an operation completes.
 */
class client
{
	/** The default quality of service */
	static const int DFLT_QOS;
	/** The actual client */
	async_client cli_;
	/**
	 * The longest amount of time to wait for an operation to complete.
	 */
	std::chrono::milliseconds timeout_;

	/**
	 * Creates a shared pointer to a non-heap object. This creates a shared
	 * pointer to an existing object. The pointer is given a no-op deleter,
	 * so it will not try to destroy the object when it goes out of scope.
	 * It is up to the caller to ensure that the object remains in scope for
	 * as long as there may be pointers to it.
	 * @param val A value which may live anywherte in memory.
	 * @return A shared pointer to a non-heap object.
	 */
	template <typename T>
	std::shared_ptr<T> ptr(const T& val) {
		return std::shared_ptr<T>(const_cast<T*>(&val), [](T*){});
	}

	/** Non-copyable */
	client() =delete;
	client(const async_client&) =delete;
	client& operator=(const async_client&) =delete;

public:
	/** Smart pointer type for this object */
	using ptr_t = std::shared_ptr<client>;
	/** Type for a collection of QOS values */
	using qos_collection = async_client::qos_collection;

	/**
	 * Create a client that can be used to communicate with an MQTT server.
	 * This uses file-based persistence in the current working directory.
	 * @param serverURI
	 * @param clientId
	 */
	client(const string& serverURI, const string& clientId);
	/**
	 * Create a client that can be used to communicate with an MQTT server.
	 * This uses file-based persistence in the specified directory.
	 * @param serverURI
	 * @param clientId
	 * @param persistDir
	 */
	client(const string& serverURI, const string& clientId,
		   const string& persistDir);
	/**
	 * Create a client that can be used to communicate with an MQTT server.
	 * This allows the caller to specify a user-defined persistence object,
	 * or use no persistence.
	 * @param serverURI
	 * @param clientId
	 * @param persistence The user persistence structure. If this is null,
	 *  				  then no persistence is used.
	 */
	client(const string& serverURI, const string& clientId,
		   iclient_persistence* persistence);

	/**
	 * Virtual destructor
	 */
	virtual ~client() {}
	/**
	 * Close the client and releases all resource associated with the
	 * client.
	 */
	virtual void close();
	/**
	 * Connects to an MQTT server using the default options.
	 */
	virtual void connect() {
		cli_.connect()->wait_for_completion(timeout_);
	}
	/**
	 * Connects to an MQTT server using the specified options.
	 * @param opts
	 */
	virtual void connect(connect_options opts) {
		cli_.connect(std::move(opts))->wait_for_completion(timeout_);
	}
	/**
	 * Disconnects from the server.
	 */
	virtual void disconnect() {
		cli_.disconnect()->wait_for_completion(timeout_);
	}
	/**
	 * Disconnects from the server.
	 * @param timeoutMS the amount of time in milliseconds to allow for
	 *  			  existing work to finish before disconnecting. A value
	 *  			  of zero or less means the client will not quiesce.
	 */
	virtual void disconnect(int timeoutMS);
	/**
	 * Disconnects from the server.
	 * @param to the amount of time in milliseconds to allow for
	 *  			  existing work to finish before disconnecting. A value
	 *  			  of zero or less means the client will not quiesce.
	 */
	template <class Rep, class Period>
	void disconnect(const std::chrono::duration<Rep, Period>& to) {
		disconnect((int) to_milliseconds(to).count());
	}
	/**
	 * Returns a randomly generated client identifier based on the current
	 * user's login name and the system time.
	 */
	//static string generate_client_id();
	/**
	 * Returns the client ID used by this client.
	 * @return string
	 */
	virtual string get_client_id() const { return cli_.get_client_id(); }
	/**
	 * Returns the delivery tokens for any outstanding publish operations.
	 */
	virtual std::vector<delivery_token_ptr> get_pending_delivery_tokens() const;
	/**
	 * Returns the address of the server used by this client, as a URI.
	 * @return string
	 */
	virtual string get_server_uri() const { return cli_.get_server_uri(); }
	/**
	 * Return the maximum time to wait for an action to complete.
	 * @return int
	 */
	virtual std::chrono::milliseconds get_timeout() const { return timeout_; }
	/**
	 * Get a topic object which can be used to publish messages.
	 * @param top
	 * @return topic
	 */
	virtual topic get_topic(const string& top) { return topic(top, cli_); }
	/**
	 * Determines if this client is currently connected to the server.
	 * @return bool
	 */
	virtual bool is_connected() const { return cli_.is_connected(); }

	/**
	 * Publishes a message to a topic on the server and return once it is
	 * delivered.
	 * @param top The topic to publish
	 * @param payload The data to publish
	 * @param n The size in bytes of the data
	 * @param qos
	 * @param retained
	 */
	virtual void publish(const string& top, const void* payload, size_t n,
						 int qos, bool retained);
	/**
	 * Publishes a message to a topic on the server.
	 * @param top The topic to publish on
	 * @param msg The message
	 */
	virtual void publish(const string& top, const_message_ptr msg);
	/**
	 * Publishes a message to a topic on the server.
	 * @param top The topic to publish on
	 * @param msg The message
	 */
	virtual void publish(const string& top, const message& msg);
	/**
	 * Sets the callback listener to use for events that happen
	 * asynchronously.
	 * @param cb The callback functions
	 */
	virtual void set_callback(callback& cb);
	/**
	 * Set the maximum time to wait for an action to complete.
	 * @param timeoutMS
	 */
	virtual void set_timeout(int timeoutMS) {
		timeout_ = std::chrono::milliseconds(timeoutMS);
	}
	/**
	 * Set the maximum time to wait for an action to complete.
	 * @param to The timeout as a std::chrono duration.
	 */
	template <class Rep, class Period>
	void set_timeout(const std::chrono::duration<Rep, Period>& to) {
		timeout_ = to_milliseconds(to);
	}
	/**
	 * Subscribe to a topic, which may include wildcards using a QoS of 1.
	 * @param topicFilter
	 */
	virtual void subscribe(const string& topicFilter);
	/**
	 * Subscribes to a one or more topics, which may include wildcards using
	 * a QoS of 1.
	 * @param topicFilters A set of topics to subscribe
	 */
	virtual void subscribe(const string_collection& topicFilters);
	/**
	 * Subscribes to multiple topics, each of which may include wildcards.
	 * @param topicFilters A collection of topics to subscribe
	 * @param qos A collection of QoS for each topic
	 */
	virtual void subscribe(const string_collection& topicFilters,
						   const qos_collection& qos);
	/**
	 * Subscribe to a topic, which may include wildcards.
	 * @param topicFilter A single topic to subscribe
	 * @param qos The QoS of the subscription
	 */
	virtual void subscribe(const string& topicFilter, int qos);
	/**
	 * Requests the server unsubscribe the client from a topic.
	 * @param topicFilter A single topic to unsubscribe.
	 */
	virtual void unsubscribe(const string& topicFilter);
	/**
	 * Requests the server unsubscribe the client from one or more topics.
	 * @param topicFilters A collection of topics to unsubscribe.
	 */
	virtual void unsubscribe(const string_collection& topicFilters);
};

/** Smart/shared pointer to an MQTT synchronous client object */
using client_ptr = client::ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_client_h

