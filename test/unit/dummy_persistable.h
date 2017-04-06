// dummy_persistable.h
// Dummy implementation of mqtt::ipersistable for Unit Test.

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
 *    Guilherme M. Ferreira - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_dummy_persistable_h
#define __mqtt_dummy_persistable_h

#include <cstddef>
#include <cstring>
#include <vector>

#include "mqtt/ipersistable.h"

namespace mqtt {
namespace test {

/////////////////////////////////////////////////////////////////////////////

class dummy_persistable : public mqtt::ipersistable
{
public:
	static const char* HEADER;
	static const char* PAYLOAD;

	const uint8_t* get_header_bytes() const override { return reinterpret_cast<const uint8_t*>(HEADER); }
	size_t get_header_length() const override { return strlen(HEADER); }
	size_t get_header_offset() const override { return 0; }

	const uint8_t* get_payload_bytes() const override { return reinterpret_cast<const uint8_t*>(PAYLOAD); }
	size_t get_payload_length() const override { return strlen(PAYLOAD); }
	size_t get_payload_offset() const override { return 0; }

	std::vector<uint8_t> get_header_byte_arr() const override {
		return std::vector<uint8_t>(HEADER, HEADER + get_header_length());
	}
	std::vector<uint8_t> get_payload_byte_arr() const override {
		return std::vector<uint8_t>(PAYLOAD, PAYLOAD + get_payload_length());
	}
};

const char* dummy_persistable::HEADER { "HEADER" };
const char* dummy_persistable::PAYLOAD { "PAYLOAD" };

/////////////////////////////////////////////////////////////////////////////
// end namespace test
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_dummy_persistable_h
