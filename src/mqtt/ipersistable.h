/////////////////////////////////////////////////////////////////////////////
/// @file ipersistable.h 
/// Declaration of MQTT ipersistable interface.
/// @date May 24, 2013 
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

#ifndef __mqtt_ipersistable_h
#define __mqtt_ipersistable_h

extern "C" {
	#include "MQTTAsync.h"
}

#include <string>
#include <memory>
#include <vector>
#include <stdexcept> 

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////  

/**
 * Represents an object used to pass data to be persisted across the
 * MqttClientPersistence interface.
 *
 * When data is passed across the interface the header and payload are
 * separated, so that unnecessary message copies may be avoided. For
 * example, if a 10 MB payload was published it would be inefficient to
 * create a byte array a few bytes larger than 10 MB and copy the MQTT
 * message header and payload into a contiguous byte array.
 *
 * When the request to persist data is made a separate byte array and offset
 * is passed for the header and payload. Only the data between offset and
 * length need be persisted. So for example, a message to be persisted
 * consists of a header byte array starting at offset 1 and length 4, plus a
 * payload byte array starting at offset 30 and length 40000. There are
 * three ways in which the persistence implementation may return data to the
 * client on recovery:
 *
 * @li
 * It could return the data as it was passed in originally, with the same
 * byte arrays and offsets.
 *
 * @li
 * It could safely just persist and return the bytes from the offset for the
 * specified length. For example, return a header byte array with offset 0
 * and length 4, plus a payload byte array with offset 0 and length 40000
 *
 * @li
 * It could return the header and payload as a contiguous byte array with
 * the header bytes preceeding the payload. The contiguous byte array should
 * be set as the header byte array, with the payload byte array being null.
 * For example, return a single byte array with offset 0 and length 40004.
 * This is useful when recovering from a file where the header and payload
 * could be written as a contiguous stream of bytes.
 */

class ipersistable
{
public:
	/**
	 * Smart/shared pointer to this class.
	 */
	typedef std::shared_ptr<ipersistable> ptr_t;
	/**
	 * Virtual destructor
	 */
	virtual ~ipersistable() {}
	/**
	 * Returns the header bytes in an array. 
	 * @return std::vector<uint8_t> 
	 */
	virtual const uint8_t* get_header_bytes() const =0;
	/**
	 * Returns the header bytes in an array. 
	 * @return std::vector<uint8_t> 
	 */
	virtual std::vector<uint8_t> get_header_byte_arr() const =0;
	/**
	 * Returns the length of the header. 
	 * @return int 
	 */
	virtual size_t get_header_length() const =0;
	/**
	 * Returns the offset of the header within the byte array returned by 
	 * get_header_bytes(). 
	 * @return int 
	 */
	virtual size_t get_header_offset() const =0;
	/**
	 * Returns the payload bytes in an array.
	 * @return std::vector<uint8_t> 
	 */
	virtual const uint8_t* get_payload_bytes() const =0;
	/**
	 * Returns the payload bytes in an array.
	 * @return std::vector<uint8_t> 
	 */
	virtual std::vector<uint8_t> get_payload_byte_arr() const =0;
	/**
	 * Returns the length of the payload.
	 * @return int 
	 */
	virtual size_t get_payload_length() const =0;
	/**
	 * Returns the offset of the payload within the byte array returned by 
	 * get_payload_bytes(). 
	 * 
	 * @return int 
	 */
	virtual size_t get_payload_offset() const =0;
};

typedef ipersistable::ptr_t ipersistable_ptr;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_ipersistable_h


