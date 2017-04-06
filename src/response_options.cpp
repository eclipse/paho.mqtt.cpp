// response_options.cpp

#include "mqtt/response_options.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

response_options::response_options()
		: opts_(MQTTAsync_responseOptions_initializer)
{
	opts_.onSuccess = &token::on_success;
	opts_.onFailure = &token::on_failure;
}

response_options::response_options(const token_ptr& tok)
		: response_options()
{
	set_token(tok);
}

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

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

