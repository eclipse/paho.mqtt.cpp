/////////////////////////////////////////////////////////////////////////////
/// @file itoken.h
/// Declaration of MQTT token abstract class
/// @date Oct 7, 2016
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
 *    Guilherme M. Ferreira - split from token.h header
 *******************************************************************************/

#ifndef __mqtt_itoken_h
#define __mqtt_itoken_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/iaction_listener.h"
#include "mqtt/exception.h"
#include <string>
#include <vector>
#include <memory>

namespace mqtt {

class iasync_client;

/////////////////////////////////////////////////////////////////////////////

/**
 * Provides a mechanism for tracking the completion of an asynchronous task.
 */
class itoken
{
public:
	/** Shared pointer to a token */
	using ptr_t = std::shared_ptr<itoken>;
	/**
	 * Virtual base destructor.
	 */
	virtual ~itoken() {}
	/**
	 * Return the async listener for this token.
	 * @return iaction_listener
	 */
	virtual iaction_listener* get_action_callback() const =0;
	/**
	 * Returns the MQTT client that is responsible for processing the
	 * asynchronous action.
	 * @return iasync_client
	 */
	virtual iasync_client* get_client() const =0;
	/**
	 * Returns the message ID of the message that is associated with the
	 * token.
	 * @return int
	 */
	virtual int get_message_id() const =0;
	/**
	 * Returns the topic string(s) for the action being tracked by this
	 * token.
	 * @return std::vector<std::string>
	 */
	virtual const std::vector<std::string>& get_topics() const =0;
	/**
	 * Retrieve the context associated with an action.
	 * @return void*
	 */
	virtual void* get_user_context() const =0;
	/**
	 * Returns whether or not the action has finished.
	 * @return bool
	 */
	virtual bool is_complete() const =0;
	/**
	 * Register a listener to be notified when an action completes.
	 * @param listener
	 */
	virtual void set_action_callback(iaction_listener& listener) =0;
	/**
	 * Store some context associated with an action.
	 * @param userContext
	 */
	virtual void set_user_context(void* userContext) =0;
	/**
	 * Blocks the current thread until the action this token is associated
	 * with has completed.
	 */
	virtual void wait_for_completion() =0;
	/**
	 * Blocks the current thread until the action this token is associated
	 * with has completed.
	 * @param timeout The timeout (in milliseconds)
	 */
	virtual void wait_for_completion(long timeout) =0;
};

using itoken_ptr = itoken::ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_itoken_h
