// async_client.cpp

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

#include "mqtt/async_client.h"
#include "mqtt/token.h"
#include "mqtt/message.h"
#include "mqtt/response_options.h"
#include "mqtt/disconnect_options.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <cstring>
#include <cstdio>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

async_client::async_client(const std::string& serverURI, const std::string& clientId)
				: serverURI_(serverURI), clientId_(clientId),
					persist_(nullptr), userCallback_(nullptr)
{
	MQTTAsync_create(&cli_, serverURI.c_str(), clientId.c_str(),
					 MQTTCLIENT_PERSISTENCE_DEFAULT, nullptr);
}


async_client::async_client(const std::string& serverURI, const std::string& clientId,
						   const std::string& persistDir)
				: serverURI_(serverURI), clientId_(clientId),
					persist_(nullptr), userCallback_(nullptr)
{
	MQTTAsync_create(&cli_, serverURI.c_str(), clientId.c_str(),
					 MQTTCLIENT_PERSISTENCE_DEFAULT, const_cast<char*>(persistDir.c_str()));
}

async_client::async_client(const std::string& serverURI, const std::string& clientId,
						   iclient_persistence* persistence)
				: serverURI_(serverURI), clientId_(clientId),
					persist_(nullptr), userCallback_(nullptr)
{
	if (!persistence) {
		MQTTAsync_create(&cli_, serverURI.c_str(), clientId.c_str(),
						 MQTTCLIENT_PERSISTENCE_NONE, nullptr);
	}
	else {
		persist_ = new MQTTClient_persistence {
			persistence,
			&iclient_persistence::persistence_open,
			&iclient_persistence::persistence_close,
			&iclient_persistence::persistence_put,
			&iclient_persistence::persistence_get,
			&iclient_persistence::persistence_remove,
			&iclient_persistence::persistence_keys,
			&iclient_persistence::persistence_clear,
			&iclient_persistence::persistence_containskey
		};

		MQTTAsync_create(&cli_, serverURI.c_str(), clientId.c_str(),
						 MQTTCLIENT_PERSISTENCE_USER, persist_);
	}
}

async_client::~async_client()
{
	MQTTAsync_destroy(&cli_);
	delete persist_;
}

// --------------------------------------------------------------------------
// Class static callbacks.
// These are the callbacks directly from the C-lib. In each case the
// 'context' should be the address of the async_client object that
// registered the callback.

void async_client::on_connection_lost(void *context, char *cause)
{
	if (context) {
		async_client* cli = static_cast<async_client*>(context);
		callback* cb = cli->get_callback();
		if (cb)
			cb->connection_lost(cause ? std::string(cause) : std::string());
	}
}

int async_client::on_message_arrived(void* context, char* topicName, int topicLen,
									 MQTTAsync_message* msg)
{
	if (context) {
		async_client* cli = static_cast<async_client*>(context);
		callback* cb = cli->get_callback();
		if (cb) {
			std::string topic(topicName, topicName+topicLen);
			const_message_ptr m = std::make_shared<message>(*msg);
			cb->message_arrived(topic, m);
		}
	}

	MQTTAsync_freeMessage(&msg);
	MQTTAsync_free(topicName);

	// TODO: Should the user code determine the return value?
	// The Java version does doesn't seem to...
	return (!0);
}

// Callback to indicate that a message was delivered to the server.
// It is called for a message with a QOS >= 1, but it happens before the
// on_success() call for the token. Thus we don't have the underlying
// MQTTAsync_token of the outgoing message at the time of this callback.
//
// *** So using the Async C library we have no way to match this msgID with
//     a delivery_token object. So this is useless to us.
//
// So, all in all, this callback in it's current implementation seems rather
// redundant.
//
#if 0
void async_client::on_delivery_complete(void* context, MQTTAsync_token msgID)
{
	if (context) {
		async_client* m = static_cast<async_client*>(context);
		callback* cb = m->get_callback();
		if (cb) {
			idelivery_token_ptr tok = m->get_pending_delivery_token(msgID);
			cb->delivery_complete(tok);
		}
	}
}
#endif

// --------------------------------------------------------------------------
// Private methods

void async_client::add_token(itoken_ptr tok)
{
	if (tok) {
		guard g(lock_);
		pendingTokens_.push_back(tok);
	}
}

void async_client::add_token(idelivery_token_ptr tok)
{
	if (tok) {
	   guard g(lock_);
	   pendingDeliveryTokens_.push_back(tok);
	}
}

// Note that we uniquely identify a token by the address of its raw pointer,
// since the message ID is not unique.

