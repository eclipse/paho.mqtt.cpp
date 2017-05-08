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

#include "mqtt/iclient_persistence.h"

namespace mqtt {
namespace test {

/////////////////////////////////////////////////////////////////////////////

// Dummy persistence is a working in-memory persistence class.
class dummy_client_persistence : virtual public mqtt::iclient_persistence
{
	// Whether the store is open
	bool open_;

	// Use an STL map to store shared persistence pointers
	// against string keys.
	std::map<std::string, std::string> store_;

public:
	dummy_client_persistence() : open_(false) {}

	// "Open" the store
	void open(const std::string& clientId, const std::string& serverURI) override {
		open_ = true;
	}

	// Close the persistent store that was previously opened.
	void close() override { open_ = false; }

	// Clears persistence, so that it no longer contains any persisted data.
	void clear() override { store_.clear(); }

	// Returns whether or not data is persisted using the specified key.
	bool contains_key(const std::string& key) override {
		return store_.find(key) != store_.end();
	}

	// Returns the keys in this persistent data store.
	// This could be more efficient, but you get the point.
	const mqtt::string_collection& keys() const override {
		static mqtt::string_collection ks;
		ks.clear();
		for (const auto& k : store_)
			ks.push_back(k.first);
		return ks;
	}

	// Puts the specified data into the persistent store.
	void put(const std::string& key, const std::vector<mqtt::string_view>& bufs) override {
		std::string str;
		for (const auto& b : bufs)
			str += b.str();
		store_[key] = std::move(str);
	}

	// Gets the specified data out of the persistent store.
	mqtt::string_view get(const std::string& key) const override {
		auto p = store_.find(key);
		if (p == store_.end())
			throw mqtt::persistence_exception();
		return mqtt::string_view(p->second);
	}

	// Remove the data for the specified key.
	void remove(const std::string& key) override {
		auto p = store_.find(key);
		if (p == store_.end())
			throw mqtt::persistence_exception();
		store_.erase(p);
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace test
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		//  __mqtt_dummy_client_persistence_h
