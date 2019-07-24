// rpc_math_cli.cpp
//
// This is a Paho MQTT v5 C++ sample application.
//
// It's an example of how to create a client for performing remote procedure
// calls using MQTT with the 'response topic' and 'correlation data'
// properties.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker
//  - Using MQTT v5 properties
//  - Publishing RPC request messages
//  - Using asynchronous tokens
//	- Subscribing to reply topic
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
#include <sstream>
#include <cstdlib>
#include <string>
#include <thread>	// For sleep
#include <atomic>
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"
#include "mqtt/properties.h"

using namespace std;
using namespace std::chrono;

const string	SERVER_ADDRESS { "tcp://localhost:1883" };
const string	REQ_TOPIC { "requests/math" };
const int		QOS = 1;

const auto TIMEOUT = std::chrono::seconds(10);

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	mqtt::async_client cli(SERVER_ADDRESS, "");

	mqtt::connect_options connopts;
	connopts.set_mqtt_version(MQTTVERSION_5);
	connopts.set_clean_start(true);

	cli.start_consuming();

	try {
		cout << "\nConnecting..." << flush;
		mqtt::token_ptr conntok = cli.connect(connopts);
		conntok->wait();
		auto connRsp = conntok->get_connect_response();
		cout << "OK (" << connRsp.serverURI << ")" << endl;

		// Figure out the "reply to" topic from the assigned (unique) client ID
		mqtt::properties conn_props = conntok->get_properties();
		string clientId = get<string>(conntok->get_properties(),
									  mqtt::property::ASSIGNED_CLIENT_IDENTIFER);

		//cout << "Client ID: " << clientId << endl;
		string repTopic = "replies/" + clientId + "/math";
		cout << "    Reply topic: " << repTopic << endl;

		// Subscribe to the reply topic and verify the QoS

		mqtt::token_ptr tok = cli.subscribe(repTopic, 1);
		tok->wait();

		if (int(tok->get_reason_code()) != 1) {
			cerr << "Error: Server doesn't support reply QoS: "
				<< tok->get_reason_code() << endl;
			return 1;
		}

		mqtt::properties props {
			{ mqtt::property::RESPONSE_TOPIC, repTopic },
			{ mqtt::property::CORRELATION_DATA, "1" }
		};

		ostringstream os;
		os << "[ ";
		for (int i=1; i<argc-1; ++i)
			os << argv[i] << ", ";
		os << argv[argc-1] << " ]";

		cout << "\nSending add request " << os.str() << "..." << flush;
		mqtt::message_ptr pubmsg = mqtt::make_message(REQ_TOPIC, os.str());
		pubmsg->set_qos(QOS);
		pubmsg->set_properties(props);

		cli.publish(pubmsg)->wait_for(TIMEOUT);
		cout << "OK" << endl;

		// Wait for reply.

		auto msg = cli.try_consume_message_for(seconds(5));
		if (!msg) {
			cerr << "Didn't receive a reply from the service." << endl;
			return 1;
		}

		//cout << msg->get_topic() << ": " << msg->to_string() << endl;
		cout << "  Result: " << msg->to_string() << endl;

		// Unsubscribe

		cli.unsubscribe(repTopic)->wait();

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

