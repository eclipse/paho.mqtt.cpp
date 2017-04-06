// dummy_client_persistence.h
// Dummy implementation of mqtt::iclient_persistence for Unit Test.

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

#ifndef __mqtt_dummy_client_persistence_h
#define __mqtt_dummy_client_persistence_h

#include <string>
#include <stdexcept>

#include "mqtt/iclient_persistence.h"

#include "dummy_persistable.h"


namespace mqtt {
namespace test {

/////////////////////////////////////////////////////////////////////////////

class dummy_client_persistence : public mqtt::iclient_persistence
{
public:
	static const char* CLIENT_ID;
	static const char* SERVER_URI;
	static const char* KEY_VALID;
	static const char* KEY_INVALID;

	void open(const std::string& clientId, const std::string& serverURI) override {
		if (clientId != CLIENT_ID) {
			throw std::invalid_argument{clientId};
		}
		if (serverURI != SERVER_URI) {
			throw std::invalid_argument{serverURI};
		}
	}

	void close() override {
	}

	void clear() override {
	}

	// Returns whether or not data is persisted using the specified key.
	bool contains_key(const std::string &key) override {
		return key == KEY_VALID;
	}

	// Gets the specified data out of the persistent store.
	mqtt::ipersistable_ptr get(const std::string& key) const override {
		if (key != KEY_VALID) {
			throw std::invalid_argument{key};
		}
		return mqtt::ipersistable_ptr{new dummy_persistable};
	}
	/**
	 * Returns the keys in this persistent data store.
	 */
	std::vector<std::string> keys() const override {
		return std::vector<std::string>{ KEY_VALID };
	}

	// Puts the specified data into the persistent store.
	void put(const std::string& key, mqtt::ipersistable_ptr persistable) override {
		if (key != KEY_VALID) {
			throw std::invalid_argument{key};
		}
		const std::string PAYLOAD { dummy_persistable::PAYLOAD };
		const std::string payload ( reinterpret_cast<const char*>(persistable->get_payload_bytes()), persistable->get_payload_length() );
		// NOTE: compare sub string, not the whole string. Because the
		// the "PAYLOAD" string might repeat N times using the method
		// mqtt::iclient_persistence::persistence_put()
		if (!std::equal(PAYLOAD.begin(), PAYLOAD.end(), payload.begin())) {
			throw std::invalid_argument{key};
		}
	}

	void remove(const std::string &key) override {
		if (key != KEY_VALID) {
			throw std::invalid_argument{key};
		}
	}
};

const char* dummy_client_persistence::CLIENT_ID { "CLIENT_ID" };
const char* dummy_client_persistence::SERVER_URI { "SERVER_URI" };
const char* dummy_client_persistence::KEY_VALID { "KEY_VALID" };
const char* dummy_client_persistence::KEY_INVALID { "KEY_INVALID" };

/////////////////////////////////////////////////////////////////////////////
// end namespace test
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_dummy_client_persistence_h
