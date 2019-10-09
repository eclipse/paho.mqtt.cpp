// response_options.cpp

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

#include "mqtt/response_options.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

response_options::response_options(int mqttVersion /*=MQTTVERSION_DEFAULT*/)
		: opts_(MQTTAsync_responseOptions_initializer)
{
	if (mqttVersion < MQTTVERSION_5) {
		opts_.onSuccess  = &token::on_success;
		opts_.onFailure  = &token::on_failure;
	}
	else {
		opts_.onSuccess5 = &token::on_success5;
		opts_.onFailure5 = &token::on_failure5;
	}
}

response_options::response_options(const token_ptr& tok,
								   int mqttVersion /*=MQTTVERSION_DEFAULT*/)
		: response_options(mqttVersion)
{
	set_token(tok);
}

void response_options::set_token(const token_ptr& tok)
{
	tok_ = tok;
	opts_.context = tok.get();
}

void response_options::set_subscribe_options(const subscribe_options& opts)
{
	opts_.subscribeOptions = opts.opts_;
}

void response_options::set_subscribe_options(const std::vector<subscribe_options>& opts)
{
	subOpts_.clear();
	for (const auto& opt : opts)
		subOpts_.push_back(opt.opts_);

	opts_.subscribeOptionsCount = int(opts.size());
	opts_.subscribeOptionsList = const_cast<MQTTSubscribe_options*>(subOpts_.data());
}

/////////////////////////////////////////////////////////////////////////////

delivery_response_options::delivery_response_options(int mqttVersion /*=MQTTVERSION_DEFAULT*/)
		: opts_(MQTTAsync_responseOptions_initializer)
{
	if (mqttVersion < MQTTVERSION_5) {
		opts_.onSuccess  = &delivery_token::on_success;
		opts_.onFailure  = &delivery_token::on_failure;
	}
	else {
		opts_.onSuccess5 = &delivery_token::on_success5;
		opts_.onFailure5 = &delivery_token::on_failure5;
	}
}

delivery_response_options::delivery_response_options(const delivery_token_ptr& tok,
													 int mqttVersion /*=MQTTVERSION_DEFAULT*/)
		: delivery_response_options(mqttVersion)
{
	set_token(tok);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

