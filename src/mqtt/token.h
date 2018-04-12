/////////////////////////////////////////////////////////////////////////////
/// @file token.h
/// Declaration of MQTT token class
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

#ifndef __mqtt_token_h
#define __mqtt_token_h

#include "MQTTAsync.h"
#include "mqtt/iaction_listener.h"
#include "mqtt/exception.h"
#include "mqtt/types.h"
#include "mqtt/buffer_ref.h"
#include "mqtt/string_collection.h"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace mqtt {

class iasync_client;

/////////////////////////////////////////////////////////////////////////////

/**
 * Provides a mechanism for tracking the completion of an asynchronous
 * action.
 */
class token
{
	/** Lock guard type for this class. */
	using guard = std::lock_guard<std::mutex>;
	/** Unique type for this class. */
	using unique_lock = std::unique_lock<std::mutex>;

	/** Object monitor mutex. */
	mutable std::mutex lock_;
	/** Condition variable signals when the action completes */
	std::condition_variable cond_;

	/** The MQTT client that is processing this action */
	iasync_client* cli_;
	/** The underlying C token. Note that this is just an integer */
	MQTTAsync_token tok_;
	/** The topic string(s) for the action being tracked by this token */
	const_string_collection_ptr topics_;
	/** User supplied context */
	void* userContext_;
	/**
	 * User supplied listener.
	 * Note that the user listener fires after the action is marked
	 * complete, but before the token is signaled.
	 */
	iaction_listener* listener_;
	/** Whether the action has yet to complete */
	bool complete_;
	/** The action success/failure code */
	int rc_;
	/** Error message from the C lib (if any) */
	string errMsg_;

	/** Client and token-related options have special access */
	friend class async_client;
	friend class token_test;

	friend class connect_options;
	friend class response_options;
	friend class delivery_response_options;
	friend class disconnect_options;

	/**
	 * Resets the token back to a non-signaled state.
	 */
	void reset();
	/**
	 * Sets the ID for the message.
	 * This is a guaranteed atomic operation.
	 * @param msgid The ID of the message.
	 */
	void set_message_id(MQTTAsync_token msgid) {
		guard g(lock_);
		tok_ = msgid;
	}
	/**
	 * C-style callback for success.
	 * This simply passes the call on to the proper token object for
	 * processing.
	 * @param tokObj The token object to process the call. Note that this is
	 *  			 @em not the user-supplied context pointer. That is
	 *  			 kept in the object itself.
	 * @param rsp The success response.
	 */
	static void on_success(void* tokObj, MQTTAsync_successData* rsp);
	/**
	 * C-style callback for failure.
	 * This simply passes the call on to the proper token object for
	 * processing.
	 * @param tokObj The token object to process the call. Note that this is
	 *  			 @em not the user-supplied context pointer. That is
	 *  			 kept in the object itself.
	 * @param rsp The failure response.
	 */
	static void on_failure(void* tokObj, MQTTAsync_failureData* rsp);
	/**
	 * C-style callback for client (re)connection.
	 * This is normally only used to process a reconnect completion message.
	 * The initial connect() is processed via on_success/failure.
	 * @param tokObj Pointer to the token object used to process the call.
	 */
	static void on_connected(void* tokObj, char* /*cause*/);
	/**
	 * Internal handler for the success callback.
	 * @param rsp The success response.
	 */
	void on_success(MQTTAsync_successData* rsp);
	/**
	 * Internal handler for the failure callback.
	 * @param rsp The failure response.
	 */
	void on_failure(MQTTAsync_failureData* rsp);

