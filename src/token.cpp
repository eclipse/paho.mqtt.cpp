// token.cpp

/*******************************************************************************
 * Copyright (c) 2013-2019 Frank Pagliughi <fpagliughi@mindspring.com>
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
 *    Frank Pagliughi - MQTT v5 support
 *******************************************************************************/

#include "mqtt/token.h"
#include "mqtt/async_client.h"
#include <cstring>

#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// Paho C logger

enum LOG_LEVELS {
	INVALID_LEVEL = -1,
	TRACE_MAX = 1,
	TRACE_MED,
	TRACE_MIN,
	TRACE_PROTOCOL,
	LOG_PROTOCOL = TRACE_PROTOCOL,
	LOG_ERROR,
	LOG_SEVERE,
	LOG_FATAL,
};

extern "C" {
	void Log(enum LOG_LEVELS, int, const char *, ...);
}

/////////////////////////////////////////////////////////////////////////////

namespace mqtt {

// --------------------------------------------------------------------------
// Constructors

token::token(Type typ, iasync_client& cli, const_string_collection_ptr topics)
				: type_(typ), cli_(&cli), msgId_(MQTTAsync_token(0)), topics_(topics),
						userContext_(nullptr), listener_(nullptr), nExpected_(0),
						complete_(false), rc_(0)
{
}

token::token(Type typ, iasync_client& cli, const_string_collection_ptr topics,
			 void* userContext, iaction_listener& cb)
				: type_(typ), cli_(&cli), msgId_(MQTTAsync_token(0)), topics_(topics),
						userContext_(userContext), listener_(&cb), nExpected_(0),
						complete_(false), rc_(0)
{
}

token::token(Type typ, iasync_client& cli, MQTTAsync_token tok)
				: type_(typ), cli_(&cli), msgId_(tok), userContext_(nullptr),
					listener_(nullptr), nExpected_(0), complete_(false), rc_(0)
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

void token::on_success5(void* context, MQTTAsync_successData5* rsp)
{
	if (context)
		static_cast<token*>(context)->on_success5(rsp);
}

void token::on_failure(void* context, MQTTAsync_failureData* rsp)
{
	if (context)
		static_cast<token*>(context)->on_failure(rsp);
}

void token::on_failure5(void* context, MQTTAsync_failureData5* rsp)
{
	if (context)
		static_cast<token*>(context)->on_failure5(rsp);
}

// --------------------------------------------------------------------------
// Object callbacks

void token::on_success(MQTTAsync_successData* rsp)
{
	::Log(TRACE_MIN, -1, "[cpp] on_success");

	unique_lock g(lock_);
	iaction_listener* listener = listener_;

	if (rsp) {
		msgId_ = rsp->token;

		switch (type_) {
			case Type::CONNECT:
				connRsp_ = std::unique_ptr<connect_response>(new connect_response);
				connRsp_->serverURI = string(rsp->alt.connect.serverURI);
				connRsp_->mqttVersion = rsp->alt.connect.MQTTVersion;
				connRsp_->sessionPresent = to_bool(rsp->alt.connect.sessionPresent);
				break;

			case SUBSCRIBE:
				subRsp_ = std::unique_ptr<subscribe_response>(new subscribe_response);
				subRsp_->reasonCodes.push_back(ReasonCode(rsp->alt.qos));
				break;

			case SUBSCRIBE_MANY:
				subRsp_ = std::unique_ptr<subscribe_response>(new subscribe_response);
				for (size_t i=0; i<nExpected_; ++i)
					subRsp_->reasonCodes.push_back(ReasonCode(rsp->alt.qosList[i]));
				break;
		}
	}

	rc_ = MQTTASYNC_SUCCESS;
	complete_ = true;
	g.unlock();

	// Note: callback always completes before the object is signaled.
	if (listener)
		listener->on_success(*this);
	cond_.notify_all();

	cli_->remove_token(this);
}

void token::on_success5(MQTTAsync_successData5* rsp)
{
	::Log(TRACE_MIN, -1, "[cpp] on_success5");

	unique_lock g(lock_);
	iaction_listener* listener = listener_;
	if (rsp) {
		msgId_ = rsp->token;
		reasonCode_ = ReasonCode(rsp->reasonCode);
		props_ = properties(rsp->properties);

		switch (type_) {
			case Type::CONNECT:
				connRsp_ = std::unique_ptr<connect_response>(new connect_response);
				connRsp_->serverURI = string(rsp->alt.connect.serverURI);
				connRsp_->mqttVersion = rsp->alt.connect.MQTTVersion;
				connRsp_->sessionPresent = to_bool(rsp->alt.connect.sessionPresent);
				break;

			case SUBSCRIBE:
			case SUBSCRIBE_MANY:
				subRsp_ = std::unique_ptr<subscribe_response>(new subscribe_response);
				if (rsp->alt.sub.reasonCodeCount == 1)
					subRsp_->reasonCodes.push_back(reasonCode_);
				else {
					for (int i=0; i<rsp->alt.sub.reasonCodeCount; ++i)
						subRsp_->reasonCodes.push_back(ReasonCode(rsp->alt.sub.reasonCodes[i]));
				}
				break;

			case UNSUBSCRIBE:
			case UNSUBSCRIBE_MANY:
				unsubRsp_ = std::unique_ptr<unsubscribe_response>(new unsubscribe_response);
				if (rsp->alt.unsub.reasonCodeCount == 1)
					unsubRsp_->reasonCodes.push_back(reasonCode_);
				else {
					for (int i=0; i<rsp->alt.unsub.reasonCodeCount; ++i)
						unsubRsp_->reasonCodes.push_back(ReasonCode(rsp->alt.unsub.reasonCodes[i]));
				}
				break;
		}
	}
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
	::Log(TRACE_MIN, -1, "[cpp] on_failure");

	unique_lock g(lock_);
	iaction_listener* listener = listener_;
	if (rsp) {
		msgId_ = rsp->token;
		rc_ = rsp->code;
		if (rsp->message)
			errMsg_ = string(rsp->message);
	}
	else {
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

void token::on_failure5(MQTTAsync_failureData5* rsp)
{
	::Log(TRACE_MIN, -1, "[cpp] on_failure");

	unique_lock g(lock_);
	iaction_listener* listener = listener_;
	if (rsp) {
		msgId_ = rsp->token;
		reasonCode_ = ReasonCode(rsp->reasonCode);
		props_ = properties(rsp->properties);
		rc_ = rsp->code;
		if (rsp->message)
			errMsg_ = string(rsp->message);
	}
	else {
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

