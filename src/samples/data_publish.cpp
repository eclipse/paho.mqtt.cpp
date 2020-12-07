// data_publish.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// It's an example of how to collect and publish periodic data to MQTT, as
// an MQTT publisher using the C++ asynchronous client interface.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker
//  - Publishing messages
//  - Using a topic object to repeatedly publish to the same topic.
//  - Automatic reconnects
//  - Off-line buffering
//  - Default file-based persistence
//
// This just uses the steady clock to run a periodic loop. Each time
// through, it generates a random number [0-100] as simulated data and
// creates a text, CSV payload in the form:
//  	<sample #>,<time stamp>,<data>
//
// Note that it uses the steady clock to pace the periodic timing, but then
// reads the system_clock to generate the timestamp for local calendar time.
//
// The sample number is just a counting integer to help test the off-line
// buffering to easily confirm that all the messages got across.
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

// Don't worry about localtime() in this context
#if defined(_WIN32)
	#define _CRT_SECURE_NO_WARNINGS
#endif

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
const std::string CLIENT_ID { "paho-cpp-data-publish" };

const string TOPIC { "data/rand" };
const int	 QOS = 1;

const auto PERIOD = seconds(5);

const int MAX_BUFFERED_MSGS = 120;	// 120 * 5sec => 10min off-line buffering

const string PERSIST_DIR { "data-persist" };

/////////////////////////////////////////////////////////////////////////////

class persistence_encoder : virtual public mqtt::ipersistence_encoder
{
	/**
	 * Callback to let the application encode data before writing it to
	 * persistence.
	 */
	void encode(mqtt::string_view bufs[], size_t n) override {
		cout << "Encoding " << n << " buffers" << endl;
		auto sz = bufs[0].size();
		auto buf = mqtt::persistence_malloc(sz+6);
		strcpy(buf, "bubba");
		memcpy(buf+6, bufs[0].data(), sz);
		bufs[0] = mqtt::string_view(buf, n+6);
	}
	/**
	 * Callback to let the application decode data after it is retrieved
	 * from persistence.
	 *
	 * @param buffers The data buffers that need to be decoded.
	 * @param n The number of buffers
	 */
	void decode(mqtt::string_view& buf) override {
		cout << "Decoding buffer: " << buf.data() << endl;
		auto n = buf.size();
		auto newBuf = mqtt::persistence_malloc(n-6);
		memcpy(newBuf, buf.data(), n-6);
		buf = mqtt::string_view(newBuf, n-6);
	}
};

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	string address = (argc > 1) ? string(argv[1]) : DFLT_ADDRESS;

	persistence_encoder encoder;
	mqtt::async_client cli(address, CLIENT_ID, MAX_BUFFERED_MSGS,
						   PERSIST_DIR, &encoder);

	auto connOpts = mqtt::connect_options_builder()
		.keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD)
		.clean_session(true)
		.automatic_reconnect(true)
		.finalize();

	// Create a topic object. This is a conventience since we will
	// repeatedly publish messages with the same parameters.
	mqtt::topic top(cli, TOPIC, QOS, true);

	// Random number generator [0 - 100]
	random_device rnd;
    mt19937 gen(rnd());
    uniform_int_distribution<> dis(0, 100);

	try {
		// Connect to the MQTT broker
		cout << "Connecting to server '" << address << "'..." << flush;
		cli.connect(connOpts)->wait();
		cout << "OK\n" << endl;

		char tmbuf[32];
		unsigned nsample = 0;

		// The time at which to reads the next sample, starting now
		auto tm = steady_clock::now();

		while (true) {
			// Pace the samples to the desired rate
			this_thread::sleep_until(tm);

			// Get a timestamp and format as a string
			time_t t = system_clock::to_time_t(system_clock::now());
			strftime(tmbuf, sizeof(tmbuf), "%F %T", localtime(&t));

			// Simulate reading some data
			int x = dis(gen);

			// Create the payload as a text CSV string
			string payload = to_string(++nsample) + "," +
								tmbuf + "," + to_string(x);
			cout << payload << endl;

			// Publish to the topic
			top.publish(std::move(payload));

			tm += PERIOD;
		}

		// Disconnect
		cout << "\nDisconnecting..." << flush;
		cli.disconnect()->wait();
		cout << "OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}

