// data_publish.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// This application is an example of how to collect and publish data to
// MQTT. It's an MQTT publisher using the C++ asynchronous client interface.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker.
//  - Publishing messages to a topic
//  - Automatic reconnects.
//  - Off-line buffering
//  - Default file-based persistence
//

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

#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "mqtt/async_client.h"

using namespace std;
using namespace std::chrono;

const std::string DFLT_ADDRESS { "tcp://localhost:1883" };

const string TOPIC { "data/rand" };
const int	 QOS = 1;

const auto TIMEOUT = seconds(10);
const auto PERIOD  = seconds(5);

const int MAX_BUFFERED_MSGS = 120;	// 120 * 5sec => 10min off-line buffering

const string PERSIST_DIR { "data-persist" };

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	string address = (argc > 1) ? string(argv[1]) : DFLT_ADDRESS;

	cout << "Initializing for server '" << address << "'..." << endl;
	mqtt::async_client cli(address, "", MAX_BUFFERED_MSGS, PERSIST_DIR);
	cout << "  ...OK" << endl;

	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD);
	connOpts.set_clean_session(true);
	connOpts.set_automatic_reconnect(true);

	// Random number generator [0 - 100]
	random_device rnd;
    mt19937 gen(rnd());
    uniform_int_distribution<> dis(0, 100);

	try {
		cout << "\nConnecting..." << endl;
		cli.connect(connOpts)->wait();
		cout << "  ...OK" << endl;

		char tmbuf[32];
		unsigned nsample = 0;

		auto msg = mqtt::message::create(TOPIC, "", QOS, true);
		auto tm = steady_clock::now();

		while (true) {
			this_thread::sleep_until(tm);

			time_t t = system_clock::to_time_t(system_clock::now());
			strftime(tmbuf, sizeof(tmbuf), "%F %T", localtime(&t));

			int x = int(dis(gen));
			string payload = to_string(++nsample) + "," + 
						tmbuf + "," + to_string(x);
			cout << payload << endl;

			msg->set_payload(std::move(payload));
			cli.publish(msg);

			tm += PERIOD;
		}

		// Disconnect
		cout << "\nDisconnecting..." << endl;
		cli.disconnect()->wait();
		cout << "  ...OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}

