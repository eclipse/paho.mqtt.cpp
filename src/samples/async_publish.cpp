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

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>	// For sleep
#include <atomic>
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"

using namespace std;

const string DFLT_ADDRESS {"tcp://localhost:1883"};
const string DFLT_CLIENT_ID {"AsyncPublisher"};

const string TOPIC {"hello"};

const char* PAYLOAD1 = "Hello World!";
const char* PAYLOAD2 = "Hi there!";
const char* PAYLOAD3 = "Is anyone listening?";
const char* PAYLOAD4 = "Someone is always listening.";

const char* LWT_PAYLOAD = "Last will and testament.";

const int  QOS = 1;

const auto TIMEOUT = std::chrono::seconds(10);

/////////////////////////////////////////////////////////////////////////////

/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback
{
public:
	void connection_lost(const string& cause) override {
		cout << "\nConnection lost" << endl;
		if (!cause.empty())
			cout << "\tcause: " << cause << endl;
	}

	// We're not subscribed to anything, so this should never be called.
	void message_arrived(const string& topic, mqtt::const_message_ptr msg) override {}

	void delivery_complete(mqtt::idelivery_token_ptr tok) override {
		cout << "\tDelivery complete for token: "
			<< (tok ? tok->get_message_id() : -1) << endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A base action listener.
 */
class action_listener : public virtual mqtt::iaction_listener
{
protected:
	void on_failure(const mqtt::itoken& tok) override {
		cout << "\tListener failure for token: "
			<< tok.get_message_id() << endl;
	}

	void on_success(const mqtt::itoken& tok) override {
		cout << "\tListener success for token: "
			<< tok.get_message_id() << endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A derived action listener for publish events.
 */
class delivery_action_listener : public action_listener
{
	atomic<bool> done_;

	void on_failure(const mqtt::itoken& tok) override {
		action_listener::on_failure(tok);
		done_ = true;
	}

	void on_success(const mqtt::itoken& tok) override {
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
	string	address  = (argc > 1) ? string(argv[1]) : DFLT_ADDRESS,
			clientID = (argc > 1) ? string(argv[2]) : DFLT_CLIENT_ID;

	cout << "Initializing for server '" << address << "'..." << endl;
	mqtt::async_client client(address, clientID);

	callback cb;
	client.set_callback(cb);

	mqtt::connect_options conopts;
	mqtt::message willmsg(LWT_PAYLOAD, 1, true);
	mqtt::will_options will(TOPIC, willmsg);
	conopts.set_will(will);

	cout << "  ...OK" << endl;

	try {
		cout << "\nConnecting..." << endl;
		mqtt::itoken_ptr conntok = client.connect(conopts);
		cout << "Waiting for the connection..." << endl;
		conntok->wait_for_completion();
		cout << "  ...OK" << endl;

		// First use a message pointer.

		cout << "\nSending message..." << endl;
		mqtt::message_ptr pubmsg = mqtt::make_message(PAYLOAD1);
		pubmsg->set_qos(QOS);
		client.publish(TOPIC, pubmsg)->wait_for_completion(TIMEOUT);
		cout << "  ...OK" << endl;

		// Now try with itemized publish.

		cout << "\nSending next message..." << endl;
		mqtt::idelivery_token_ptr pubtok;
		pubtok = client.publish(TOPIC, PAYLOAD2, strlen(PAYLOAD2), QOS, false);
		cout << "  ...with token: " << pubtok->get_message_id() << endl;
		cout << "  ...for message with " << pubtok->get_message()->get_payload().size()
			<< " bytes" << endl;
		pubtok->wait_for_completion(TIMEOUT);
		cout << "  ...OK" << endl;

		// Now try with a listener

		cout << "\nSending next message..." << endl;
		action_listener listener;
		pubmsg = mqtt::make_message(PAYLOAD3);
		pubtok = client.publish(TOPIC, pubmsg, nullptr, listener);
		pubtok->wait_for_completion();
		cout << "  ...OK" << endl;

		// Finally try with a listener, but no token

		cout << "\nSending final message..." << endl;
		delivery_action_listener deliveryListener;
		pubmsg = mqtt::make_message(PAYLOAD4);
		client.publish(TOPIC, pubmsg, nullptr, deliveryListener);

		while (!deliveryListener.is_done()) {
			this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		cout << "OK" << endl;

		// Double check that there are no pending tokens

		vector<mqtt::idelivery_token_ptr> toks = client.get_pending_delivery_tokens();
		if (!toks.empty())
			cout << "Error: There are pending delivery tokens!" << endl;

		// Disconnect
		cout << "\nDisconnecting..." << endl;
		conntok = client.disconnect();
		conntok->wait_for_completion();
		cout << "  ...OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << "Error: " << exc.what() << endl;
		return 1;
	}

 	return 0;
}

