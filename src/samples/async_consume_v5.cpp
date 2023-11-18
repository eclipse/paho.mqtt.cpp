// async_consume_v5.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// This application is an MQTT consumer/subscriber using the C++
// asynchronous client interface, employing the  to receive messages
// and status updates.
//
// The sample demonstrates:
//  - Connecting to an MQTT v5 server/broker.
//  - Subscribing to a topic
//  - Receiving messages through the consuming (queuing) API
//

/*******************************************************************************
 * Copyright (c) 2013-2023 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v20.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/async_client.h"

using namespace std;

const string SERVER_ADDRESS	{ "mqtt://localhost:1883" };
const string CLIENT_ID		{ "PahoCppAsyncConsumeV5" };
const string TOPIC 			{ "hello" };

const int  QOS = 1;

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	// Create a client using MQTT v5
	mqtt::create_options createOpts(MQTTVERSION_5);
	mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID, createOpts);

	auto connOpts = mqtt::connect_options_builder()
		.properties({
			{mqtt::property::SESSION_EXPIRY_INTERVAL, 604800}
	    })
		.clean_session(false)
		.finalize();

	try {
		cli.set_connection_lost_handler([](const std::string&) {
			cout << "*** Connection Lost ***" << endl;
		});

		cli.set_disconnected_handler([](const mqtt::properties&, mqtt::ReasonCode reason) {
			cout << "*** Disconnected. Reason: " << reason << " ***" << endl;
		});

		// Start consumer before connecting to make sure to not miss messages

		cli.start_consuming();

		// Connect to the server

		cout << "Connecting to the MQTT server..." << flush;
		auto tok = cli.connect(connOpts);

		// Getting the connect response will block waiting for the
		// connection to complete.
		auto rsp = tok->get_connect_response();

		// Make sure we were granted a v5 connection.
		if (rsp.get_mqtt_version() < MQTTVERSION_5) {
			cout << "Did not get an MQTT v5 connection." << endl;
			exit(1);
		}

		// If there is no session present, then we need to subscribe, but if
		// there is a session, then the server remembers us and our
		// subscriptions.
		if (!rsp.is_session_present()) {
			cout << "Session not present on broker. Subscribing." << endl;
			cli.subscribe(TOPIC, QOS)->wait();
		}

		cout << "OK" << endl;

		// Consume messages
		// This just exits if the client is disconnected.
		// (See some other examples for auto or manual reconnect)

		cout << "Waiting for messages on topic: '" << TOPIC << "'" << endl;

		while (true) {
			auto msg = cli.consume_message();
			if (!msg) break;
			cout << msg->get_topic() << ": " << msg->to_string() << endl;
		}

		// If we're here, the client was almost certainly disconnected.
		// But we check, just to make sure.

		if (cli.is_connected()) {
			cout << "\nShutting down and disconnecting from the MQTT server..." << flush;
			cli.stop_consuming();
			cli.disconnect()->wait();
			cout << "OK" << endl;
		}
		else {
			cout << "\nClient was disconnected" << endl;
		}
	}
	catch (const mqtt::exception& exc) {
		cerr << "\n  " << exc << endl;
		return 1;
	}

 	return 0;
}

