// dummy_async_client.h
// Dummy implementation of mqtt::iasync_client for Unit Test.

/*******************************************************************************
 * Copyright (c) 2016 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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
 *    Guilherme M. Ferreira - initial implementation
 *******************************************************************************/

#ifndef __mqtt_dummy_async_client_h
#define __mqtt_dummy_async_client_h

#include <vector>

#include "mqtt/iasync_client.h"
#include "mqtt/token.h"
#include "mqtt/connect_options.h"

namespace mqtt {
namespace test {

/////////////////////////////////////////////////////////////////////////////

class dummy_async_client : public mqtt::iasync_client
{
public:
	void remove_token(mqtt::token* tok) override {}

	mqtt::token_ptr connect() override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr connect(mqtt::connect_options options) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr connect(mqtt::connect_options options, void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr connect(void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr reconnect() override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr disconnect() override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr disconnect(disconnect_options) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr disconnect(int timeout) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr disconnect(int timeout, void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr disconnect(void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::token_ptr{};
	}

	mqtt::delivery_token_ptr get_pending_delivery_token(int msgID) const override {
		return mqtt::delivery_token_ptr{};
	}

	std::vector<mqtt::delivery_token_ptr> get_pending_delivery_tokens() const override {
		return std::vector<mqtt::delivery_token_ptr>{};
	};

	std::string get_client_id() const override {
		return std::string{""};
	};

	std::string get_server_uri() const override {
		return std::string{""};
	};

	bool is_connected() const override {
		return true;
	};

	mqtt::delivery_token_ptr publish(string_ref topic,
									 const void* payload, size_t n,
									 int qos, bool retained) override {
		auto msg = mqtt::message::create(topic, payload, n, qos, retained);
		return publish(msg);
	};

	mqtt::delivery_token_ptr publish(string_ref topic,
									 const void* payload, size_t n) override {
		auto msg = mqtt::message::create(topic, payload, n);
		return publish(msg);
	};

	mqtt::delivery_token_ptr publish(string_ref topic, binary_ref payload,
									 int qos, bool retained) override {
		auto msg = mqtt::message::create(topic, payload, qos, retained);
		return publish(msg);
	};

	mqtt::delivery_token_ptr publish(string_ref topic, binary_ref payload) override {
		auto msg = mqtt::message::create(topic, payload);
		return publish(msg);
	};

	mqtt::delivery_token_ptr publish(string_ref topic,
									 const void* payload, size_t n,
									 int qos, bool retained, void* userContext,
									 mqtt::iaction_listener& cb) override {
		return mqtt::delivery_token_ptr{};
	}

	mqtt::delivery_token_ptr publish(mqtt::const_message_ptr msg) override {
		return mqtt::delivery_token::create(*this, msg);
	}

	mqtt::delivery_token_ptr publish(mqtt::const_message_ptr msg,
									 void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::delivery_token_ptr{};
	}

	void set_callback(mqtt::callback& cb) override {}
	void disable_callbacks() override {}

	mqtt::token_ptr subscribe(const_string_collection_ptr topicFilters,
							  const qos_collection& qos) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr subscribe(const_string_collection_ptr topicFilters,
							  const qos_collection& qos,
							  void* userContext, mqtt::iaction_listener& callback) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr subscribe(const string& topicFilter, int qos) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr subscribe(const string& topicFilter, int qos,
			void* userContext, mqtt::iaction_listener& callback) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr unsubscribe(const string& topicFilter) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr unsubscribe(const_string_collection_ptr topicFilters) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr unsubscribe(const_string_collection_ptr topicFilters,
								void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::token_ptr{};
	}

	mqtt::token_ptr unsubscribe(const string& topicFilter,
								void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::token_ptr{};
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace test
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_dummy_async_client_h
