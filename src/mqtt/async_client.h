/////////////////////////////////////////////////////////////////////////////
/// @file async_client.h
/// Declaration of MQTT async_client class
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

#ifndef __mqtt_async_client_h
#define __mqtt_async_client_h

#include "MQTTAsync.h"
#include "mqtt/token.h"
#include "mqtt/delivery_token.h"
#include "mqtt/iclient_persistence.h"
#include "mqtt/iaction_listener.h"
#include "mqtt/exception.h"
#include "mqtt/message.h"
#include "mqtt/callback.h"
#include "mqtt/iasync_client.h"
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <stdexcept>

namespace mqtt {

const uint32_t		VERSION = 0x00050000;
const std::string	VERSION_STR("mqttpp v. 0.5"),
					COPYRIGHT("Copyright (c) 2013-2016 Frank Pagliughi");

/////////////////////////////////////////////////////////////////////////////

/**
 * Lightweight client for talking to an MQTT server using non-blocking
 * methods that allow an operation to run in the background.
 */
class async_client : public virtual iasync_client
{
public:
	/** Smart/shared pointer for an object of this class */
	using ptr_t = std::shared_ptr<async_client>;

private:
	/** Lock guard type for this class */
	using guard = std::unique_lock<std::mutex>;

	/** Object monitor mutex */
	mutable std::mutex lock_;
	/** The underlying C-lib client. */
	MQTTAsync cli_;
	/** The server URI string. */
	std::string serverURI_;
	/** The client ID string that we provided to the server. */
	std::string clientId_;
	/** A user persistence wrapper (if any) */
	MQTTClient_persistence* persist_;
	/** Callback supplied by the user (if any) */
	callback* userCallback_;
	/** A list of tokens that are in play */
	std::list<itoken_ptr> pendingTokens_;
	/** A list of delivery tokens that are in play */
	std::list<idelivery_token_ptr> pendingDeliveryTokens_;

	static void on_connection_lost(void *context, char *cause);
	static int on_message_arrived(void* context, char* topicName, int topicLen,
								  MQTTAsync_message* msg);
	static void on_delivery_complete(void* context, MQTTAsync_token tok);

	/** Manage internal list of active tokens */
	friend class token;
	virtual void add_token(itoken_ptr tok);
	virtual void add_token(idelivery_token_ptr tok);
	virtual void remove_token(itoken* tok) override;
	virtual void remove_token(itoken_ptr tok) { remove_token(tok.get()); }
	void remove_token(idelivery_token_ptr tok) { remove_token(tok.get()); }

	/** Memory management for C-style filter collections */
	std::vector<char*> alloc_topic_filters(
							const topic_filter_collection& topicFilters);
	void free_topic_filters(std::vector<char*>& filts);

	/**
	 * Convenience function to get user callback safely.
	 * @return callback*
	 */
	callback* get_callback() const {
		guard g(lock_);
		return userCallback_;
	}

