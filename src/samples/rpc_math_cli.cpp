// async_publish.cpp
//
// This is a Paho MQTT v5 C++ sample application.
//
// It's an example of how to create a client for performing remote procedure
// calls using MQTT with the 'response topic' and 'correlation data'
// properties.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker
//  - Publishing RPC reuest messages
//  - Last will and testament
//  - Using asynchronous tokens
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
#include "mqtt/properties.h"

using namespace std;

const std::string DFLT_SERVER_ADDRESS	{ "tcp://localhost:1883" };
const std::string DFLT_CLIENT_ID		{ "cpp_rpc_math_cli" };

const string TOPIC { "requests/math" };
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

	void delivery_complete(mqtt::delivery_token_ptr tok) override {
		cout << "\tDelivery complete for token: "
			<< (tok ? tok->get_message_id() : -1) << endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A base action listener.
 */
class action_listener : public virtual mqtt::iaction_listener
{
protected:
	void on_failure(const mqtt::token& tok) override {
		cout << "\tListener failure for token: "
			<< tok.get_message_id() << endl;
	}

	void on_success(const mqtt::token& tok) override {
		cout << "\tListener success for token: "
			<< tok.get_message_id() << endl;
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A derived action listener for publish events.
 */
class delivery_action_listener : public action_listener
{
	atomic<bool> done_;

	void on_failure(const mqtt::token& tok) override {
		action_listener::on_failure(tok);
		done_ = true;
	}

	void on_success(const mqtt::token& tok) override {
		action_listener::on_success(tok);
		done_ = true;
	}

public:
	delivery_action_listener() : done_(false) {}
	bool is_done() const { return done_; }
};

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	string	address  = (argc > 1) ? string(argv[1]) : DFLT_SERVER_ADDRESS,
			clientID = (argc > 2) ? string(argv[2]) : DFLT_CLIENT_ID;

	cout << "Initializing for server '" << address << "'..." << endl;
	mqtt::async_client client(address, clientID);

	callback cb;
	client.set_callback(cb);

	mqtt::connect_options connopts;
	connopts.set_mqtt_version(MQTTVERSION_5);
	connopts.set_clean_session(false);

	cout << "  ...OK" << endl;

	try {
		cout << "\nConnecting..." << endl;
		mqtt::token_ptr conntok = client.connect(connopts);
		cout << "Waiting for the connection..." << endl;
		conntok->wait();
		cout << "  ...OK" << endl;

		/*
		mqtt::property fmtind  { mqtt::property::PAYLOAD_FORMAT_INDICATOR, 42 };
		mqtt::property msgexp  { mqtt::property::MESSAGE_EXPIRY_INTERVAL, 1000 };
		mqtt::property rsptop  { mqtt::property::RESPONSE_TOPIC, "replies/bubba" };
		mqtt::property usrprop { mqtt::property::USER_PROPERTY, "bubba", "wally wanna" };

		mqtt::property usrprop2 { std::move(usrprop) };
		auto usr = mqtt::get<mqtt::string_pair>(usrprop2);

		cout << fmtind.type_name() << ": "
				<< unsigned(mqtt::get<uint8_t>(fmtind)) << "\n"
			<< msgexp.type_name() << ": "
				<< mqtt::get<int16_t>(msgexp) << "\n"
			<< rsptop.type_name() << ": "
				<< mqtt::get<string>(rsptop) << "\n"
			<< usrprop.type_name() << ": ("
				<< std::get<0>(usr) << ", "
				<< std::get<1>(usr) << ")" << "\n"
			<< endl;
		*/

		mqtt::properties props;
		props.add({ mqtt::property::RESPONSE_TOPIC, "replies/bubba" });
		props.add({ mqtt::property::CORRELATION_DATA, "1" });

		cout << "\nSending request..." << endl;
		mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, "[4, 5]");
		pubmsg->set_qos(QOS);
		pubmsg->set_properties(props);

		client.publish(pubmsg)->wait_for(TIMEOUT);
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

