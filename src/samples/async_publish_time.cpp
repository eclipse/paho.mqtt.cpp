// async_publish_time.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// It's a fairly contrived, but useful example of an MQTT data monitor and
// publisher, using the C++ asynchronous client interface. A fairly common
// usage for MQTT applications to monitor a sensor and publish the reading
// when it changes by a "significant" amount (whatever that may be).
// This might be temperature, pressure, humidity, soil moisture, CO2 levels,
// or anything like that.
//
// Since we don't have a universal sensor to use for this example, we simply
// use time itself as out input data. We periodically "sample" the time
// value and when it changes by more than our required delta amount, we
// publish the time. In this case we use the system clock, measuring the
// time with millisecond precision.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker
//  - Sampling a value
//  - Publishing messages
//  - Last will and testament
//  - Callbacks with lambdas
//  - Implementing callbacks and action listeners
//

/*******************************************************************************
 * Copyright (c) 2019 Frank Pagliughi <fpagliughi@mindspring.com>
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
using namespace std::chrono;

const std::string DFLT_SERVER_ADDRESS { "tcp://localhost:1883" };

// The QoS for sending data
const int QOS = 1;

// How often to sample the "data"
const auto SAMPLE_PERIOD = milliseconds(5);

// How much the "data" needs to change before we publish a new value.
const int DELTA_MS = 100;

// How many to buffer while off-line
const int MAX_BUFFERED_MESSAGES = 1200;

// --------------------------------------------------------------------------
// Gets the current time as the number of milliseconds since the epoch:
// like a time_t with ms resolution.

uint64_t timestamp()
{
	auto now = system_clock::now();
	auto tse = now.time_since_epoch();
	auto msTm = duration_cast<milliseconds>(tse);
	return uint64_t(msTm.count());
}

// --------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	string address = (argc > 1) ? string(argv[1]) : DFLT_SERVER_ADDRESS;
	uint64_t trun = (argc > 2) ? stoll(argv[2]) : 0LL;

	cout << "Initializing for server '" << address << "'..." << endl;
	mqtt::async_client cli(address, "", MAX_BUFFERED_MESSAGES);

	// Set callbacks for connected and connection lost.

	cli.set_connected_handler([&cli](const std::string&) {
		std::cout << "*** Connected ("
			<< timestamp() << ") ***" << std::endl;
	});

	cli.set_connection_lost_handler([&cli](const std::string&) {
		std::cout << "*** Connection Lost ("
			<< timestamp() << ") ***" << std::endl;
	});

	mqtt::connect_options connopts;
	mqtt::message willmsg("test/events", "Time publisher disconnected", 1, true);
	mqtt::will_options will(willmsg);
	connopts.set_will(will);
	connopts.set_automatic_reconnect(1, 10);

	try {
		cout << "Connecting..." << endl;
		cli.connect(connopts)->wait();

		mqtt::topic top(cli, "data/time", QOS);
		cout << "Publishing data..." << endl;

		uint64_t	t = timestamp(),
					tlast = t,
					tstart = t;

		top.publish(to_string(t));

		while (true) {
			this_thread::sleep_for(SAMPLE_PERIOD);

			t = timestamp();
			if (abs(int(t - tlast)) >= DELTA_MS)
				top.publish(to_string(tlast = t));

			if (trun > 0 && t >= (trun + tstart))
				break;
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

