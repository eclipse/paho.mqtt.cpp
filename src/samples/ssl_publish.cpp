// ssl_publish.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// It's an example of how to connect to an MQTT broker securely, and then
// send messages as an MQTT publisher using the C++ asynchronous client
// interface.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker securely
//  - Setting SSL/TLS options
//  - Last will and testament
//  - Publishing messages
//  - Using asynchronous tokens
//  - Implementing callbacks and action listeners
//
// We can test this using mosquitto configured with certificates in the
// Paho C library. The C library has an SSL/TSL test suite, and we can use
// that to test:
//     mosquitto -c paho.mqtt.c/test/tls-testing/mosquitto.conf
//
// Then use the file "test-root-ca.crt" from that directory
// (paho.mqtt.c/test/tls-testing) for the trust store for this program.
//

/*******************************************************************************
 * Copyright (c) 2013-2017 Frank Pagliughi <fpagliughi@mindspring.com>
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
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"

const std::string DFLT_SERVER_ADDRESS	{ "ssl://localhost:18885" };
const std::string DFLT_CLIENT_ID		{ "ssl_publish_cpp" };

const std::string TOPIC { "hello" };

const char* PAYLOAD1 = "Hello World!";
const char* PAYLOAD2 = "Hi there!";

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
	void connection_lost(const std::string& cause) override {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;
	}

	void delivery_complete(mqtt::delivery_token_ptr tok) override {
		std::cout << "\tDelivery complete for token: "
			<< (tok ? tok->get_message_id() : -1) << std::endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

using namespace std;

int main(int argc, char* argv[])
{
	string	address  = (argc > 1) ? string(argv[1]) : DFLT_SERVER_ADDRESS,
			clientID = (argc > 2) ? string(argv[2]) : DFLT_CLIENT_ID;

	cout << "Initializing for server '" << address << "'..." << endl;
	mqtt::async_client client(address, clientID);

	callback cb;
	client.set_callback(cb);

	mqtt::connect_options connopts("testuser", "testpassword");

	mqtt::ssl_options sslopts;
	sslopts.set_trust_store("test-root-ca.crt");

	mqtt::message willmsg(TOPIC, LWT_PAYLOAD, 1, true);
	mqtt::will_options will(willmsg);

	connopts.set_will(will);
	connopts.set_ssl(sslopts);

	cout << "  ...OK" << endl;

	try {
		cout << "\nConnecting..." << endl;
		mqtt::token_ptr conntok = client.connect(connopts);
		cout << "Waiting for the connection..." << endl;
		conntok->wait();
		cout << "  ...OK" << endl;

		// First use a message pointer.

		cout << "\nSending message..." << endl;
		mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, PAYLOAD1);
		pubmsg->set_qos(QOS);
		client.publish(pubmsg)->wait_for(TIMEOUT);
		cout << "  ...OK" << endl;

		// Now try with itemized publish.

		cout << "\nSending next message..." << endl;
		mqtt::delivery_token_ptr pubtok;
		pubtok = client.publish(TOPIC, PAYLOAD2, strlen(PAYLOAD2), QOS, false);
		pubtok->wait_for(TIMEOUT);
		cout << "  ...OK" << endl;

		// Disconnect
		cout << "\nDisconnecting..." << endl;
		conntok = client.disconnect();
		conntok->wait();
		cout << "  ...OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}

