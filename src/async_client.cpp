//async_client.cpp

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

#include "mqtt/async_client.h"
#include "mqtt/token.h"
#include "mqtt/message.h"
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
	MQTTAsync_create(&cli_, const_cast<char*>(serverURI.c_str()),
					 const_cast<char*>(clientId.c_str()),
					 MQTTCLIENT_PERSISTENCE_DEFAULT, nullptr);
}

	
async_client::async_client(const std::string& serverURI, const std::string& clientId,
						   const std::string& persistDir)
				: serverURI_(serverURI), clientId_(clientId), 
					persist_(nullptr), userCallback_(nullptr)
{
	MQTTAsync_create(&cli_, const_cast<char*>(serverURI.c_str()),
					 const_cast<char*>(clientId.c_str()),
					 MQTTCLIENT_PERSISTENCE_DEFAULT, 
					 const_cast<char*>(persistDir.c_str()));
}

async_client::async_client(const std::string& serverURI, const std::string& clientId, 
						   iclient_persistence* persistence)
				: serverURI_(serverURI), clientId_(clientId), 
					persist_(nullptr), userCallback_(nullptr)
{
	if (!persistence) {
		MQTTAsync_create(&cli_, const_cast<char*>(serverURI.c_str()),
						 const_cast<char*>(clientId.c_str()),
						 MQTTCLIENT_PERSISTENCE_NONE, nullptr);
	}
	else {
		persist_ = new MQTTClient_persistence {
			persistence,
			(Persistence_open) &iclient_persistence::persistence_open,
			(Persistence_close) &iclient_persistence::persistence_close,
			(Persistence_put) &iclient_persistence::persistence_put,
			(Persistence_get) &iclient_persistence::persistence_get,
			(Persistence_remove) &iclient_persistence::persistence_remove,
			(Persistence_keys) &iclient_persistence::persistence_keys,
			(Persistence_clear) &iclient_persistence::persistence_clear,
			(Persistence_containskey) &iclient_persistence::persistence_containskey
		};

		MQTTAsync_create(&cli_, const_cast<char*>(serverURI.c_str()),
						 const_cast<char*>(clientId.c_str()),
						 MQTTCLIENT_PERSISTENCE_USER, persist_);
	}
}

async_client::~async_client()
{
	MQTTAsync_destroy(&cli_);
	delete persist_;
}

// --------------------------------------------------------------------------

void async_client::on_connection_lost(void *context, char *cause) 
{
	if (context) {
		async_client* m = static_cast<async_client*>(context);
		callback* cb = m->get_callback();
		if (cb)
			cb->connection_lost(cause ? std::string(cause) : std::string());
	}
}

int async_client::on_message_arrived(void* context, char* topicName, int topicLen, 
									 MQTTAsync_message* msg)
{
	if (context) {
		async_client* m = static_cast<async_client*>(context);
		callback* cb = m->get_callback();
		if (cb) {
			std::string topic(topicName, topicName+topicLen);
			message_ptr m = std::make_shared<message>(*msg);
			cb->message_arrived(topic, m);
		}
	}

    MQTTAsync_freeMessage(&msg);
    MQTTAsync_free(topicName);

	// TODO: Should the user code determine the return value?
	// The Java version does doesn't seem to...
	return (!0);
}

// Callback to indicate that a message was delivered to the server. It seems
// to only be called for a message with a QOS >= 1, but it happens before
// the on_success() call for the token. Thus we don't have the underlying
// MQTTAsync_token of the outgoing message at the time of this callback.
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
				message_ptr msg = dtok->get_message();
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

itoken_ptr async_client::connect() throw(exception, security_exception)
{
	connect_options opts;
	return connect(opts);
}

