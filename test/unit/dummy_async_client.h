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
#include "mqtt/qos.h"

namespace mqtt {
namespace test {

/////////////////////////////////////////////////////////////////////////////

class dummy_async_client : public mqtt::iasync_client
{
public:
	void remove_token(mqtt::itoken* tok) override {}

	mqtt::itoken_ptr connect() override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr connect(mqtt::connect_options options) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr connect(mqtt::connect_options options, void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr connect(void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr disconnect() override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr disconnect(long quiesceTimeout) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr disconnect(long quiesceTimeout, void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr disconnect(void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::idelivery_token_ptr get_pending_delivery_token(int msgID) const override {
		return mqtt::idelivery_token_ptr{};
	}

	std::vector<mqtt::idelivery_token_ptr> get_pending_delivery_tokens() const override {
		return std::vector<mqtt::idelivery_token_ptr>{};
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

	mqtt::idelivery_token_ptr publish(const std::string& topic, const void* payload,
			size_t n, mqtt::QoS qos, bool retained) override {
		auto msg = mqtt::make_message(payload, n, qos, retained);
		return publish(topic, msg);
	};

	mqtt::idelivery_token_ptr publish(const std::string& topic,
			const void* payload, size_t n,
			mqtt::QoS qos, bool retained, void* userContext,
			mqtt::iaction_listener& cb) override {
		return mqtt::idelivery_token_ptr{};
	}

	mqtt::idelivery_token_ptr publish(const std::string& topic, mqtt::const_message_ptr msg) override {
		return std::make_shared<mqtt::delivery_token>(*this, topic, msg);
	}

	mqtt::idelivery_token_ptr publish(const std::string& topic, mqtt::const_message_ptr msg,
			void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::idelivery_token_ptr{};
	}

	void set_callback(mqtt::callback& cb) override {}

	mqtt::itoken_ptr subscribe(const topic_filter_collection& topicFilters,
			const qos_collection& qos) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr subscribe(const topic_filter_collection& topicFilters,
			const qos_collection& qos,
			void* userContext, mqtt::iaction_listener& callback) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr subscribe(const std::string& topicFilter, mqtt::QoS qos) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr subscribe(const std::string& topicFilter, mqtt::QoS qos,
			void* userContext, mqtt::iaction_listener& callback) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr unsubscribe(const std::string& topicFilter) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr unsubscribe(const topic_filter_collection& topicFilters) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr unsubscribe(const topic_filter_collection& topicFilters,
			void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::itoken_ptr{};
	}

	mqtt::itoken_ptr unsubscribe(const std::string& topicFilter,
			void* userContext, mqtt::iaction_listener& cb) override {
		return mqtt::itoken_ptr{};
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace test
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_dummy_async_client_h
