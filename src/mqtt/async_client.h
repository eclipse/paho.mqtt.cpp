/////////////////////////////////////////////////////////////////////////////
/// @file async_client.h
/// Declaration of MQTT async_client class
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

#ifndef __mqtt_async_client_h
#define __mqtt_async_client_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/token.h"
#include "mqtt/delivery_token.h"
#include "mqtt/iclient_persistence.h"
#include "mqtt/iaction_listener.h"
#include "mqtt/connect_options.h"
#include "mqtt/exception.h"
#include "mqtt/message.h"
#include "mqtt/callback.h"
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <stdexcept>

namespace mqtt {

const uint32_t		VERSION = 0x00010000;
const std::string	VERSION_STR("mqttpp v. 0.1"),
					COPYRIGHT("Copyright (c) 2013 Frank Pagliughi");

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
	virtual void remove_token(itoken* tok) =0;

public:
	/** Type for a collection of filters */
	typedef std::vector<std::string> topic_filter_collection;
	/** Type for a collection of QOS values */
	typedef std::vector<int> qos_collection;

	/**
	 * Virtual destructor
	 */
	virtual ~iasync_client() {}
	/**
	 * Connects to an MQTT server using the default options.
	 * @return bool 
	 * @throw exception 
	 * @throw security_exception
	 */
	virtual itoken_ptr connect() throw(exception, security_exception) =0;
	/**
	 * Connects to an MQTT server using the provided connect options. 
	 * @param options 
	 * @return bool 
	 * @throw exception 
	 * @throw security_exception
	 */
	virtual itoken_ptr connect(connect_options options) 
							throw(exception, security_exception) =0;
	/**
	 * Connects to an MQTT server using the specified options.
	 * 
	 * @param options 
	 * 
	 * @return bool 
	 * @throw exception 
	 * @throw security_exception
	 */
	virtual itoken_ptr connect(connect_options options, void* userContext, 
							   iaction_listener& cb) throw(exception, security_exception) =0;
	/**
	 *  
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 * @throw exception 
	 * @throw security_exception
	 */
	virtual itoken_ptr connect(void* userContext, iaction_listener& cb)
							throw(exception, security_exception) =0;
	/**
	 * Disconnects from the server. 
	 * @return itoken_ptr
	 */
	virtual itoken_ptr disconnect() throw(exception) =0;
	/**
	 * Disconnects from the server.
	 * 
	 * @param quiesceTimeout 
	 * @return itoken_ptr
	 */
	virtual itoken_ptr disconnect(long quiesceTimeout) throw(exception) =0;
	/**
	 * Disconnects from the server.
	 * 
	 * @param quiesceTimeout 
	 * @param userContext 
	 * @param callback 
	 * @return itoken_ptr
	 */
	virtual itoken_ptr disconnect(long quiesceTimeout, void* userContext, iaction_listener& cb)
						throw(exception) =0;
	/**
	 * Disconnects from the server.
	 * @param userContext 
	 * @param callback 
	 * @return itoken_ptr
	 */
	virtual itoken_ptr disconnect(void* userContext, iaction_listener& cb) 
						throw(exception) =0;
	/**
	 * Returns the delivery token for the specified message ID.
	 * @return idelivery_token
	 */
	virtual idelivery_token_ptr get_pending_delivery_token(int msgID) const =0;
	/**
	 * Returns the delivery tokens for any outstanding publish operations. 
	 * @return idelivery_token[]
	 */
	virtual std::vector<idelivery_token_ptr> get_pending_delivery_tokens() const =0;
	/**
	 * Returns the client ID used by this client.
	 * @return std::string 
	 */
	virtual std::string get_client_id() const =0;
	/**
	 * Returns the address of the server used by this client.
	 */
	virtual std::string get_server_uri() const =0;
	/**
	 * Determines if this client is currently connected to the server.
	 */
	virtual bool is_connected() const =0;
	/**
	 * Publishes a message to a topic on the server
	 * @param topic 
	 * @param payload 
	 * @param qos 
	 * @param retained 
	 * 
	 * @return idelivery_token 
	 */
	virtual idelivery_token_ptr publish(const std::string& topic, const void* payload, 
										size_t n, int qos, bool retained) 
									throw(exception) =0;
	/**
	 * Publishes a message to a topic on the server 
	 * @param topic 
	 * @param payload 
	 * @param qos 
	 * @param retained 
	 * @param userContext 
	 * @param cb 
	 * 
	 * @return idelivery_token 
	 */
	virtual idelivery_token_ptr publish(const std::string& topic, 
										const void* payload, size_t n,
										int qos, bool retained, void* userContext, 
										iaction_listener& cb) throw(exception) =0;
	/**
	 * Publishes a message to a topic on the server Takes an Message 
	 * message and delivers it to the server at the requested quality of 
	 * service. 
	 * 
	 * @param topic 
	 * @param message 
	 * 
	 * @return idelivery_token 
	 */
	virtual idelivery_token_ptr publish(const std::string& topic, message_ptr msg)
					throw(exception) =0;
	/**
	 * Publishes a message to a topic on the server. 
	 * @param topic 
	 * @param message 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return idelivery_token 
	 */
	virtual idelivery_token_ptr publish(const std::string& topic, message_ptr msg, 
										void* userContext, iaction_listener& cb)
					throw(exception) =0;
	/**
	 * Sets a callback listener to use for events that happen 
	 * asynchronously. 
	 * @param callback 
	 */
	virtual void set_callback(callback& cb) throw(exception) =0;
	/**
	 * Subscribe to multiple topics, each of which may include wildcards. 
	 * @param topicFilters 
	 * @param qos 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr subscribe(const topic_filter_collection& topicFilters, 
								 const qos_collection& qos) 
					throw(std::invalid_argument,exception) =0;
	/**
	 * Subscribes to multiple topics, each of which may include wildcards. 
	 * @param topicFilters 
	 * @param qos 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr subscribe(const topic_filter_collection& topicFilters, 
								 const qos_collection& qos,
								 void* userContext, iaction_listener& callback)
					throw(std::invalid_argument,exception) =0;
	/**
	 * Subscribe to a topic, which may include wildcards. 
	 * @param topicFilter 
	 * @param qos 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr subscribe(const std::string& topicFilter, int qos) 
					throw(exception) =0;
	/**
	 * Subscribe to a topic, which may include wildcards. 
	 * @param topicFilter 
	 * @param qos 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr subscribe(const std::string& topicFilter, int qos, 
								 void* userContext, iaction_listener& callback)
					throw(exception) =0;
	/**
	 * Requests the server unsubscribe the client from a topic. 
	 * @param topicFilter 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr unsubscribe(const std::string& topicFilter) throw(exception) =0;
	/**
	 * Requests the server unsubscribe the client from one or more topics. 
	 * @param string 
	 * @return bool 
	 */
	virtual itoken_ptr unsubscribe(const topic_filter_collection& topicFilters) 
					throw(exception) =0;
	/**
	 * Requests the server unsubscribe the client from one or more topics. 
	 * @param string 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr unsubscribe(const topic_filter_collection& topicFilters, 
								   void* userContext, iaction_listener& callback)
					throw(exception) =0;
	/**
	 * Requests the server unsubscribe the client from a topics. 
	 * @param topicFilter 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr unsubscribe(const std::string& topicFilter, 
								   void* userContext, iaction_listener& callback)
					throw(exception) =0;
};

/////////////////////////////////////////////////////////////////////////////  

/**
 * Lightweight client for talking to an MQTT server using non-blocking
 * methods that allow an operation to run in the background.
 */
class async_client : public virtual iasync_client
{
public:
	/** Pointer type for this object */
	typedef std::shared_ptr<async_client> ptr_t;

private:
	/** Lock guard type for this class */
	typedef std::unique_lock<std::mutex> guard;

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
	virtual void remove_token(itoken* tok);
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
	 * @param serverURI 
	 * @param clientId 
	 */
	async_client(const std::string& serverURI, const std::string& clientId);
	/**
	 * Create an async_client that can be used to communicate with an MQTT
	 * server. 
	 * This uses file-based persistence in the specified directory. 
	 * @param serverURI 
	 * @param clientId 
	 * @param persistDir 
	 */
	async_client(const std::string& serverURI, const std::string& clientId,
				 const std::string& persistDir);
	/**
	 * Create an async_client that can be used to communicate with an MQTT
	 * server. 
	 * This allows the caller to specify a user-defined persistance object, 
	 * or use no persistence. 
	 * @param serverURI 
	 * @param clientId 
	 * @param persistence The user persistence structure. If this is null, 
	 *  				  then no persistence is used.
	 */
	async_client(const std::string& serverURI, const std::string& clientId, 
				 iclient_persistence* persistence);
	/**
	 * Destructor
	 */
	~async_client();
	/**
	 * Connects to an MQTT server using the default options.
	 * @return bool 
	 * @throw exception 
	 * @throw security_exception
	 */
	virtual itoken_ptr connect() throw(exception, security_exception);
	/**
	 * Connects to an MQTT server using the provided connect options. 
	 * @param options 
	 * @return bool 
	 * @throw exception 
	 * @throw security_exception
	 */
	virtual itoken_ptr connect(connect_options options) throw(exception, security_exception);
	/**
	 * Connects to an MQTT server using the specified options.
	 * 
	 * @param options 
	 * 
	 * @return bool 
	 * @throw exception 
	 * @throw security_exception
	 */
	virtual itoken_ptr connect(connect_options options, void* userContext, 
							   iaction_listener& cb) throw(exception, security_exception);
	/**
	 *  
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 * @throw exception 
	 * @throw security_exception
	 */
	virtual itoken_ptr connect(void* userContext, iaction_listener& cb)
							throw(exception, security_exception);
	/**
	 * Disconnects from the server. 
	 * @return itoken_ptr
	 */
	virtual itoken_ptr disconnect() throw(exception) { return disconnect(0L); }
	/**
	 * Disconnects from the server.
	 * 
	 * @param quiesceTimeout 
	 * @return itoken_ptr
	 */
	virtual itoken_ptr disconnect(long quiesceTimeout) throw(exception);
	/**
	 * Disconnects from the server.
	 * 
	 * @param quiesceTimeout 
	 * @param userContext 
	 * @param callback 
	 * @return itoken_ptr
	 */
	virtual itoken_ptr disconnect(long quiesceTimeout, void* userContext, iaction_listener& cb)
						throw(exception);
	/**
	 * Disconnects from the server.
	 * @param userContext 
	 * @param callback 
	 * @return itoken_ptr
	 */
	virtual itoken_ptr disconnect(void* userContext, iaction_listener& cb) throw(exception) {
		return disconnect(0L, userContext, cb);
	}
	/**
	 * Returns the delivery token for the specified message ID.
	 * @return idelivery_token
	 */
	virtual idelivery_token_ptr get_pending_delivery_token(int msgID) const;
	/**
	 * Returns the delivery tokens for any outstanding publish operations. 
	 * @return idelivery_token[]
	 */
	virtual std::vector<idelivery_token_ptr> get_pending_delivery_tokens() const;
	/**
	 * Returns the client ID used by this client.
	 * @return std::string 
	 */
	virtual std::string get_client_id() const { return clientId_; }
	/**
	 * Returns the address of the server used by this client.
	 */
	virtual std::string get_server_uri() const { return serverURI_; }
	/**
	 * Determines if this client is currently connected to the server.
	 */
	virtual bool is_connected() const { return MQTTAsync_isConnected(cli_) != 0; }
	/**
	 * Publishes a message to a topic on the server
	 * @param topic 
	 * @param payload 
	 * @param qos 
	 * @param retained 
	 * 
	 * @return idelivery_token 
	 */
	virtual idelivery_token_ptr publish(const std::string& topic, const void* payload, 
										size_t n, int qos, bool retained) throw(exception);
	/**
	 * Publishes a message to a topic on the server 
	 * @param topic 
	 * @param payload 
	 * @param qos 
	 * @param retained 
	 * @param userContext 
	 * @param cb 
	 * 
	 * @return idelivery_token 
	 */
	virtual idelivery_token_ptr publish(const std::string& topic, 
										const void* payload, size_t n,
										int qos, bool retained, void* userContext, 
										iaction_listener& cb) throw(exception);
	/**
	 * Publishes a message to a topic on the server Takes an Message 
	 * message and delivers it to the server at the requested quality of 
	 * service. 
	 * 
	 * @param topic 
	 * @param message 
	 * 
	 * @return idelivery_token 
	 */
	virtual idelivery_token_ptr publish(const std::string& topic, message_ptr msg)
					throw(exception);
	/**
	 * Publishes a message to a topic on the server. 
	 * @param topic 
	 * @param message 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return idelivery_token 
	 */
	virtual idelivery_token_ptr publish(const std::string& topic, message_ptr msg, 
										void* userContext, iaction_listener& cb)
					throw(exception);
	/**
	 * Sets a callback listener to use for events that happen 
	 * asynchronously. 
	 * @param callback 
	 */
	virtual void set_callback(callback& cb) throw(exception);
	/**
	 * Subscribe to multiple topics, each of which may include wildcards. 
	 * @param topicFilters 
	 * @param qos 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr subscribe(const topic_filter_collection& topicFilters, 
								 const qos_collection& qos) 
					throw(std::invalid_argument,exception);
	/**
	 * Subscribes to multiple topics, each of which may include wildcards. 
	 * @param topicFilters 
	 * @param qos 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr subscribe(const topic_filter_collection& topicFilters, 
								 const qos_collection& qos,
								 void* userContext, iaction_listener& callback)
					throw(std::invalid_argument,exception);
	/**
	 * Subscribe to a topic, which may include wildcards. 
	 * @param topicFilter 
	 * @param qos 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr subscribe(const std::string& topicFilter, int qos) 
					throw(exception);
	/**
	 * Subscribe to a topic, which may include wildcards. 
	 * @param topicFilter 
	 * @param qos 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr subscribe(const std::string& topicFilter, int qos, 
								 void* userContext, iaction_listener& callback)
					throw(exception);
	/**
	 * Requests the server unsubscribe the client from a topic. 
	 * @param topicFilter 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr unsubscribe(const std::string& topicFilter) throw(exception);
	/**
	 * Requests the server unsubscribe the client from one or more topics. 
	 * @param string 
	 * @return bool 
	 */
	virtual itoken_ptr unsubscribe(const topic_filter_collection& topicFilters) 
					throw(exception);
	/**
	 * Requests the server unsubscribe the client from one or more topics. 
	 * @param string 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr unsubscribe(const topic_filter_collection& topicFilters, 
								   void* userContext, iaction_listener& callback)
					throw(exception);
	/**
	 * Requests the server unsubscribe the client from a topics. 
	 * @param topicFilter 
	 * @param userContext 
	 * @param callback 
	 * 
	 * @return bool 
	 */
	virtual itoken_ptr unsubscribe(const std::string& topicFilter, 
								   void* userContext, iaction_listener& callback)
					throw(exception);
};

/**
 * Shared pointer to an asynchronous MQTT client object.
 */
typedef async_client::ptr_t async_client_ptr;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_async_client_h

