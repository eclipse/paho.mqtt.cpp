// client.cpp
// Implementation of the client class for the mqtt C++ client library.

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


#include "mqtt/client.h"
#include <memory>
#include <iostream>

namespace mqtt {

const int client::DFLT_QOS = 1;

/////////////////////////////////////////////////////////////////////////////

client::client(const string& serverURI, const string& clientId)
			: cli_(serverURI, clientId), timeout_(-1)
{
}

client::client(const string& serverURI, const string& clientId,
			   const string& persistDir)
			: cli_(serverURI, clientId, persistDir), timeout_(-1)
{
}

client::client(const string& serverURI, const string& clientId,
			   iclient_persistence* persistence)
			: cli_(serverURI, clientId, persistence), timeout_(-1)
{
}

void client::close()
{
	// TODO: What?
}

void client::disconnect(int timeout)
{
	cli_.disconnect(timeout)->wait_for_completion(timeout_);
}


//string client::generate_client_id()
//{
//}

std::vector<delivery_token_ptr> client::get_pending_delivery_tokens() const
{
	return cli_.get_pending_delivery_tokens();
}

void client::publish(const string& top, const void* payload, size_t n,
					 int qos, bool retained)
{
	cli_.publish(top, payload, n, qos, retained)->wait_for_completion(timeout_);
}

void client::publish(const string& top, const_message_ptr msg)
{
	cli_.publish(top, msg)->wait_for_completion(timeout_);
}

void client::publish(const string& top, const message& msg)
{
	// Don't destroy non-heap message.
	std::shared_ptr<message> msgp(const_cast<message*>(&msg), [](message*){});
	cli_.publish(top, msgp)->wait_for_completion(timeout_);
}

void client::set_callback(callback& cb)
{
	cli_.set_callback(cb);
}

void client::subscribe(const string& topicFilter)
{
	cli_.subscribe(topicFilter, DFLT_QOS)->wait_for_completion(timeout_);
}

void client::subscribe(const topic_collection& topicFilters)
{
	qos_collection qos;
	for (size_t i=0; i<topicFilters.size(); ++i)
		qos.push_back(DFLT_QOS);

	cli_.subscribe(topicFilters, qos)->wait_for_completion(timeout_);
}

void client::subscribe(const topic_collection& topicFilters,
					   const qos_collection& qos)
{
	cli_.subscribe(topicFilters, qos)->wait_for_completion(timeout_);
}

void client::subscribe(const string& topicFilter, int qos)
{
	cli_.subscribe(topicFilter, qos)->wait_for_completion(timeout_);
}

void client::unsubscribe(const string& topicFilter)
{
	cli_.unsubscribe(topicFilter)->wait_for_completion(timeout_);
}

void client::unsubscribe(const topic_collection& topicFilters)
{
	cli_.unsubscribe(topicFilters)->wait_for_completion(timeout_);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}



