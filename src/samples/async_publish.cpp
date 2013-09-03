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
#include <thread>	// For sleep
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"

const std::string ADDRESS("tcp://localhost:1883");
const std::string CLIENTID("AsyncPublisher");
const std::string TOPIC("hello");

const char* PAYLOAD1 = "Hello World!";
const char* PAYLOAD2 = "Hi there!";
const char* PAYLOAD3 = "Is anyone listening?";
const char* PAYLOAD4 = "Someone is always listening.";

const int  QOS = 1;
const long TIMEOUT = 10000L;

inline void sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

/////////////////////////////////////////////////////////////////////////////

/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback
{
public:
	virtual void connection_lost(const std::string& cause) {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	// We're not subscribed to anything, so this should never be called.
	virtual void message_arrived(const std::string& topic, mqtt::message_ptr msg) {}

	virtual void delivery_complete(mqtt::idelivery_token_ptr tok) {
		std::cout << "Delivery complete for token: " 
			<< (tok ? tok->get_message_id() : -1) << std::endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A base action listener.
 */
class action_listener : public virtual mqtt::iaction_listener
{
protected:
	virtual void on_failure(const mqtt::itoken& tok) {
		std::cout << "\n\tListener: Failure on token: " 
			<< tok.get_message_id() << std::endl;
	}

	virtual void on_success(const mqtt::itoken& tok) {
		std::cout << "\n\tListener: Success on token: " 
			<< tok.get_message_id() << std::endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A derived action listener for publish events.
 */
class delivery_action_listener : public action_listener
{
	bool done_;

	virtual void on_failure(const mqtt::itoken& tok) {
		action_listener::on_failure(tok);
		done_ = true;
	}

	virtual void on_success(const mqtt::itoken& tok) {
		action_listener::on_success(tok);
		done_ = true;
	}

public:
	delivery_action_listener() : done_(false) {}
	bool is_done() const { return done_; }
};

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	mqtt::async_client client(ADDRESS, CLIENTID);
	
	callback cb;
	client.set_callback(cb);

	try {
		mqtt::itoken_ptr conntok = client.connect();
		std::cout << "Waiting for the connection..." << std::flush;
		conntok->wait_for_completion();
		std::cout << "OK" << std::endl;

		// First use a message pointer.

		std::cout << "Sending message..." << std::flush;
		mqtt::message_ptr pubmsg = std::make_shared<mqtt::message>(PAYLOAD1);
		pubmsg->set_qos(QOS);
		client.publish(TOPIC, pubmsg)->wait_for_completion(TIMEOUT);
		std::cout << "OK" << std::endl;

		// Now try with itemized publish.

		std::cout << "Sending next message..." << std::flush;
		mqtt::idelivery_token_ptr pubtok;
		pubtok = client.publish(TOPIC, PAYLOAD2, std::strlen(PAYLOAD2), QOS, false);
		pubtok->wait_for_completion(TIMEOUT);
		std::cout << "OK" << std::endl;

		// Now try with a listener

		std::cout << "Sending next message..." << std::flush;
		action_listener listener;
		pubmsg = std::make_shared<mqtt::message>(PAYLOAD3);
		pubtok = client.publish(TOPIC, pubmsg, nullptr, listener);
		pubtok->wait_for_completion();
		std::cout << "OK" << std::endl;

		// Finally try with a listener, but no token

		std::cout << "Sending final message..." << std::flush;
		delivery_action_listener deliveryListener;
		pubmsg = std::make_shared<mqtt::message>(PAYLOAD4);
		client.publish(TOPIC, pubmsg, nullptr, deliveryListener);

		while (!deliveryListener.is_done()) {
			sleep(100);
		}
		std::cout << "OK" << std::endl;

		// Double check that there are no pending tokens

		std::vector<mqtt::idelivery_token_ptr> toks = client.get_pending_delivery_tokens();
		if (!toks.empty())
			std::cout << "Error: There are pending delivery tokens!" << std::endl;

		// Disconnect
		std::cout << "Disconnecting..." << std::flush;
		conntok = client.disconnect();
		conntok->wait_for_completion();
		std::cout << "OK" << std::endl;
	}
	catch (const mqtt::exception& exc) {
		std::cerr << "Error: " << exc.what() << std::endl;
		return 1;
	}

 	return 0;
}

