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

class sample_mem_persistence : virtual public mqtt::iclient_persistence
{
	bool open_;
	std::map<std::string, mqtt::ipersistable_ptr> store_;

public:
	sample_mem_persistence() : open_(false) {}

	// "Open" the store
	virtual void open(const std::string& clientId, const std::string& serverURI) {
		std::cout << "[Opening persistence for '" << clientId 
			<< "' at '" << serverURI << "']" << std::endl;
		open_ = true;
	}

	// Close the persistent store that was previously opened.
	virtual void close() {
		std::cout << "[Closing persistence store.]" << std::endl;
		open_ = false; 
	}

	// Clears persistence, so that it no longer contains any persisted data.
	virtual void clear() {
		std::cout << "[Clearing persistence store.]" << std::endl;
		store_.clear(); 
	}

	// Returns whether or not data is persisted using the specified key.
	virtual bool contains_key(const std::string &key) {
		return store_.find(key) != store_.end();
	}

	// Gets the specified data out of the persistent store.
	virtual mqtt::ipersistable_ptr get(const std::string& key) const {
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
	virtual std::vector<std::string> keys() const {
		std::vector<std::string> ks;
		for (const auto& k : store_)
			ks.push_back(k.first);
		return ks;
	}

	// Puts the specified data into the persistent store.
	virtual void put(const std::string& key, mqtt::ipersistable_ptr persistable) {
		std::cout << "[Persisting data with key '"
			<< key << "']" << std::endl;

		store_[key] = persistable;
	}

	// Remove the data for the specified key.
	virtual void remove(const std::string &key) {
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
	virtual void connection_lost(const std::string& cause) {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	// We're not subscrived to anything, so this should never be called.
	virtual void message_arrived(const std::string& topic, mqtt::message_ptr msg) {
	}

	virtual void delivery_complete(mqtt::idelivery_token_ptr tok) {
		std::cout << "\n\t[Delivery complete for token: " 
			<< (tok ? tok->get_message_id() : -1) << "]" << std::endl;
	}
};

// --------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	sample_mem_persistence persist;
	mqtt::client client(ADDRESS, CLIENTID, &persist);
	
	callback cb;
	client.set_callback(cb);

	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(20);
	connOpts.set_clean_session(true);

	try {
		std::cout << "Connecting..." << std::flush;
		client.connect(connOpts);
		std::cout << "OK" << std::endl;

		// First use a message pointer.

		std::cout << "Sending message..." << std::flush;
		mqtt::message_ptr pubmsg = std::make_shared<mqtt::message>(PAYLOAD1);
		pubmsg->set_qos(QOS);
		client.publish(TOPIC, pubmsg);
		std::cout << "OK" << std::endl;

		// Now try with itemized publish.

		std::cout << "Sending next message..." << std::flush;
		client.publish(TOPIC, PAYLOAD2, strlen(PAYLOAD2)+1, 0, false);
		std::cout << "OK" << std::endl;

		// Now try with a listener, but no token

		std::cout << "Sending final message..." << std::flush;
		pubmsg = std::make_shared<mqtt::message>(PAYLOAD3);
		pubmsg->set_qos(QOS);
		client.publish(TOPIC, pubmsg);
		std::cout << "OK" << std::endl;

		// Disconnect
		std::cout << "Disconnecting..." << std::flush;
		client.disconnect();
		std::cout << "OK" << std::endl;
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

 	return 0;
}

