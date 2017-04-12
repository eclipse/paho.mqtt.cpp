/////////////////////////////////////////////////////////////////////////////
/// @file types.h
/// Basic types for the Paho MQTT C++ library.
/// @date May 17, 2015 @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2015-2017 Frank Pagliughi <fpagliughi@mindspring.com>
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

#ifndef __mqtt_types_h
#define __mqtt_types_h

#include <string>

namespace mqtt {

///////////////////////////////////////////////////////////////////////////// 

/** A 'byte' is an 8-bit, unsigned int */
using byte = uint8_t;

/** A collection of bytes  */
using byte_buffer = std::basic_string<byte>;

///////////////////////////////////////////////////////////////////////////// 
// end namespace mqtt
}

#endif		// __mqtt_types_h

