/*******************************************************************************
 * Copyright (c) 2013-2016 Frank Pagliughi <fpagliughi@mindspring.com>
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

// This is a Paho C++ sample application for the synchronous (blocking) API.
// It also demonstrates the use of custom message persistence, in this case
// creating an in-memory store.

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include "mqtt/client.h"
#include "mqtt/ipersistable.h"

const std::string ADDRESS("tcp://localhost:1883");
const std::string CLIENTID("SyncPublisher");
const std::string TOPIC("hello");

const std::string PAYLOAD1("Hello World!");

const char* PAYLOAD2 = "Hi there!";
const char* PAYLOAD3 = "Is anyone listening?";

const int QOS = 1;
const int TIMEOUT = 10000;

/////////////////////////////////////////////////////////////////////////////

// Example of a simple, in-memory persistence class.
class sample_mem_persistence : virtual public mqtt::iclient_persistence
{
	// Whether the store is open
	bool open_;

	// Use an STL map to store shared persistence pointers
	// against string keys.
	std::map<std::string, mqtt::ipersistable_ptr> store_;

public:
	sample_mem_persistence() : open_(false) {}

	// "Open" the store
	void open(const std::string& clientId, const std::string& serverURI) override {
		std::cout << "[Opening persistence store for '" << clientId
			<< "' at '" << serverURI << "']" << std::endl;
		open_ = true;
	}

	// Close the persistent store that was previously opened.
	void close() override {
		std::cout << "[Closing persistence store.]" << std::endl;
		open_ = false;
	}

	// Clears persistence, so that it no longer contains any persisted data.
	void clear() override {
		std::cout << "[Clearing persistence store.]" << std::endl;
		store_.clear();
	}

	// Returns whether or not data is persisted using the specified key.
	bool contains_key(const std::string &key) override {
		return store_.find(key) != store_.end();
	}

	// Gets the specified data out of the persistent store.
	mqtt::ipersistable_ptr get(const std::string& key) const override {
		std::cout << "[Searching persistence for key '"
			<< key << "']" << std::endl;
		auto p = store_.find(key);
		if (p == store_.end())
			throw mqtt::persistence_exception();
		std::cout << "[Found persistence data for key '"
			<< key << "']" << std::endl;

		return p->second;
	}
	/**
	 * Returns the keys in this persistent data store.
	 */
	std::vector<std::string> keys() const override {
		std::vector<std::string> ks;
		for (const auto& k : store_)
			ks.push_back(k.first);
		return ks;
	}

	// Puts the specified data into the persistent store.
	void put(const std::string& key, mqtt::ipersistable_ptr persistable) override {
		std::cout << "[Persisting data with key '"
			<< key << "']" << std::endl;

		store_[key] = persistable;
	}

	// Remove the data for the specified key.
	void remove(const std::string &key) override {
		std::cout << "[Persistence removing key '" << key << "']" << std::endl;
		auto p = store_.find(key);
		if (p == store_.end())
			throw mqtt::persistence_exception();
		store_.erase(p);
		std::cout << "[Persistence key removed '" << key << "']" << std::endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

class callback : public virtual mqtt::callback
{
public:
	void connection_lost(const std::string& cause) override {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	// We're not subscrived to anything, so this should never be called.
	void message_arrived(const std::string& topic,
								 mqtt::const_message_ptr msg) override {}

	void delivery_complete(mqtt::idelivery_token_ptr tok) override {
		std::cout << "\n\t[Delivery complete for token: "
			<< (tok ? tok->get_message_id() : -1) << "]" << std::endl;
	}
};

// --------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	std::cout << "Initialzing..." << std::endl;
	sample_mem_persistence persist;
	mqtt::client client(ADDRESS, CLIENTID, &persist);

	callback cb;
	client.set_callback(cb);

	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(20);
	connOpts.set_clean_session(true);
	std::cout << "...OK" << std::endl;

	try {
		std::cout << "\nConnecting..." << std::endl;
		client.connect(connOpts);
		std::cout << "...OK" << std::endl;

		// First use a message pointer.

		std::cout << "\nSending message..." << std::endl;
		auto pubmsg = mqtt::make_message(PAYLOAD1);
		pubmsg->set_qos(QOS);
		client.publish(TOPIC, pubmsg);
		std::cout << "...OK" << std::endl;

		// Now try with itemized publish.

		std::cout << "\nSending next message..." << std::endl;
		client.publish(TOPIC, PAYLOAD2, strlen(PAYLOAD2)+1, 0, false);
		std::cout << "...OK" << std::endl;

		// Now try with a listener, no token, and non-heap message

		std::cout << "\nSending final message..." << std::endl;
		client.publish(TOPIC, mqtt::message(PAYLOAD3, QOS, false));
		std::cout << "OK" << std::endl;

		// Disconnect
		std::cout << "\nDisconnecting..." << std::endl;
		client.disconnect();
		std::cout << "...OK" << std::endl;
	}
	catch (const mqtt::persistence_exception& exc) {
		std::cerr << "Persistence Error: " << exc.what() << " ["
			<< exc.get_reason_code() << "]" << std::endl;
		return 1;
	}
	catch (const mqtt::exception& exc) {
		std::cerr << "Error: " << exc.what() << " ["
			<< exc.get_reason_code() << "]" << std::endl;
		return 1;
	}

	std::cout << "\nExiting" << std::endl;
 	return 0;
}

