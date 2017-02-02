/////////////////////////////////////////////////////////////////////////////
/// @file delivery_token.h
/// Declaration of MQTT idelivery_token abstract class
/// @date Feb 2, 2017
/// @author Guilherme Maciel Ferreira
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2017 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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
 *    Guilherme M. Ferreira - split from delivery_token header
 *******************************************************************************/

#ifndef __mqtt_idelivery_token_h
#define __mqtt_idelivery_token_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/token.h"
#include "mqtt/message.h"
#include <memory>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Provides a mechanism for tracking the delivery of a message.
 */
class idelivery_token : public virtual itoken
{
public:
	/** Smart/shared pointer to an object of this class */
	using ptr_t = std::shared_ptr<idelivery_token>;
	/** Smart/shared pointer to a const object of this class */
	using const_ptr_t = std::shared_ptr<const idelivery_token>;

	/**
	 * Gets the message associated with this token.
	 * @return The message associated with this token.
	 */
	virtual const_message_ptr get_message() const =0;
};

/** Smart/shared pointer to a delivery token */
using idelivery_token_ptr = idelivery_token::ptr_t;

/** Smart/shared pointer to a const delivery token */
using const_idelivery_token_ptr = idelivery_token::const_ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_idelivery_token_h

