/////////////////////////////////////////////////////////////////////////////
/// @file iasync_client.h
/// Implementation of the interface for the asynchronous clients,
/// 'iasync_client'
/// @date 25-Aug-2016
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


#ifndef __mqtt_iasync_client_h
#define __mqtt_iasync_client_h

#include "mqtt/types.h"
#include "mqtt/token.h"
#include "mqtt/delivery_token.h"
#include "mqtt/iclient_persistence.h"
#include "mqtt/iaction_listener.h"
#include "mqtt/connect_options.h"
#include "mqtt/disconnect_options.h"
#include "mqtt/exception.h"
#include "mqtt/message.h"
#include "mqtt/callback.h"
#include <vector>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Enables an application to communicate with an MQTT server using
 * non-blocking methods.
 *
 * It provides applications a simple programming interface to all features
 * of the MQTT version 3.1 specification including:
 *
 * @li connect
 * @li publish
 * @li subscribe
 * @li unsubscribe
 * @li disconnect
 */
class iasync_client
{
	friend class token;
	virtual void remove_token(token* tok) =0;

public:
	/** Type for a collection of QOS values */
	using qos_collection = std::vector<int>;

	/**
	 * Virtual destructor
	 */
	virtual ~iasync_client() {}
	/**
	 * Connects to an MQTT server using the default options.
	 * @return token used to track and wait for the connect to complete. The
	 *  	   token will be passed to any callback that has been set.
	 * @throw exception for non security related problems
	 * @throw security_exception for security related problems
	 */
	virtual token_ptr connect() =0;
	/**
	 * Connects to an MQTT server using the provided connect options.
	 * @param options a set of connection parameters that override the
	 *  			  defaults.
	 * @return token used to track and wait for the connect to complete. The
	 *  	   token will be passed to any callback that has been set.
	 * @throw exception for non security related problems
	 * @throw security_exception for security related problems
	 */
	virtual token_ptr connect(connect_options options) =0;
	/**
	 * Connects to an MQTT server using the specified options.
	 *
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
	virtual token_ptr connect(connect_options options, void* userContext,
							   iaction_listener& cb) =0;
	/**
	 *
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb listener that will be notified when the connect completes.
	 * @return token used to track and wait for the connect to complete. The
	 *  	   token will be passed to any callback that has been set.
	 * @throw exception for non security related problems
	 * @throw security_exception for security related problems
	 */
	virtual token_ptr connect(void* userContext, iaction_listener& cb) =0;
	/**
	 * Reconnects the client using options from the previous connect.
	 * The client must have previously called connect() for this to work.
	 * @return token used to track the progress of the reconnect.
	 */
	virtual token_ptr reconnect() =0;
	/**
	 * Disconnects from the server.
	 * @return token used to track and wait for the disconnect to complete.
	 *  	   The token will be passed to any callback that has been set.
	 * @throw exception for problems encountered while disconnecting
	 */
	virtual token_ptr disconnect() =0;
	/**
	 * Disconnects from the server.
	 * @param opts Options for disconnecting.
	 * @return token used to track and wait for the disconnect to complete.
	 *  	   The token will be passed to any callback that has been set.
	 * @throw exception for problems encountered while disconnecting
	 */
	virtual token_ptr disconnect(disconnect_options opts) =0;
	/**
	 * Disconnects from the server.
	 * @param timeout the amount of time in milliseconds to allow for
	 *  			  existing work to finish before disconnecting. A value
	 *  			  of zero or less means the client will not quiesce.
	 * @return token used to track and wait for the disconnect to complete.
	 *  	   The token will be passed to any callback that has been set.
	 * @throw exception for problems encountered while disconnecting
	 */
	virtual token_ptr disconnect(int timeout) =0;
	/**
	 * Disconnects from the server.
	 * @param timeout the amount of time in milliseconds to allow for
	 *  			  existing work to finish before disconnecting. A value
	 *  			  of zero or less means the client will not quiesce.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb listener that will be notified when the disconnect
	 *  		 completes.
	 * @return token used to track and wait for the disconnect to complete.
	 *  	   The token will be passed to any callback that has been set.
	 * @throw exception for problems encountered while disconnecting
	 */
	virtual token_ptr disconnect(int timeout, void* userContext, iaction_listener& cb) =0;
	/**
	 * Disconnects from the server.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb listener that will be notified when the disconnect
	 *  		 completes.
	 * @return token used to track and wait for the disconnect to complete.
	 *  	   The token will be passed to any callback that has been set.
	 * @throw exception for problems encountered while disconnecting
	 */
	virtual token_ptr disconnect(void* userContext, iaction_listener& cb) =0;
	/**
	 * Returns the delivery token for the specified message ID.
	 * @return delivery_token
	 */
	virtual delivery_token_ptr get_pending_delivery_token(int msgID) const =0;
	/**
	 * Returns the delivery tokens for any outstanding publish operations.
	 * @return delivery_token[]
	 */
	virtual std::vector<delivery_token_ptr> get_pending_delivery_tokens() const =0;
	/**
	 * Returns the client ID used by this client.
	 * @return string
	 */
	virtual string get_client_id() const =0;
	/**
	 * Returns the address of the server used by this client.
	 */
	virtual string get_server_uri() const =0;
	/**
	 * Determines if this client is currently connected to the server.
	 */
	virtual bool is_connected() const =0;
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
	virtual delivery_token_ptr publish(string_ref topic,
									   const void* payload, size_t n,
									   int qos, bool retained) =0;
	/**
	 * Publishes a message to a topic on the server
	 * @param topic The topic to deliver the message to
	 * @param payload the bytes to use as the message payload
	 * @param n the number of bytes in the payload
	 * @return token used to track and wait for the publish to complete. The
	 *  	   token will be passed to callback methods if set.
	 */
	virtual delivery_token_ptr publish(string_ref topic,
									   const void* payload, size_t n) =0;
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
	virtual delivery_token_ptr publish(string_ref topic,
									   const void* payload, size_t n,
									   int qos, bool retained,
									   void* userContext, iaction_listener& cb) =0;
	/**
	 * Publishes a message to a topic on the server
	 * @param topic The topic to deliver the message to
	 * @param payload the bytes to use as the message payload
	 * @param qos the Quality of Service to deliver the message at. Valid
	 *  		  values are 0, 1 or 2.
	 * @param retained whether or not this message should be retained by the
	 *  			   server.
	 * @return token used to track and wait for the publish to complete. The
	 *  	   token will be passed to callback methods if set.
	 */
	virtual delivery_token_ptr publish(string_ref topic, binary_ref payload,
									   int qos, bool retained) =0;
	/**
	 * Publishes a message to a topic on the server.
	 * @param topic The topic to deliver the message to
	 * @param payload the bytes to use as the message payload
	 * @return token used to track and wait for the publish to complete. The
	 *  	   token will be passed to callback methods if set.
	 */
	virtual delivery_token_ptr publish(string_ref topic, binary_ref payload) =0;
	/**
	 * Publishes a message to a topic on the server Takes an Message
	 * message and delivers it to the server at the requested quality of
	 * service.
	 * @param msg the message to deliver to the server
	 * @return token used to track and wait for the publish to complete. The
	 *  	   token will be passed to callback methods if set.
	 */
	virtual delivery_token_ptr publish(const_message_ptr msg) =0;
	/**
	 * Publishes a message to a topic on the server.
	 * @param msg the message to deliver to the server
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb optional listener that will be notified when message
	 *  		 delivery has completed to the requested quality of service
	 * @return token used to track and wait for the publish to complete. The
	 *  	   token will be passed to callback methods if set.
	 */
	virtual delivery_token_ptr publish(const_message_ptr msg,
									   void* userContext, iaction_listener& cb) =0;
	/**
	 * Sets a callback listener to use for events that happen
	 * asynchronously.
	 * @param cb callback which will be invoked for certain asynchronous
	 *  		 events
	 */
	virtual void set_callback(callback& cb) =0;
	/**
	 * Stops the callbacks.
	 */
	virtual void disable_callbacks() = 0;
	/**
	 * Subscribe to multiple topics, each of which may include wildcards.
	 * Provides an optimized way to subscribe to multiple topics compared to
	 * subscribing to each one individually.
	 * @param topicFilters one or more topics to subscribe to, which can
	 *  				   include wildcards
	 * @param qos the maximum quality of service at which to subscribe.
	 *  		  Messages published at a lower quality of service will be
	 *  		  received at the published QoS. Messages published at a
	 *  		  higher quality of service will be received using the QoS
	 *  		  specified on the subscribe.
	 * @return token used to track and wait for the subscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	virtual token_ptr subscribe(const_string_collection_ptr topicFilters,
								const qos_collection& qos) =0;
	/**
	 * Subscribes to multiple topics, each of which may include wildcards.
	 * @param topicFilters one or more topics to subscribe to, which can
	 *  				   include wildcards
	 * @param qos  the maximum quality of service at which to subscribe.
	 *  		   Messages published at a lower quality of service will be
	 *  		   received at the published QoS. Messages published at a
	 *  		   higher quality of service will be received using the QoS
	 *  		   specified on the subscribe.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param callback listener that will be notified when subscribe has
	 *  			   completed
	 * @return token used to track and wait for the subscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	virtual token_ptr subscribe(const_string_collection_ptr topicFilters,
								 const qos_collection& qos,
								 void* userContext, iaction_listener& callback) =0;
	/**
	 * Subscribe to a topic, which may include wildcards.
	 * @param topicFilter the topic to subscribe to, which can include
	 *  				  wildcards.
	 * @param qos the maximum quality of service at which to subscribe.
	 *  		  Messages published at a lower quality of service will be
	 *  		  received at the published QoS. Messages published at a
	 *  		  higher quality of service will be received using the QoS
	 *  		  specified on the subscribe.
	 * @return token used to track and wait for the subscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	virtual token_ptr subscribe(const string& topicFilter, int qos) =0;
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
	 * @param callback listener that will be notified when subscribe has
	 *  			   completed
	 * @return token used to track and wait for the subscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	virtual token_ptr subscribe(const string& topicFilter, int qos,
								void* userContext, iaction_listener& callback) =0;
	/**
	 * Requests the server unsubscribe the client from a topic.
	 * @param topicFilter the topic to unsubscribe from. It must match a
	 *  				  topicFilter specified on an earlier subscribe.
	 * @return token used to track and wait for the unsubscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	virtual token_ptr unsubscribe(const string& topicFilter) =0;
	/**
	 * Requests the server unsubscribe the client from one or more topics.
	 * @param topicFilters one or more topics to unsubscribe from. Each
	 *  				   topicFilter must match one specified on an
	 *  				   earlier subscribe.
	 * @return token used to track and wait for the unsubscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	virtual token_ptr unsubscribe(const_string_collection_ptr topicFilters) =0;
	/**
	 * Requests the server unsubscribe the client from one or more topics.
	 * @param topicFilters one or more topics to unsubscribe from. Each
	 *  				   topicFilter must match one specified on an
	 *  				   earlier subscribe.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb listener that will be notified when unsubscribe has
	 *  		 completed
	 * @return token used to track and wait for the unsubscribe to complete.
	 *  	   The token will be passed to callback methods if set.
	 */
	virtual token_ptr unsubscribe(const_string_collection_ptr topicFilters,
								   void* userContext, iaction_listener& cb) =0;
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
	virtual token_ptr unsubscribe(const string& topicFilter,
								  void* userContext, iaction_listener& cb) =0;
};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		// __mqtt_iasync_client_h

