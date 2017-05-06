// token.cpp

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

#include "mqtt/token.h"
#include "mqtt/async_client.h"
#include <cstring>

#include <iostream>

namespace mqtt {

// --------------------------------------------------------------------------
// Constructors

token::token(iasync_client& cli) : token(cli, MQTTAsync_token(0))
{
}

token::token(iasync_client& cli, void* userContext, iaction_listener& cb)
				: token(cli, const_string_collection_ptr(), userContext, cb)
{
}

token::token(iasync_client& cli, const string& top)
				: token(cli, string_collection::create(top))
{
}

token::token(iasync_client& cli, const string& top,
			 void* userContext, iaction_listener& cb)
				: token(cli, string_collection::create(top), userContext, cb)
{
}

token::token(iasync_client& cli, const_string_collection_ptr topics)
				: cli_(&cli), tok_(MQTTAsync_token(0)), topics_(topics),
						userContext_(nullptr), listener_(nullptr),
						complete_(false), rc_(0)
{
}

token::token(iasync_client& cli, const_string_collection_ptr topics,
			 void* userContext, iaction_listener& cb)
				: cli_(&cli), tok_(MQTTAsync_token(0)), topics_(topics),
						userContext_(userContext), listener_(&cb),
						complete_(false), rc_(0)
{
}

token::token(iasync_client& cli, MQTTAsync_token tok)
				: cli_(&cli), tok_(tok), userContext_(nullptr),
					listener_(nullptr), complete_(false), rc_(0)
{
}

// --------------------------------------------------------------------------
// Class static callbacks.
// These are the callbacks directly from the C library.
// The 'context' is a raw pointer to the token object.

void token::on_success(void* context, MQTTAsync_successData* rsp)
{
	if (context)
		static_cast<token*>(context)->on_success(rsp);
}

void token::on_failure(void* context, MQTTAsync_failureData* rsp)
{
	if (context)
		static_cast<token*>(context)->on_failure(rsp);
}

// --------------------------------------------------------------------------
// Object callbacks

void token::on_success(MQTTAsync_successData* rsp)
{
	unique_lock g(lock_);
	iaction_listener* listener = listener_;
	tok_ = (rsp) ? rsp->token : 0;
	rc_ = MQTTASYNC_SUCCESS;
	complete_ = true;
	g.unlock();

	// Note: callback always completes before the object is signaled.
	if (listener)
		listener->on_success(*this);
	cond_.notify_all();

	cli_->remove_token(this);
}

void token::on_failure(MQTTAsync_failureData* rsp)
{
	unique_lock g(lock_);
	iaction_listener* listener = listener_;
	if (rsp) {
		tok_ = rsp->token;
		rc_ = rsp->code;
		if (rsp->message)
			errMsg_ = string(rsp->message);
	}
	else {
		tok_ = 0;
		rc_ = -1;
	}
	complete_ = true;
	g.unlock();

	// Note: callback always completes before the obect is signaled.
	if (listener)
		listener->on_failure(*this);
	cond_.notify_all();

	cli_->remove_token(this);
}

// --------------------------------------------------------------------------
// API

void token::reset()
{
	guard g(lock_);
	complete_ = false;
	rc_ = 0;
	errMsg_.clear();
}

void token::wait()
{
	unique_lock g(lock_);
	cond_.wait(g, [this]{return complete_;});
	check_rc();
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