void async_client::remove_token(itoken* tok)
{
	if (!tok)
		return;

	guard g(lock_);
	for (auto p=pendingDeliveryTokens_.begin();
					p!=pendingDeliveryTokens_.end(); ++p) {
		if (p->get() == tok) {
			idelivery_token_ptr dtok = *p;
			pendingDeliveryTokens_.erase(p);

			// If there's a user callback registered, we can now call
			// delivery_complete()

			if (userCallback_) {
				const_message_ptr msg = dtok->get_message();
				if (msg && msg->get_qos() > 0) {
					callback* cb = userCallback_;
					g.unlock();
					cb->delivery_complete(dtok);
				}
			}
			return;
		}
	}
	for (auto p=pendingTokens_.begin(); p!=pendingTokens_.end(); ++p) {
		if (p->get() == tok) {
			pendingTokens_.erase(p);
			return;
		}
	}
}

std::vector<char*> async_client::alloc_topic_filters(
							const topic_filter_collection& topicFilters)
{
	std::vector<char*> filts;
	for (const auto& t : topicFilters) {
		char* filt = new char[t.size()+1];
		std::strcpy(filt, t.c_str());
		filts.push_back(filt);
	}
	return filts;
}

void async_client::free_topic_filters(std::vector<char*>& filts)
{
	for (const auto& f : filts)
		delete[] f;
}

// --------------------------------------------------------------------------
// Connect

itoken_ptr async_client::connect()
{ 
	return connect(connect_options());
}

