/////////////////////////////////////////////////////////////////////////////
/// @file iaction_listener.h 
/// Declaration of MQTT iaction_listener class 
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

#ifndef __mqtt_iaction_listener_h
#define __mqtt_iaction_listener_h

extern "C" {
	#include "MQTTAsync.h"
}

#include <string>
#include <vector>
#include <memory>

namespace mqtt {
	
class itoken;
 	
/////////////////////////////////////////////////////////////////////////////  

/**
 * Provides a mechanism for tracking the completion of an asynchronous 
 * action. 
 *  
 * A listener is registered on a token and that token is associated with
 * an action like connect or publish. When used with tokens on the
 * async_client the listener will be called back on the MQTT client's
 * thread. The listener will be informed if the action succeeds or fails. It
 * is important that the listener returns control quickly otherwise the
 * operation of the MQTT client will be stalled.
 */
class iaction_listener
{
public:
	/**
	 * Shared pointer to this class.
	 */
	typedef std::shared_ptr<iaction_listener> ptr_t;
	/**
	 * Virtual base destructor.
	 */
	virtual ~iaction_listener() {}
	/**
	 * This method is invoked when an action fails. 
	 * @param asyncActionToken 
	 * @param exc 
	 */
	virtual void on_failure(const itoken& asyncActionToken /*, java.lang.Throwable exc*/) =0;
	/**
	 * This method is invoked when an action has completed successfully. 
	 * @param asyncActionToken 
	 */
	virtual void on_success(const itoken& asyncActionToken) =0;
};

typedef iaction_listener::ptr_t iaction_listener_ptr;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_iaction_listener_h

