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

const std::chrono::minutes client::DFLT_TIMEOUT = std::chrono::minutes(5);

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
			   int maxBufferedMessages, iclient_persistence* persistence /*=nullptr*/)
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

void client::set_callback(callback& cb)
{
	userCallback_ = &cb;
	cli_.set_callback(*this);
}

void client::subscribe(const string_collection& topicFilters)
{
	qos_collection qos;
	for (size_t i=0; i<topicFilters.size(); ++i)
		qos.push_back(DFLT_QOS);

	cli_.subscribe(ptr(topicFilters), qos)->wait_for(timeout_);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}



