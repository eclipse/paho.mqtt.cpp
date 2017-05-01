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
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/client.h"

using namespace std;

const string ADDRESS	{ "tcp://localhost:1883" };
const string CLIENT_ID	{ "sync_consumer" };

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(20);
	connOpts.set_clean_session(true);

	mqtt::client cli(ADDRESS, CLIENT_ID);

	const vector<string> TOPICS { "hello", "command" };
	const vector<int> QOS { 1, 1 };

	// Start the connection.
	// When completed, the callback will subscribe to topic.

	try {
		cout << "Connecting to the MQTT server..." << flush;
		cli.connect(connOpts);
		cli.subscribe(TOPICS, QOS);
		cout << "OK\n" << endl;

		// Consume messages

		cli.start_consuming();

		while (true) {
			auto msg = cli.consume_message();

			if (!msg) {
				if (!cli.is_connected()) {
					cout << "Lost connection. Attempting reconnect" << endl;
					cli.reconnect();
					continue;
				}
				else
					break;
			}
			if (msg->get_topic() == "command" &&
					msg->to_string() == "exit") {
				cout << "Exit command received" << endl;
				break;
			}

			cout << msg->get_topic() << ": " << msg->to_string() << endl;
		}

		cli.stop_consuming();

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

