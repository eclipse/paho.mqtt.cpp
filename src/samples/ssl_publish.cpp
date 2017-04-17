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

// We can test this using mosquitto configured with certificates in the
// Paho C library. The C library has an SSL/TSL test suite, and we can use
// that to test:
//     mosquitto -c paho.mqtt.c/test/tls-testing/mosquitto.conf
//
// Then use the file "test-root-ca.crt" from that directory
// (paho.mqtt.c/test/tls-testing) for the trust store for this program.
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"

using namespace std;

const string DFLT_ADDRESS {"ssl://localhost:18885"};
const string DFLT_CLIENT_ID {"CppAsyncPublisherSSL"};

const string TOPIC {"hello"};

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

int main(int argc, char* argv[])
{
	string	address  = (argc > 1) ? string(argv[1]) : DFLT_ADDRESS,
			clientID = (argc > 2) ? string(argv[2]) : DFLT_CLIENT_ID;

	cout << "Initializing for server '" << address << "'..." << endl;
	mqtt::async_client client(address, clientID);

	callback cb;
	client.set_callback(cb);

	mqtt::connect_options connopts("testuser", "testpassword");

	mqtt::ssl_options sslopts;
	sslopts.set_trust_store("test-root-ca.crt");

	mqtt::message willmsg(LWT_PAYLOAD, 1, true);
	mqtt::will_options will(TOPIC, willmsg);

	connopts.set_will(will);
	connopts.set_ssl(sslopts);

	cout << "  ...OK" << endl;

	try {
		cout << "\nConnecting..." << endl;
		mqtt::itoken_ptr conntok = client.connect(connopts);
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
		pubtok->wait_for_completion(TIMEOUT);
		cout << "  ...OK" << endl;


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

