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
	// XOR bit mask for data.
	uint16_t mask_;

	/**
	 * Callback to let the application encode data before writing it to
	 * persistence.
	 */
	void encode(size_t nbuf, char* bufs[], size_t lens[]) override {
		for (size_t i=0; i<nbuf; ++i) {
			auto sz = lens[i];
			auto buf16 = static_cast<uint16_t*>(mqtt::persistence_malloc(sz*2));

			for (size_t j=0; j<sz; ++j)
				buf16[j] = uint16_t(bufs[i][j] ^ mask_);

			mqtt::persistence_free(bufs[i]);
			bufs[i] = reinterpret_cast<char*>(buf16);
			lens[i] = sz*2;
		}
	}
	/**
	 * Callback to let the application decode data after it is retrieved
	 * from persistence.
	 *
	 * We do an in-place decode taking care with the overlapped data.
	 */
	void decode(char** pbuf, size_t* len) override {
		cout << "Decoding buffer @: 0x" << pbuf << endl;
		char* buf = *pbuf;
		uint16_t* buf16 = reinterpret_cast<uint16_t*>(*pbuf);
		size_t sz = *len / 2;

		for (size_t i=0; i<sz; ++i)
			buf[i] = char(buf16[i] ^ mask_);

		*len = sz;
	}

public:
	persistence_encoder() : mask_(0x0055) {}
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

			// TODO: Get rid of this
			break;
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

