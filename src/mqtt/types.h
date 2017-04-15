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
#include <chrono>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/** A 'byte' is an 8-bit, unsigned int */
using byte = uint8_t;

/** A collection of bytes  */
using byte_buffer = std::basic_string<byte>;

/**
 * Convert (cast) a byte_buffer to a string.
 * This creates a string that's a binary equivalent to the buffer.
 * @param buf A binary byte_buffer
 * @return A string that's a binary copy of the buffer.
 */
inline std::string to_string(const byte_buffer& buf) {
	return std::string(reinterpret_cast<const char*>(buf.data()), buf.size());
}

/**
 * Convert (cast) a string to a byte buffer.
 * This creates a byte_buffer which is a binary equivalent to the string.
 * @param str A string
 * @return A byte_buffer that's a binary copy of the string.
 */
inline byte_buffer to_buffer(const std::string& str) {
	return byte_buffer(reinterpret_cast<const byte*>(str.data()), str.size());
}

/////////////////////////////////////////////////////////////////////////////

template <class Rep, class Period>
std::chrono::seconds to_seconds(const std::chrono::duration<Rep, Period>& dur) {
	return std::chrono::duration_cast<std::chrono::seconds>(dur);
}

template <class Rep, class Period>
std::chrono::milliseconds to_milliseconds(const std::chrono::duration<Rep, Period>& dur) {
	return std::chrono::duration_cast<std::chrono::milliseconds>(dur);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_types_h

