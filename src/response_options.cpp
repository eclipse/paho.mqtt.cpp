// response_options.cpp

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