itoken_ptr async_client::connect(connect_options opts) throw(exception, security_exception)
{
	token* ctok = new token(*this);
	itoken_ptr tok = itoken_ptr(ctok);
	add_token(tok);

	opts.opts_.onSuccess = &token::on_success;
	opts.opts_.onFailure = &token::on_failure;
	opts.opts_.context = ctok;

	int rc = MQTTAsync_connect(cli_, &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::connect(connect_options opts, void* userContext, 
								 iaction_listener& cb) throw(exception, security_exception)
{
	token* ctok = new token(*this);
	itoken_ptr tok = itoken_ptr(ctok);
	add_token(tok);

	tok->set_user_context(userContext);
	tok->set_action_callback(cb);

	opts.opts_.onSuccess = &token::on_success;
	opts.opts_.onFailure = &token::on_failure;
	opts.opts_.context = ctok;

	int rc = MQTTAsync_connect(cli_, &opts.opts_);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::connect(void* userContext, iaction_listener& cb)
							 throw(exception, security_exception)
{
	connect_options opts;
	opts.opts_.keepAliveInterval = 30;
	opts.opts_.cleansession = 1;
	return connect(opts, userContext, cb);
}

itoken_ptr async_client::disconnect(long timeout) throw(exception)
{
	token* ctok = new token(*this);
	itoken_ptr tok = itoken_ptr(ctok);
	add_token(tok);

	MQTTAsync_disconnectOptions disconnOpts( MQTTAsync_disconnectOptions_initializer );

	// TODO: Check timeout range?
	disconnOpts.timeout = int(timeout);
	disconnOpts.onSuccess = &token::on_success;
	disconnOpts.onFailure = &token::on_failure;
	disconnOpts.context = ctok;

	int rc = MQTTAsync_disconnect(cli_, &disconnOpts);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::disconnect(long timeout, void* userContext, iaction_listener& cb)
							 throw(exception)
{
	token* ctok = new token(*this);
	itoken_ptr tok = itoken_ptr(ctok);
	add_token(tok);

	tok->set_user_context(userContext);
	tok->set_action_callback(cb);


	MQTTAsync_disconnectOptions disconnOpts( MQTTAsync_disconnectOptions_initializer );

	// TODO: Check timeout range?
	disconnOpts.timeout = int(timeout);
	disconnOpts.onSuccess = &token::on_success;
	disconnOpts.onFailure = &token::on_failure;
	disconnOpts.context = ctok;

	int rc = MQTTAsync_disconnect(cli_, &disconnOpts);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

idelivery_token_ptr async_client::get_pending_delivery_token(int msgID) const
{
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
	for (const auto& t : pendingDeliveryTokens_)
		toks.push_back(t);
	return toks;
}

idelivery_token_ptr async_client::publish(const std::string& topic, const void* payload, 
										  size_t n, int qos, bool retained)
						throw(exception)			 
{
	message_ptr msg = std::make_shared<message>(payload, n);
	msg->set_qos(qos);
	msg->set_retained(retained);

	return publish(topic, msg);
}

idelivery_token_ptr async_client::publish(const std::string& topic, 
										  const void* payload, size_t n,
										  int qos, bool retained, void* userContext, 
										  iaction_listener& cb)
						throw(exception)
{
	message_ptr msg = std::make_shared<message>(payload, n);
	msg->set_qos(qos);
	msg->set_retained(retained);

	return publish(topic, msg, userContext, cb);
}

idelivery_token_ptr async_client::publish(const std::string& topic, message_ptr msg)
						throw(exception)			 
{
	delivery_token* dtok = new delivery_token(*this, topic);
	idelivery_token_ptr tok = idelivery_token_ptr(dtok);

	dtok->set_message(msg);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &delivery_token::on_success;
	opts.onFailure = &delivery_token::on_failure;
	opts.context = dtok;

	int rc = MQTTAsync_sendMessage(cli_, (char*) topic.c_str(), &(msg->msg_), &opts);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

idelivery_token_ptr async_client::publish(const std::string& topic, message_ptr msg, 
										  void* userContext, iaction_listener& cb)
						throw(exception)			 
{
	delivery_token* dtok = new delivery_token(*this, topic);
	idelivery_token_ptr tok = idelivery_token_ptr(dtok);

	dtok->set_message(msg);
	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &delivery_token::on_success;
	opts.onFailure = &delivery_token::on_failure;
	opts.context = dtok;

	int rc = MQTTAsync_sendMessage(cli_, (char*) topic.c_str(), &(msg->msg_), &opts);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

void async_client::set_callback(callback& cb) throw(exception)
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

itoken_ptr async_client::subscribe(const topic_filter_collection& topicFilters, 
								   const qos_collection& qos)
						throw(std::invalid_argument,exception)
		   
{
	if (topicFilters.size() != qos.size())
		throw std::invalid_argument("Collection sizes don't match");

	std::vector<char*> filts = alloc_topic_filters(topicFilters);

	token* stok = new token(*this, topicFilters);
	itoken_ptr tok = itoken_ptr(stok);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &token::on_success;
	opts.onFailure = &token::on_failure;
	opts.context = stok;

	int rc = MQTTAsync_subscribeMany(cli_, (int) topicFilters.size(),
									 (char**) &filts[0], (int*) &qos[0], &opts);

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
						throw(std::invalid_argument,exception)
{
	if (topicFilters.size() != qos.size())
		throw std::invalid_argument("Collection sizes don't match");

	std::vector<char*> filts = alloc_topic_filters(topicFilters);

	// No exceptions till C-strings are deleted!

	token* stok = new token(*this, topicFilters);
	itoken_ptr tok = itoken_ptr(stok);

	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &token::on_success;
	opts.onFailure = &token::on_failure;
	opts.context = stok;

	int rc = MQTTAsync_subscribeMany(cli_, (int) topicFilters.size(),
									 (char**) &filts[0], (int*) &qos[0], &opts);

	free_topic_filters(filts);
	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::subscribe(const std::string& topicFilter, int qos)
						throw(exception)			 
{
	token* stok = new token(*this, topicFilter);
	itoken_ptr tok = itoken_ptr(stok);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &token::on_success;
	opts.onFailure = &token::on_failure;
	opts.context = stok;

	int rc = MQTTAsync_subscribe(cli_, (char*) topicFilter.c_str(), qos, &opts);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::subscribe(const std::string& topicFilter, int qos, 
								   void* userContext, iaction_listener& cb)
						throw(exception)			 
{
	token* stok = new token(*this, topicFilter);
	itoken_ptr tok = itoken_ptr(stok);

	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &token::on_success;
	opts.onFailure = &token::on_failure;
	opts.context = stok;

	int rc = MQTTAsync_subscribe(cli_, (char*) topicFilter.c_str(), qos, &opts);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::unsubscribe(const std::string& topicFilter)
						throw(exception)			 
{
	token* stok = new token(*this, topicFilter);
	itoken_ptr tok = itoken_ptr(stok);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &token::on_success;
	opts.onFailure = &token::on_failure;
	opts.context = stok;

	int rc = MQTTAsync_unsubscribe(cli_, (char*) topicFilter.c_str(), &opts);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::unsubscribe(const topic_filter_collection& topicFilters)
						throw(exception)			 
{
	size_t n = topicFilters.size();
	std::vector<char*> filts = alloc_topic_filters(topicFilters);

	token* stok = new token(*this, topicFilters);
	itoken_ptr tok = itoken_ptr(stok);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &token::on_success;
	opts.onFailure = &token::on_failure;
	opts.context = stok;

	int rc = MQTTAsync_unsubscribeMany(cli_, (int) n, (char**) &filts[0], &opts);

	free_topic_filters(filts);
	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::unsubscribe(const topic_filter_collection& topicFilters, 
									 void* userContext, iaction_listener& cb)
						throw(exception)			 
{
	size_t n = topicFilters.size();
	std::vector<char*> filts = alloc_topic_filters(topicFilters);

	token* stok = new token(*this, topicFilters);
	itoken_ptr tok = itoken_ptr(stok);

	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &token::on_success;
	opts.onFailure = &token::on_failure;
	opts.context = stok;

	int rc = MQTTAsync_unsubscribeMany(cli_, (int) n, (char**) &filts[0], &opts);

	free_topic_filters(filts);
	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

itoken_ptr async_client::unsubscribe(const std::string& topicFilter, 
									 void* userContext, iaction_listener& cb)
						throw(exception)				
{
	token* stok = new token(*this, topicFilter);
	itoken_ptr tok = itoken_ptr(stok);

	tok->set_user_context(userContext);
	tok->set_action_callback(cb);
	add_token(tok);

	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	opts.onSuccess = &token::on_success;
	opts.onFailure = &token::on_failure;
	opts.context = stok;

	int rc = MQTTAsync_unsubscribe(cli_, (char*) topicFilter.c_str(), &opts);

	if (rc != MQTTASYNC_SUCCESS) {
		remove_token(tok);
		throw exception(rc);
	}

	return tok;
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

