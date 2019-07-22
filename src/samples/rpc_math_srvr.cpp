// rpc_math_srvr.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// This application is an MQTT consumer/subscriber using the C++ synchronous
// client interface, which uses the queuing API to receive messages.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker
//  - Subscribing to multiple topics
//  - Receiving messages through the queueing consumer API
//  - Recieving and acting upon commands via MQTT topics
//  - Manual reconnects
//  - Using a persistent (non-clean) session
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
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/client.h"

using namespace std;
using namespace std::chrono;

const string SERVER_ADDRESS	{ "tcp://localhost:1883" };
const string CLIENT_ID		{ "rpc_math_srvr" };

// --------------------------------------------------------------------------
// Simple function to manually reconect a client.

bool try_reconnect(mqtt::client& cli)
{
	constexpr int N_ATTEMPT = 30;

	for (int i=0; i<N_ATTEMPT && !cli.is_connected(); ++i) {
		try {
			cli.reconnect();
			return true;
		}
		catch (const mqtt::exception&) {
			this_thread::sleep_for(seconds(1));
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	mqtt::connect_options connOpts;
	connOpts.set_mqtt_version(MQTTVERSION_5);
	connOpts.set_keep_alive_interval(20);
	connOpts.set_clean_start(true);


	mqtt::client cli(SERVER_ADDRESS, CLIENT_ID);

	const vector<string> TOPICS { "requests/math" };
	const vector<int> QOS { 1 };

	try {
		cout << "Connecting to the MQTT server..." << flush;
		cli.connect(connOpts);
		cli.subscribe(TOPICS, QOS);
		cout << "OK\n" << endl;

		// Consume messages

		while (true) {
			auto msg = cli.consume_message();

			if (!msg) {
				if (!cli.is_connected()) {
					cout << "Lost connection. Attempting reconnect" << endl;
					if (try_reconnect(cli)) {
						cli.subscribe(TOPICS, QOS);
						cout << "Reconnected" << endl;
						continue;
					}
					else {
						cout << "Reconnect failed." << endl;
						break;
					}
				}
				else
					break;
			}

			cout << "Received a request" << endl;

			const mqtt::properties& props = msg->get_properties();
			if (props.contains(mqtt::property::CORRELATION_DATA) &&
					props.contains(mqtt::property::RESPONSE_TOPIC)) {
				mqtt::binary corr_id  = mqtt::get<string>(props, mqtt::property::CORRELATION_DATA);
				string reply_to = mqtt::get<string>(props, mqtt::property::RESPONSE_TOPIC);

				cout << "Client wants a reply to [" << corr_id << "] on '"
					<< reply_to << "'" << endl;
			}

			if (msg->get_topic() == "requests/math" &&
					msg->to_string() == "exit") {
				cout << "Exit command received" << endl;
				break;
			}

			cout << msg->get_topic() << ": " << msg->to_string() << endl;
		}

		// Disconnect

		cout << "\nDisconnecting from the MQTT server..." << flush;
		cli.disconnect();
		cout << "OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}