itoken_ptr async_client::connect(connect_options opts)
{
	itoken_ptr tok = std::make_shared<token>(*this);
	add_token(tok);

	opts.set_token(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_connect(cli_, &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::connect(connect_options opts, void* userContext,
								 iaction_listener& cb)
{
	itoken_ptr tok = std::make_shared<token>(*this);
	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	opts.set_token(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_connect(cli_, &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::connect(void* userContext, iaction_listener& cb)
{
	connect_options opts;
	opts.opts_.keepAliveInterval = 30;
	opts.opts_.cleansession = 1;
	return connect(opts, userContext, cb);
}

// --------------------------------------------------------------------------
// Disconnect

itoken_ptr async_client::disconnect(long timeout)
{
	itoken_ptr tok = std::make_shared<token>(*this);
	add_token(tok);

	// TODO may truncate timeout
	disconnect_options opts(static_cast<int>(timeout), dynamic_cast<token*>(tok.get()));

	int rc = MQTTAsync_disconnect(cli_, &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::disconnect(long timeout, void* userContext, iaction_listener& cb)
{
	itoken_ptr tok = std::make_shared<token>(*this);
	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	// TODO may truncate timeout
	disconnect_options opts(static_cast<int>(timeout), dynamic_cast<token*>(tok.get()));

	int rc = MQTTAsync_disconnect(cli_, &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

// --------------------------------------------------------------------------
// Queries

idelivery_token_ptr async_client::get_pending_delivery_token(int msgID) const
{
	// Messages with QOS=1 or QOS=2 that require a response/acknowledge should
	// have a non-zero 16-bit message ID. The library keeps the token objects
	// for all of these messages that are in flight. When the acknowledge comes
	// back from the broker, the C++ library can look up the token from the
	// msgID and signal it, indicating completion.

	if (msgID > 0) {
		guard g(lock_);
		for (const auto& t : pendingDeliveryTokens_) {
			if (t->get_message_id() == msgID)
				return t;
		}
	}
	return idelivery_token_ptr();
}

std::vector<idelivery_token_ptr> async_client::get_pending_delivery_tokens() const
{
	std::vector<idelivery_token_ptr> toks;
	guard g(lock_);
	for (const auto& t : pendingDeliveryTokens_) {
		if (t->get_message_id() > 0) {
			toks.push_back(t);
		}
	}
	return toks;
}

// --------------------------------------------------------------------------
// Publish

idelivery_token_ptr async_client::publish(const std::string& topic, const void* payload,
										  size_t n, int qos, bool retained)
{
	auto msg = make_message(payload, n, qos, retained);
	return publish(topic, msg);
}

idelivery_token_ptr async_client::publish(const std::string& topic,
										  const void* payload, size_t n,
										  int qos, bool retained, void* userContext,
										  iaction_listener& cb)
{
	auto msg = make_message(payload, n, qos, retained);
	return publish(topic, msg, userContext, cb);
}

idelivery_token_ptr async_client::publish(const std::string& topic, const_message_ptr msg)
{
	idelivery_token_ptr tok = std::make_shared<delivery_token>(*this, topic, msg);
	add_token(tok);

	auto dtok = std::dynamic_pointer_cast<delivery_token>(tok);
	dtok->set_message(msg);

	delivery_response_options opts(dtok);

	int rc = MQTTAsync_sendMessage(cli_, topic.c_str(), &(msg->msg_),
								   &opts.opts_);

	if (rc == MQTTASYNC_SUCCESS) {
		dtok->set_message_id(opts.opts_.token);
	}
	else {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

idelivery_token_ptr async_client::publish(const std::string& topic, const_message_ptr msg,
										  void* userContext, iaction_listener& cb)
{
	idelivery_token_ptr tok = std::make_shared<delivery_token>(*this, topic, msg);
	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	auto dtok = std::dynamic_pointer_cast<delivery_token>(tok);
	dtok->set_message(msg);

	delivery_response_options opts(dtok);

	int rc = MQTTAsync_sendMessage(cli_, topic.c_str(), &(msg->msg_),
								   &opts.opts_);

	if (rc == MQTTASYNC_SUCCESS) {
		dtok->set_message_id(opts.opts_.token);
	}
	else {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

// --------------------------------------------------------------------------

void async_client::set_callback(callback& cb)
{
	guard g(lock_);
	userCallback_ = &cb;

	int rc = MQTTAsync_setCallbacks(cli_, this,
									&async_client::on_connection_lost,
									&async_client::on_message_arrived,
									nullptr /*&async_client::on_delivery_complete*/);

	if (rc != MQTTASYNC_SUCCESS)
		throw exception(rc);
}

// --------------------------------------------------------------------------
// Subscribe

itoken_ptr async_client::subscribe(const topic_filter_collection& topicFilters,
								   const qos_collection& qos)

{
	if (topicFilters.size() != qos.size())
		throw std::invalid_argument("Collection sizes don't match");

	std::vector<char*> filts = alloc_topic_filters(topicFilters);

	itoken_ptr tok = std::make_shared<token>(*this, topicFilters);
	add_token(tok);

	response_options opts(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_subscribeMany(cli_, static_cast<int>(topicFilters.size()),
									 static_cast<char**>(&filts[0]),
									 const_cast<int*>(&qos[0]), &opts.opts_);

	free_topic_filters(filts);
	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::subscribe(const topic_filter_collection& topicFilters,
								   const qos_collection& qos,
								   void* userContext, iaction_listener& cb)
{
	if (topicFilters.size() != qos.size())
		throw std::invalid_argument("Collection sizes don't match");

	std::vector<char*> filts = alloc_topic_filters(topicFilters);

	// No exceptions till C-strings are deleted!

	itoken_ptr tok = std::make_shared<token>(*this, topicFilters);
	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	response_options opts(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_subscribeMany(cli_, static_cast<int>(topicFilters.size()),
									 static_cast<char**>(&filts[0]),
									 const_cast<int*>(&qos[0]), &opts.opts_);

	free_topic_filters(filts);
	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::subscribe(const std::string& topicFilter, int qos)
{
	itoken_ptr tok = std::make_shared<token>(*this, topicFilter);
	add_token(tok);

	response_options opts(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_subscribe(cli_, topicFilter.c_str(), qos, &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::subscribe(const std::string& topicFilter, int qos,
								   void* userContext, iaction_listener& cb)
{
	itoken_ptr tok = std::make_shared<token>(*this, topicFilter);
	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	response_options opts(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_subscribe(cli_, topicFilter.c_str(), qos, &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

// --------------------------------------------------------------------------
// Unsubscribe

itoken_ptr async_client::unsubscribe(const std::string& topicFilter)
{
	itoken_ptr tok = std::make_shared<token>(*this, topicFilter);
	add_token(tok);

	response_options opts(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_unsubscribe(cli_, topicFilter.c_str(), &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::unsubscribe(const topic_filter_collection& topicFilters)
{
	size_t n = topicFilters.size();
	std::vector<char*> filts = alloc_topic_filters(topicFilters);

	itoken_ptr tok = std::make_shared<token>(*this, topicFilters);
	add_token(tok);

	response_options opts(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_unsubscribeMany(cli_, static_cast<int>(n), static_cast<char**>(&filts[0]), &opts.opts_);

	free_topic_filters(filts);
	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::unsubscribe(const topic_filter_collection& topicFilters,
									 void* userContext, iaction_listener& cb)
{
	size_t n = topicFilters.size();
	std::vector<char*> filts = alloc_topic_filters(topicFilters);

	itoken_ptr tok = std::make_shared<token>(*this, topicFilters);
	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	response_options opts(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_unsubscribeMany(cli_, static_cast<int>(n), static_cast<char**>(&filts[0]), &opts.opts_);

	free_topic_filters(filts);
	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::unsubscribe(const std::string& topicFilter,
									 void* userContext, iaction_listener& cb)
{
	itoken_ptr tok = std::make_shared<token>(*this, topicFilter);
	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	response_options opts(std::dynamic_pointer_cast<token>(tok));

	int rc = MQTTAsync_unsubscribe(cli_, topicFilter.c_str(), &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

