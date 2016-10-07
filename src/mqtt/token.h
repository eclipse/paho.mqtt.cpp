/////////////////////////////////////////////////////////////////////////////
/// @file token.h
/// Declaration of MQTT token class
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

#ifndef __mqtt_token_h
#define __mqtt_token_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/itoken.h"
#include "mqtt/iaction_listener.h"
#include "mqtt/exception.h"
#include <string>
#include <vector>
#include <memory>
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
class token : public virtual itoken
{
	/** Lock guard type for this class. */
	using guard = std::unique_lock<std::mutex>;

	/** Object monitor mutex. */
	mutable std::mutex lock_;
	/** Condition variable signals when the action completes */
	std::condition_variable cond_;
	/** The underlying C token. Note that this is just an integer */
	MQTTAsync_token tok_;
	/** The topic string(s) for the action being tracked by this token */
	std::vector<std::string> topics_;
	/** The MQTT client that is processing this action */
	iasync_client* cli_;
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

	/** Client and token-related options have special access */
	friend class async_client;
	friend class response_options;
	friend class delivery_response_options;
	friend class connect_options;
	friend class disconnect_options;

	void set_topics(const std::string& top) {
		topics_.clear();
		topics_.push_back(top);
	}
	void set_topics(const std::vector<std::string>& top) {
		topics_ = top;
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
	 * Internal handler for the success callback.
	 * @param rsp The success response.
	 */
	void on_success(MQTTAsync_successData* rsp);
	/**
	 * Internal handler for the failure callback.
	 * @param rsp The failure response.
	 */
	void on_failure(MQTTAsync_failureData* rsp);

public:
	/** Smart pointer to an object of this class */
	using ptr_t = std::shared_ptr<token>;
	/** Smart pointer to an object of this class */
	using const_ptr_t = std::shared_ptr<const token>;

	/**
	 * Constructs a token object.
	 * @param cli
	 */
	token(iasync_client& cli);
	/**
	 * Constructs a token object.
	 * @param cli
	 * @param tok
	 */
	token(iasync_client& cli, MQTTAsync_token tok);
	/**
	 * Constructs a token object.
	 * @param cli
	 * @param topic
	 */
	token(iasync_client& cli, const std::string& topic);
	/**
	 * Constructs a token object.
	 * @param cli
	 * @param topics
	 */
	token(iasync_client& cli, const std::vector<std::string>& topics);
	/**
	 * Return the async listener for this token.
	 * @return iaction_listener
	 */
	iaction_listener* get_action_callback() const override {
		// TODO: Guard?
		return listener_;
	}
	/**
	 * Returns the MQTT client that is responsible for processing the
	 * asynchronous action.
	 * @return iasync_client
	 */
	iasync_client* get_client() const override {
		return cli_;
	}
	/**
	 * Returns the message ID of the message that is associated with the
	 * token.
	 * @return int
	 */
	int get_message_id() const override {
		static_assert(sizeof(tok_) == sizeof(int), "MQTTAsync_token must fit into int");
		return static_cast<int>(tok_);
	}
	/**
	 * Returns the topic string(s) for the action being tracked by this
	 * token.
	 */
	const std::vector<std::string>& get_topics() const override {
		return topics_;
	}
	/**
	 * Retrieve the context associated with an action.
	 */
	void* get_user_context() const override {
		guard g(lock_);
		return userContext_;
	}
	/**
	 * Returns whether or not the action has finished.
	 * @return bool
	 */
	bool is_complete() const override { return complete_; }
	/**
	 * Register a listener to be notified when an action completes.
	 * @param listener
	 */
	void set_action_callback(iaction_listener& listener) override {
		guard g(lock_);
		listener_ = &listener;
	}
	/**
	 * Store some context associated with an action.
	 * @param userContext
	 */
	void set_user_context(void* userContext) override {
		guard g(lock_);
		userContext_ = userContext;
	}
	/**
	 * Blocks the current thread until the action this token is associated
	 * with has completed.
	 */
	void wait_for_completion() override;
	/**
	 * Blocks the current thread until the action this token is associated
	 * with has completed.
	 * @param timeout The timeout (in milliseconds)
	 */
	void wait_for_completion(long timeout) override;
	/**
	 * Waits a relative amount of time for the action to complete.
	 * @param relTime The amount of time to wait for the event.
	 * @return @em true if the event gets signaled in the specified time,
	 *  	   @em false on a timeout.
	 */
	template <class Rep, class Period>
	bool wait_for_completion(const std::chrono::duration<Rep, Period>& relTime) {
		wait_for_completion(static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(relTime).count()));
		return rc_ == 0;
	}
	/**
	 * Waits until an absolute time for the action to complete.
	 * @param absTime The absolute time to wait for the event.
	 * @return @em true if the event gets signaled in the specified time,
	 *  	   @em false on a timeout.
	 */
	template <class Clock, class Duration>
	bool wait_until_completion(const std::chrono::time_point<Clock, Duration>& absTime) {
		guard g(lock_);
		if (!cond_.wait_until(g, absTime, [this]{return complete_;}))
			return false;
		if (rc_ != MQTTASYNC_SUCCESS)
			throw exception(rc_);
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

