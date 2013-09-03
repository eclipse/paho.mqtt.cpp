// client.cpp
// Implementation of the client class for the mqtt C++ client library.

/*******************************************************************************
 * Copyright (c) 2013 Frank Pagliughi <fpagliughi@mindspring.com>
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

namespace mqtt {

const int client::DFLT_QOS = 1;

/////////////////////////////////////////////////////////////////////////////

client::client(const std::string& serverURI, const std::string& clientId)
			: cli_(serverURI, clientId), timeout_(-1)
{
}

client::client(const std::string& serverURI, const std::string& clientId,
			   const std::string& persistDir)
			: cli_(serverURI, clientId, persistDir), timeout_(-1)
{
}

client::client(const std::string& serverURI, const std::string& clientId, 
			   iclient_persistence* persistence)
			: cli_(serverURI, clientId, persistence), timeout_(-1)
{
}

void client::close()
{
	// TODO: What?
}

void client::connect()
{
	cli_.connect()->wait_for_completion(timeout_);
}

void client::connect(connect_options opts)
{
	cli_.connect(opts)->wait_for_completion(timeout_);
}

void client::disconnect()
{
	cli_.disconnect()->wait_for_completion(timeout_);
}

void client::disconnect(long timeout)
{
	cli_.disconnect(timeout)->wait_for_completion(timeout_);
}

//std::string client::generate_client_id()
//{
//}

std::string client::get_client_id() const
{
	return cli_.get_client_id();
}

std::string client::get_server_uri() const
{
	return cli_.get_server_uri();
}

//Debug 	getDebug()
//Return a debug object that can be used to help solve problems.

std::vector<idelivery_token_ptr> client::get_pending_delivery_tokens() const
{
	return cli_.get_pending_delivery_tokens();
}

long client::get_time_to_wait() const
{
	return timeout_;
}

topic client::get_topic(const std::string& top)
{
	return topic(top, cli_);
}

bool client::is_connected() const
{
	return cli_.is_connected();
}

void client::publish(const std::string& top, const void* payload, size_t n, 
					 int qos, bool retained)
{
	cli_.publish(top, payload, n, qos, retained)->wait_for_completion(timeout_);
}

void client::publish(const std::string& top, message_ptr msg)
{
	cli_.publish(top, msg)->wait_for_completion(timeout_);
}

void client::set_callback(callback& cb)
{
	cli_.set_callback(cb);
}

void client::set_time_to_wait(int timeout)
{
	timeout_ = timeout;
}

void client::subscribe(const std::string& topicFilter)
{
	cli_.subscribe(topicFilter, DFLT_QOS)->wait_for_completion(timeout_);
}

void client::subscribe(const topic_filter_collection& topicFilters)
{
	qos_collection qos;
	for (size_t i=0; i<topicFilters.size(); ++i)
		qos.push_back(DFLT_QOS);

	cli_.subscribe(topicFilters, qos)->wait_for_completion(timeout_);
}

void client::subscribe(const topic_filter_collection& topicFilters, 
					   const qos_collection& qos)
{
	cli_.subscribe(topicFilters, qos)->wait_for_completion(timeout_);
}

void client::subscribe(const std::string& topicFilter, int qos)
{
	cli_.subscribe(topicFilter, qos)->wait_for_completion(timeout_);
}

void client::unsubscribe(const std::string& topicFilter)
{
	cli_.unsubscribe(topicFilter)->wait_for_completion(timeout_);
}

void client::unsubscribe(const topic_filter_collection& topicFilters)
{
	qos_collection qos;
	for (size_t i=0; i<topicFilters.size(); ++i)
		qos.push_back(DFLT_QOS);

	cli_.subscribe(topicFilters, qos)->wait_for_completion(timeout_);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}



