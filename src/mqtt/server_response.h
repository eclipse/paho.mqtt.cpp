/////////////////////////////////////////////////////////////////////////////
/// @file server_response.h
/// Declaration of MQTT server response classes.
/// @date July 26, 2019
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

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

#ifndef __mqtt_server_response_h
#define __mqtt_server_response_h

#include "MQTTAsync.h"
#include "mqtt/types.h"
#include "mqtt/properties.h"

#include <iostream>

namespace mqtt {

/**
 * Base class for responses from the server.
 */
class server_response
{
	/** The properties from the acknowledge  */
	properties props_;

public:
	/**
	 * Creates a response with empty property list.
	 */
	server_response() {}
	/**
	 * Creates a server response with the specified properties.
	 * @param props The properties in the response.
	 */
	server_response(const properties& props)
		: props_(props) {}
	/**
	 * Creates a server response with the specified properties.
	 * @param props The properties in the response.
	 */
	server_response(properties&& props)
		: props_(std::move(props)) {}
	/**
	 * Virtual destructor.
	 */
	virtual ~server_response() {}
	/**
	 * Gets the properties from the response.
	 * @return The properties from the response.
	 */
	const properties& get_properties() const { return props_; }
};

/**
 * Response for a connect request
 */
class connect_response : public server_response
{
	/** The connection string of the server */
	string serverURI_;
	/** The version of MQTT being used */
	int mqttVersion_;
	/** The session present flag returned from the server */
	bool sessionPresent_;

	friend class token;

	connect_response(const MQTTAsync_successData5* rsp) :
		server_response(properties(rsp->properties)),
		serverURI_(string(rsp->alt.connect.serverURI)),
		mqttVersion_(rsp->alt.connect.MQTTVersion),
		sessionPresent_(to_bool(rsp->alt.connect.sessionPresent)) {
	}

	connect_response(const MQTTAsync_successData* rsp) :
		serverURI_(string(rsp->alt.connect.serverURI)),
		mqttVersion_(rsp->alt.connect.MQTTVersion),
		sessionPresent_(to_bool(rsp->alt.connect.sessionPresent)) {
	}

public:
	/**
	 * Gets the URI of the broker to which we connected.
	 * @return The URI of the broker.
	 */
	string get_server_uri() const { return serverURI_; }
	/**
	 * Gets the MQTT version for the connection.
	 * @return The MQTT version for the connection.
	 */
	int get_mqtt_version() const { return mqttVersion_; }
	/**
	 * Determines whether a session already existed for this client on the
	 * server.
	 * This tells whether the server has a persistent session stored for the
	 * client, given the ClientID specified in the connect message.
	 * @return Whether a session already existed for this client on the server.
	 */
	bool is_session_present() const { return sessionPresent_; }
};

/**
 * Response for subscribe messages
 */
struct subscribe_response : public server_response
{
	/** The reason/result code for each topic request. */
	std::vector<ReasonCode> reasonCodes_;

	friend class token;

	/**
	 * Create v5 subscribe response.
	 * @param rsp The v5 response struct from the C lib
	 */
	subscribe_response(MQTTAsync_successData5* rsp)
			: server_response(properties(rsp->properties)) {
		if (rsp->alt.sub.reasonCodeCount < 2)
			reasonCodes_.push_back(ReasonCode(rsp->reasonCode));
		else if (rsp->alt.sub.reasonCodes) {
			for (int i=0; i<rsp->alt.sub.reasonCodeCount; ++i)
				reasonCodes_.push_back(ReasonCode(rsp->alt.sub.reasonCodes[i]));
		}
	}

	/**
	 * Create v3 subscribe response.
	 * @param n The number of subscription topics
	 * @param rsp The v3 response struct from the C lib
	 */
	subscribe_response(size_t n, MQTTAsync_successData* rsp) {
		if (n < 2)
			reasonCodes_.push_back(ReasonCode(rsp->alt.qos));
		else if (rsp->alt.qosList) {
			for (size_t i=0; i<n; ++i)
				reasonCodes_.push_back(ReasonCode(rsp->alt.qosList[i]));
		}
	}

public:
	/**
	 * Gets the reason codes from the server response.
	 * On a subscribe ack there is a reason code for each topic that
	 * was sent in the subscribe packet. Each tells the granted QoS
	 * for the corresponding topic.
	 * @return A collection of return codes corresponding to
	 *  	   subscribing each topic. On success, this is the
	 *  	   granted QoS for each topic. On failure it is the
	 *  	   reason for the failure.
	 */
	const std::vector<ReasonCode>& get_reason_codes() const {
		return reasonCodes_;
	}
};

/**
 * Response for unsubscribe messages.
 */
class unsubscribe_response : public server_response
{
	/** The reason/result code for each topic request. */
	std::vector<ReasonCode> reasonCodes_;

	friend class token;

	unsubscribe_response(MQTTAsync_successData5* rsp)
			: server_response(properties(rsp->properties)) {
		if (rsp->alt.unsub.reasonCodeCount < 2)
			reasonCodes_.push_back(ReasonCode(rsp->reasonCode));
		else if (rsp->alt.unsub.reasonCodes) {
			for (int i=0; i<rsp->alt.unsub.reasonCodeCount; ++i)
				reasonCodes_.push_back(ReasonCode(rsp->alt.unsub.reasonCodes[i]));
		}
	}

	unsubscribe_response(MQTTAsync_successData* rsp) {}

public:
	/**
	 * Gets the reason codes from the server response.
	 * On an unsubscribe ack there is a reason code for each topic
	 * that was sent in the unsubscribe packet. Each tells the
	 * result of unsubscribing to the corresponding topic.
	 * @return A collection of return codes corresponding to
	 *  	   unsubscribing each topic.
	 */
	const std::vector<ReasonCode>& get_reason_codes() const {
		return reasonCodes_;
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_server_response_h

