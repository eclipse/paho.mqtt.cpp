/////////////////////////////////////////////////////////////////////////////
/// @file subscribe_options.h
/// Declaration of MQTT subscribe_options class
/// @date Aug 1, 2019 @
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2019 Frank Pagliughi <fpagliughi@mindspring.com>
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

#ifndef __mqtt_subscribe_options_h
#define __mqtt_subscribe_options_h

#include "MQTTAsync.h"
#include "MQTTSubscribeOpts.h"
#include "mqtt/types.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * The MQTT v5 subscribe options.
 */
class subscribe_options
{
	/** The underlying C structure */
	MQTTSubscribe_options opts_;

	friend class async_client;
	friend class response_options;
	friend class subscribe_options_test;

public:
	/** Smart/shared pointer to an object of this class. */
	using ptr_t = std::shared_ptr<subscribe_options>;
	/** Smart/shared pointer to a const object of this class. */
	using const_ptr_t = std::shared_ptr<const subscribe_options>;

	/** Don't receive our own publications */
	static constexpr bool SUBSCRIBE_NO_LOCAL = true;
	/** Receive our own publications */
	static constexpr bool SUBSCRIBE_LOCAL = false;

	/**
	 * Retain flag is only set on publications sent by a broker if in
	 * response to a subscribe request
	 */
	static constexpr byte NO_RETAIN_AS_PUBLISHED = 0;
	/** Keep the retain flag as on the original publish message */
	static constexpr byte RETAIN_AS_PUBLISHED = 1;

	/** Send retained messages at the time of the subscribe */
	static constexpr byte SEND_RETAINED_ON_SUBSCRIBE = 0;
	/** Send retained messages on subscribe only if subscription is new */
	static constexpr byte SEND_RETAINED_ON_NEW = 1;
	/** Do not send retained messages at all */
	static constexpr byte DONT_SEND_RETAINED = 2;

	/**
	 * Create default subscription options.
	 * These are the default options corresponding to the original MQTT (v3)
	 * behaviours.
	 */
	subscribe_options()
			: opts_(MQTTSubscribe_options_initializer) {}

	explicit subscribe_options(bool noLocal, byte retainAsPublished=0,
							   byte retainHandling=0)
			: opts_(MQTTSubscribe_options_initializer) {
		opts_.noLocal = to_int(noLocal);
		opts_.retainAsPublished = (unsigned char) retainAsPublished ;
		opts_.retainHandling = (unsigned char) retainHandling;
	}

};


/** Smart/shared pointer to a subscribe options object. */
using subscribe_options_ptr = subscribe_options::ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_subscribe_options_h

