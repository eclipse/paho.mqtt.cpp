// delivery_response_options.cpp

#include "mqtt/delivery_response_options.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

delivery_response_options::delivery_response_options()
		: opts_(MQTTAsync_responseOptions_initializer)
{
	opts_.onSuccess = &delivery_token::on_success;
	opts_.onFailure = &delivery_token::on_failure;
}

delivery_response_options::delivery_response_options(const delivery_token_ptr& tok)
		: delivery_response_options()
{
	set_token(tok);
}

void delivery_response_options::update_message_id()
{
	if (opts_.context) {
		delivery_token* dtok { reinterpret_cast<delivery_token*>(opts_.context) };
		token* tok { dynamic_cast<token*>(dtok) };
		if (tok) {
			tok->tok_ = opts_.token;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}
