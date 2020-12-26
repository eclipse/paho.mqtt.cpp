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

const std::chrono::seconds client::DFLT_TIMEOUT = std::chrono::seconds(30);

#if __cplusplus < 201703L
	constexpr int client::DFLT_QOS;
#endif

/////////////////////////////////////////////////////////////////////////////

client::client(const string& serverURI, const string& clientId,
			   iclient_persistence* persistence /*=nullptr*/)
		: cli_(serverURI, clientId, persistence),
			timeout_(DFLT_TIMEOUT), userCallback_(nullptr)
{
}

client::client(const string& serverURI, const string& clientId,
			   const string& persistDir)
		: cli_(serverURI, clientId, persistDir),
			timeout_(DFLT_TIMEOUT), userCallback_(nullptr)
{
}

client::client(const string& serverURI, const string& clientId,
			   int maxBufferedMessages,
			   iclient_persistence* persistence /*=nullptr*/)
		: cli_(serverURI, clientId, maxBufferedMessages, persistence),
			timeout_(DFLT_TIMEOUT), userCallback_(nullptr)
{
}

client::client(const string& serverURI, const string& clientId,
			   int maxBufferedMessages, const string& persistDir)
		: cli_(serverURI, clientId, maxBufferedMessages, persistDir),
			timeout_(DFLT_TIMEOUT), userCallback_(nullptr)
{
}

client::client(const string& serverURI, const string& clientId,
			   const create_options& opts,
			   iclient_persistence* persistence /*=nullptr*/)
		: cli_(serverURI, clientId, opts, persistence),
			timeout_(DFLT_TIMEOUT), userCallback_(nullptr)
{
}

// --------------------------------------------------------------------------

void client::set_callback(callback& cb)
{
	userCallback_ = &cb;
	cli_.set_callback(*this);
}

connect_response client::connect()
{
	cli_.start_consuming();
	auto tok = cli_.connect();
	if (!tok->wait_for(timeout_))
		throw timeout_error();
	return tok->get_connect_response();
}

connect_response client::connect(connect_options opts)
{
	cli_.start_consuming();
	auto tok = cli_.connect(std::move(opts));
	if (!tok->wait_for(timeout_))
		throw timeout_error();
	return tok->get_connect_response();
}

connect_response client::reconnect()
{
	auto tok = cli_.reconnect();
	if (!tok->wait_for(timeout_))
		throw timeout_error();
	return tok->get_connect_response();
}

subscribe_response client::subscribe(const string& topicFilter,
									 const subscribe_options& opts /*=subscribe_options()*/,
									 const properties& props /*=properties()*/)
{
	auto tok = cli_.subscribe(topicFilter, DFLT_QOS, opts, props);
	if (!tok->wait_for(timeout_))
		throw timeout_error();
	return tok->get_subscribe_response();
}

subscribe_response client::subscribe(const string& topicFilter, int qos,
									 const subscribe_options& opts /*=subscribe_options()*/,
									 const properties& props /*=properties()*/)
{
	auto tok = cli_.subscribe(topicFilter, qos, opts, props);
	if (!tok->wait_for(timeout_))
		throw timeout_error();
	return tok->get_subscribe_response();
}

subscribe_response client::subscribe(const string_collection& topicFilters,
									 const std::vector<subscribe_options>& opts /*=std::vector<subscribe_options>()*/,
									 const properties& props /*=properties()*/)
{
	qos_collection qos;
	for (size_t i=0; i<topicFilters.size(); ++i)
		qos.push_back(DFLT_QOS);

	auto tok = cli_.subscribe(ptr(topicFilters), qos, opts, props);
	if (!tok->wait_for(timeout_))
		throw timeout_error();
	return tok->get_subscribe_response();
}

subscribe_response client::subscribe(const string_collection& topicFilters,
									 const qos_collection& qos,
									 const std::vector<subscribe_options>& opts /*=std::vector<subscribe_options>()*/,
									 const properties& props /*=properties()*/)
{
	auto tok = cli_.subscribe(ptr(topicFilters), qos, opts, props);
	if (!tok->wait_for(timeout_))
		throw timeout_error();
	return tok->get_subscribe_response();
}

unsubscribe_response client::unsubscribe(const string& topicFilter,
										 const properties& props /*=properties()*/)
{
	auto tok = cli_.unsubscribe(topicFilter, props);
	if (!tok->wait_for(timeout_))
		throw timeout_error();
	return tok->get_unsubscribe_response();
}

unsubscribe_response client::unsubscribe(const string_collection& topicFilters,
										 const properties& props /*=properties()*/)
{
	auto tok = cli_.unsubscribe(ptr(topicFilters), props);
	if (!tok->wait_for(timeout_))
		throw timeout_error();
	return tok->get_unsubscribe_response();
}

void client::disconnect()
{
	cli_.stop_consuming();
	if (!cli_.disconnect()->wait_for(timeout_))
		throw timeout_error();
}

void client::disconnect(int timeoutMS)
{
	cli_.stop_consuming();
	if (!cli_.disconnect(timeoutMS)->wait_for(timeout_))
		throw timeout_error();
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