	/** Non-copyable */
	async_client() =delete;
	async_client(const async_client&) =delete;
	async_client& operator=(const async_client&) =delete;

public:
	/**
	 * Create an async_client that can be used to communicate with an MQTT
	 * server.
	 * This uses file-based persistence in the current working directory.
	 * @param serverURI the address of the server to connect to, specified
	 *  				as a URI.
	 * @param clientId a client identifier that is unique on the server
	 *  			   being connected to.
	 */
	async_client(const std::string& serverURI, const std::string& clientId);
	/**
	 * Create an async_client that can be used to communicate with an MQTT
	 * server.
	 * This uses file-based persistence in the specified directory.
	 * @param serverURI the address of the server to connect to, specified
	 *  				as a URI.
	 * @param clientId a client identifier that is unique on the server
	 *  			   being connected to
	 * @param persistDir
	 */
	async_client(const std::string& serverURI, const std::string& clientId,
				 const std::string& persistDir);
	/**
	 * Create an async_client that can be used to communicate with an MQTT
	 * server.
	 * This allows the caller to specify a user-defined persistence object,
	 * or use no persistence.
	 * @param serverURI the address of the server to connect to, specified
	 *  				as a URI.
	 * @param clientId a client identifier that is unique on the server
	 *  			   being connected to
	 * @param persistence The user persistence structure. If this is null,
	 *  				  then no persistence is used.
	 */
	async_client(const std::string& serverURI, const std::string& clientId,
				 iclient_persistence* persistence);
	/**
	 * Destructor
	 */
	~async_client() override;
	/**
	 * Connects to an MQTT server using the default options.
	 * @return token used to track and wait for the connect to complete. The
	 *  	   token will be passed to any callback that has been set.
	 * @throw exception for non security related problems
	 * @throw security_exception for security related problems
	 */
	itoken_ptr connect() override;
	/**
	 * Connects to an MQTT server using the provided connect options.
	 * @param options a set of connection parameters that override the
	 *  			  defaults.
	 * @return token used to track and wait for the connect to complete. The
	 *  	   token will be passed to any callback that has been set.
	 * @throw exception for non security related problems
	 * @throw security_exception for security related problems
	 */
	itoken_ptr connect(connect_options options) override;
	/**
	 * Connects to an MQTT server using the specified options.
	 * @param options a set of connection parameters that override the
	 *  			  defaults.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when the connect
	 *  			   completes.
	 * @return token used to track and wait for the connect to complete. The
	 *  	   token will be passed to any callback that has been set.
	 * @throw exception for non security related problems
	 * @throw security_exception for security related problems
	 */
	itoken_ptr connect(connect_options options, void* userContext,
					   iaction_listener& cb) override;
	/**
	 *
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when the connect
	 *  			   completes.
	 * @return token used to track and wait for the connect to complete. The
	 *  	   token will be passed to any callback that has been set.
	 * @throw exception for non security related problems
	 * @throw security_exception for security related problems
	 */
	itoken_ptr connect(void* userContext, iaction_listener& cb) override;
	/**
	 * Disconnects from the server.
	 * @return token used to track and wait for the disconnect to complete.
	 *  	   The token will be passed to any callback that has been set.
	 * @throw exception for problems encountered while disconnecting
	 */
	itoken_ptr disconnect() override { return disconnect(0L); }
	/**
	 * Disconnects from the server.
	 *
	 * @param quiesceTimeout the amount of time in milliseconds to allow for
	 *  					 existing work to finish before disconnecting. A
	 *  					 value of zero or less means the client will not
	 *  					 quiesce.
	 * @return Token used to track and wait for disconnect to complete. The
	 *  	   token will be passed to the callback methods if a callback is
	 *  	   set.
	 * @throw exception for problems encountered while disconnecting
	 */
	itoken_ptr disconnect(long quiesceTimeout) override;
	/**
	 * Disconnects from the server.
	 *
	 * @param quiesceTimeout the amount of time in milliseconds to allow for
	 *  					 existing work to finish before disconnecting. A
	 *  					 value of zero or less means the client will not
	 *  					 quiesce.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when the disconnect
	 *  			   completes.
	 * @return itoken_ptr Token used to track and wait for disconnect to
	 *  	   complete. The token will be passed to the callback methods if
	 *  	   a callback is set.
	 * @throw exception for problems encountered while disconnecting
	 */
	itoken_ptr disconnect(long quiesceTimeout, void* userContext,
						  iaction_listener& cb) override;
	/**
	 * Disconnects from the server.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when the disconnect
	 *  			   completes.
	 * @return itoken_ptr Token used to track and wait for disconnect to
	 *  	   complete. The token will be passed to the callback methods if
	 *  	   a callback is set.
	 * @throw exception for problems encountered while disconnecting
	 */
	itoken_ptr disconnect(void* userContext, iaction_listener& cb) override {
		return disconnect(0L, userContext, cb);
	}
	/**
	 * Returns the delivery token for the specified message ID.
	 * @return idelivery_token
	 */
	idelivery_token_ptr get_pending_delivery_token(int msgID) const override;
	/**
	 * Returns the delivery tokens for any outstanding publish operations.
	 * @return idelivery_token[]
	 */
	std::vector<idelivery_token_ptr> get_pending_delivery_tokens() const override;
	/**
	 * Returns the client ID used by this client.
	 * @return The client ID used by this client.
	 */
	std::string get_client_id() const override { return clientId_; }
	/**
	 * Returns the address of the server used by this client.
	 * @return The server's address, as a URI String.
	 */
	std::string get_server_uri() const override { return serverURI_; }
	/**
	 * Determines if this client is currently connected to the server.
	 * @return true if connected, false otherwise.
	 */
	bool is_connected() const override { return MQTTAsync_isConnected(cli_) != 0; }
	/**
	 * Publishes a message to a topic on the server
	 * @param topic The topic to deliver the message to
	 * @param payload the bytes to use as the message payload
	 * @param n the number of bytes in the payload
	 * @param qos the Quality of Service to deliver the message at. Valid
	 *  		  values are 0, 1 or 2.
	 * @param retained whether or not this message should be retained by the
	 *  			   server.
	 * @return token used to track and wait for the publish to complete. The
	 *  	   token will be passed to callback methods if set.
	 */
	idelivery_token_ptr publish(const std::string& topic, const void* payload,
										size_t n, int qos, bool retained) override;
	/**
	 * Publishes a message to a topic on the server
	 * @param topic The topic to deliver the message to
	 * @param payload the bytes to use as the message payload
	 * @param n the number of bytes in the payload
	 * @param qos the Quality of Service to deliver the message at. Valid
	 *  		  values are 0, 1 or 2.
	 * @param retained whether or not this message should be retained by the
	 *  			   server.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb
	 * @return token used to track and wait for the publish to complete. The
	 *  	   token will be passed to callback methods if set.
	 */
	idelivery_token_ptr publish(const std::string& topic,
										const void* payload, size_t n,
										int qos, bool retained, void* userContext,
										iaction_listener& cb) override;
	/**
	 * Publishes a message to a topic on the server Takes an Message
	 * message and delivers it to the server at the requested quality of
	 * service.
	 * @param topic the topic to deliver the message to
	 * @param msg the message to deliver to the server
	 * @return token used to track and wait for the publish to complete. The
	 *  	   token will be passed to callback methods if set.
	 */
	idelivery_token_ptr publish(const std::string& topic, const_message_ptr msg) override;
	/**
	 * Publishes a message to a topic on the server.
	 * @param topic the topic to deliver the message to
	 * @param msg the message to deliver to the server
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback optional listener that will be notified when message
	 *  			   delivery has completed to the requested quality of
	 *  			   service
	 * @return token used to track and wait for the publish to complete. The
	 *  	   token will be passed to callback methods if set.
	 */
	idelivery_token_ptr publish(const std::string& topic, const_message_ptr msg,
										void* userContext, iaction_listener& cb) override;
	/**
	 * Sets a callback listener to use for events that happen
	 * asynchronously.
	 * @param cb callback which will be invoked for certain asynchronous events
	 */
	void set_callback(callback& cb) override;
	/**
	 * Subscribe to multiple topics, each of which may include wildcards.
	 * @param topicFilters
	 * @param qos the maximum quality of service at which to subscribe.
	 *  		  Messages published at a lower quality of service will be
	 *  		  received at the published QoS. Messages published at a
	 *  		  higher quality of service will be received using the QoS
	 *  		  specified on the subscribe.
	 * @return token used to track and wait for the subscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	itoken_ptr subscribe(const topic_filter_collection& topicFilters,
								 const qos_collection& qos) override;
	/**
	 * Subscribes to multiple topics, each of which may include wildcards.
	 * @param topicFilters
	 * @param qos the maximum quality of service at which to subscribe.
	 *  		  Messages published at a lower quality of service will be
	 *  		  received at the published QoS. Messages published at a
	 *  		  higher quality of service will be received using the QoS
	 *  		  specified on the subscribe.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb listener that will be notified when subscribe has completed
	 * @return token used to track and wait for the subscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	itoken_ptr subscribe(const topic_filter_collection& topicFilters,
								 const qos_collection& qos,
								 void* userContext, iaction_listener& cb) override;
	/**
	 * Subscribe to a topic, which may include wildcards.
	 * @param topicFilter the topic to subscribe to, which can include
	 *  				  wildcards.
	 * @param qos
	 *
	 * @return token used to track and wait for the subscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	itoken_ptr subscribe(const std::string& topicFilter, int qos) override;
	/**
	 * Subscribe to a topic, which may include wildcards.
	 * @param topicFilter the topic to subscribe to, which can include
	 *  				  wildcards.
	 * @param qos the maximum quality of service at which to subscribe.
	 *  		  Messages published at a lower quality of service will be
	 *  		  received at the published QoS. Messages published at a
	 *  		  higher quality of service will be received using the QoS
	 *  		  specified on the subscribe.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb listener that will be notified when subscribe has completed
	 * @return token used to track and wait for the subscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	itoken_ptr subscribe(const std::string& topicFilter, int qos,
								 void* userContext, iaction_listener& cb) override;
	/**
	 * Requests the server unsubscribe the client from a topic.
	 * @param topicFilter the topic to unsubscribe from. It must match a
	 *  				  topicFilter specified on an earlier subscribe.
	 * @return token used to track and wait for the unsubscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	itoken_ptr unsubscribe(const std::string& topicFilter) override;
	/**
	 * Requests the server unsubscribe the client from one or more topics.
	 * @param topicFilters one or more topics to unsubscribe from. Each
	 *  				   topicFilter must match one specified on an
	 *  				   earlier subscribe.
	 * @return token used to track and wait for the unsubscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	itoken_ptr unsubscribe(const topic_filter_collection& topicFilters) override;
	/**
	 * Requests the server unsubscribe the client from one or more topics.
	 * @param topicFilters
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb listener that will be notified when unsubscribe has
	 *  		 completed
	 * @return token used to track and wait for the unsubscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	itoken_ptr unsubscribe(const topic_filter_collection& topicFilters,
								   void* userContext, iaction_listener& cb) override;
	/**
	 * Requests the server unsubscribe the client from a topics.
	 * @param topicFilter the topic to unsubscribe from. It must match a
	 *  				  topicFilter specified on an earlier subscribe.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb listener that will be notified when unsubscribe has
	 *  		 completed
	 * @return token used to track and wait for the unsubscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	itoken_ptr unsubscribe(const std::string& topicFilter,
								   void* userContext, iaction_listener& cb) override;
};

/** Smart/shared pointer to an asynchronous MQTT client object */
using async_client_ptr = async_client::ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_async_client_h

