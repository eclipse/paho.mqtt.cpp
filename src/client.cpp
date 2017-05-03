// client.cpp
// Implementation of the client class for the mqtt C++ client library.

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


#include "mqtt/client.h"
#include <memory>
#include <iostream>

namespace mqtt {

constexpr std::chrono::minutes client::DFLT_TIMEOUT;
constexpr int client::DFLT_QOS;

/////////////////////////////////////////////////////////////////////////////


client::client(const string& serverURI, const string& clientId,
			   iclient_persistence* persistence /*=nullptr*/)
		: cli_(serverURI, clientId, persistence), timeout_(DFLT_TIMEOUT)
{
}

client::client(const string& serverURI, const string& clientId,
			   const string& persistDir)
		: cli_(serverURI, clientId, persistDir), timeout_(DFLT_TIMEOUT)
{
}

client::client(const string& serverURI, const string& clientId,
			   int maxBufferedMessages, iclient_persistence* persistence /*=nullptr*/)
		: cli_(serverURI, clientId, maxBufferedMessages, persistence), timeout_(DFLT_TIMEOUT)
{
}

client::client(const string& serverURI, const string& clientId,
			   int maxBufferedMessages, const string& persistDir)
		: cli_(serverURI, clientId, maxBufferedMessages, persistDir), timeout_(DFLT_TIMEOUT)
{
}

void client::close()
{
	// TODO: What?
}

//string client::generate_client_id()
//{
//}

std::vector<delivery_token_ptr> client::get_pending_delivery_tokens() const
{
	return cli_.get_pending_delivery_tokens();
}

void client::publish(string_ref top, const void* payload, size_t n,
					 int qos, bool retained)
{
	cli_.publish(std::move(top), payload, n, qos, retained)->wait_for(timeout_);
}

void client::publish(string_ref top, const void* payload, size_t n)
{
	cli_.publish(std::move(top), payload, n)->wait_for(timeout_);
}

void client::publish(const_message_ptr msg)
{
	cli_.publish(msg)->wait_for(timeout_);
}

void client::publish(const message& msg)
{
	cli_.publish(ptr(msg))->wait_for(timeout_);
}

void client::set_callback(callback& cb)
{
	cli_.set_callback(cb);
}

void client::subscribe(const string& topicFilter)
{
	cli_.subscribe(topicFilter, DFLT_QOS)->wait_for(timeout_);
}

void client::subscribe(const string_collection& topicFilters)
{
	qos_collection qos;
	for (size_t i=0; i<topicFilters.size(); ++i)
		qos.push_back(DFLT_QOS);

	cli_.subscribe(ptr(topicFilters), qos)->wait_for(timeout_);
}

void client::subscribe(const string_collection& topicFilters,
					   const qos_collection& qos)
{
	cli_.subscribe(ptr(topicFilters), qos)->wait_for(timeout_);
}

void client::subscribe(const string& topicFilter, int qos)
{
	cli_.subscribe(topicFilter, qos)->wait_for(timeout_);
}

void client::unsubscribe(const string& topicFilter)
{
	cli_.unsubscribe(topicFilter)->wait_for(timeout_);
}

void client::unsubscribe(const string_collection& topicFilters)
{
	cli_.unsubscribe(ptr(topicFilters))->wait_for(timeout_);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}