	/**
	 * Check the current return code and throw an exception if it is not a
	 * success code.
	 */
	void check_rc() {
		if (rc_ != MQTTASYNC_SUCCESS)
			throw exception(rc_, errMsg_);
	}

public:
	/** Smart/shared pointer to an object of this class */
	using ptr_t = std::shared_ptr<token>;
	/** Smart/shared pointer to an object of this class */
	using const_ptr_t = std::shared_ptr<const token>;
	/** Weak pointer to an object of this class */
	using weak_ptr_t = std::weak_ptr<token>;

	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 */
	token(iasync_client& cli);
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when subscribe has
	 *  		 completed
	 */
	token(iasync_client& cli, void* userContext, iaction_listener& cb);
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param topic The topic assiciated with the token
	 */
	token(iasync_client& cli, const string& topic);
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param topic The topic assiciated with the token
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when subscribe has
	 *  		 completed
	 */
	token(iasync_client& cli, const string& topic,
		  void* userContext, iaction_listener& cb);
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param topics The topics associated with the token
	 */
	token(iasync_client& cli, const_string_collection_ptr topics);
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param topics The topics associated with the token
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when subscribe has
	 *  		 completed
	 */
	token(iasync_client& cli, const_string_collection_ptr topics,
		  void* userContext, iaction_listener& cb);
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param tok The message ID
	 */
	token(iasync_client& cli, MQTTAsync_token tok);
	/**
	 * Virtual destructor.
	 */
	virtual ~token() {}
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @return A smart/shared pointer to a token.
	 */
	static ptr_t create(iasync_client& cli) {
		return std::make_shared<token>(cli);
	}
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when subscribe has
	 *  		 completed
	 */
	static ptr_t create(iasync_client& cli, void* userContext, iaction_listener& cb) {
		return std::make_shared<token>(cli, userContext, cb);
	}
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param topic The topic assiciated with the token
	 */
	static ptr_t create(iasync_client& cli, const string& topic) {
		return std::make_shared<token>(cli, topic);
	}
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param topic The topic assiciated with the token
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when subscribe has
	 *  		 completed
	 */
	static ptr_t create(iasync_client& cli, const string& topic,
						void* userContext, iaction_listener& cb) {
		return std::make_shared<token>(cli, topic, userContext, cb);
	}
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param topics The topics associated with the token
	 */
	static ptr_t create(iasync_client& cli, const_string_collection_ptr topics) {
		return std::make_shared<token>(cli, topics);
	}
	/**
	 * Constructs a token object.
	 * @param cli The client that created the token.
	 * @param topics The topics associated with the token
	 *
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 * @param cb callback listener that will be notified when subscribe has
	 */
	static ptr_t create(iasync_client& cli, const_string_collection_ptr topics,
						void* userContext, iaction_listener& cb) {
		return std::make_shared<token>(cli, topics, userContext, cb);
	}
	/**
	 * Gets the action listener for this token.
	 * @return The action listener for this token.
	 */
	virtual iaction_listener* get_action_callback() const {
		guard g(lock_);
		return listener_;
	}
	/**
	 * Returns the MQTT client that is responsible for processing the
	 * asynchronous action.
	 * @return The client to which this token is connected.
	 */
	virtual iasync_client* get_client() const { return cli_; }
	/**
	 * Returns the ID of the message that is associated with the token.
	 * @return The message ID of the transaction being tracked.
	 */
	virtual int get_message_id() const {
		static_assert(sizeof(tok_) <= sizeof(int), "MQTTAsync_token must fit into int");
		return int(tok_);
	}
	/**
	 * Gets the topic string(s) for the action being tracked by this
	 * token.
	 * @return A const pointer to the collection of topics being tracked by
	 *  	   the token.
	 */
	virtual const_string_collection_ptr get_topics() const {
		return topics_;
	}
	/**
	 * Retrieve the context associated with an action.
	 * @return The context associated with an action.
	 */
	virtual void* get_user_context() const {
		guard g(lock_);
		return userContext_;
	}
	/**
	 * Returns whether or not the action has finished.
	 * @return @em true if the transaction has completed, @em false if not.
	 */
	virtual bool is_complete() const { return complete_; }
	/**
	 * Gets the return code from the action.
	 * This is only valid after the action has completed (i.e. if @ref
	 * is_complete() returns @em true).
	 * @return The return code from the action.
	 */
	virtual int get_return_code() const { return rc_; }
	/**
	 * Register a listener to be notified when an action completes.
	 * @param listener The callback to be notified when actions complete.
	 */
	virtual void set_action_callback(iaction_listener& listener) {
		guard g(lock_);
		listener_ = &listener;
	}
	/**
	 * Store some context associated with an action.
	 * @param userContext optional object used to pass context to the
	 *  				  callback. Use @em nullptr if not required.
	 */
	virtual void set_user_context(void* userContext) {
		guard g(lock_);
		userContext_ = userContext;
	}
	/**
	 * Blocks the current thread until the action this token is associated
	 * with has completed.
	 */
	virtual void wait();
	/**
	 * Non-blocking check to see if the action has completed.
	 * @return @em true if the wait finished successfully, @em false if the
	 *  	   action has not completed yet.
	 */
	virtual bool try_wait() {
		guard g(lock_);
		if (complete_)
			check_rc();
		return complete_;
	}
	/**
	 * Blocks the current thread until the action this token is associated
	 * with has completed.
	 * @param timeout The timeout (in milliseconds)
	 * @return @em true if the wait finished successfully, @em false if a
	 *  	   timeout occurred.
	 */
	virtual bool wait_for(long timeout) {
		return wait_for(std::chrono::milliseconds(timeout));
	}
	/**
	 * Waits a relative amount of time for the action to complete.
	 * @param relTime The amount of time to wait for the event.
	 * @return @em true if the event gets signaled in the specified time,
	 *  	   @em false on a timeout.
	 */
	template <class Rep, class Period>
	bool wait_for(const std::chrono::duration<Rep, Period>& relTime) {
		unique_lock g(lock_);
		if (!cond_.wait_for(g, std::chrono::milliseconds(relTime),
							[this]{return complete_;}))
			return false;
		check_rc();
		return true;
	}
	/**
	 * Waits until an absolute time for the action to complete.
	 * @param absTime The absolute time to wait for the event.
	 * @return @em true if the event gets signaled in the specified time,
	 *  	   @em false on a timeout.
	 */
	template <class Clock, class Duration>
	bool wait_until( const std::chrono::time_point<Clock, Duration>& absTime) {
		unique_lock g(lock_);
		if (!cond_.wait_until(g, absTime, [this]{return complete_;}))
			return false;
		check_rc();
		return true;
	}
};

/** Smart/shared pointer to a token object */
using token_ptr = token::ptr_t;

/** Smart/shared pointer to a const token object */
using const_token_ptr = token::const_ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_token_h

