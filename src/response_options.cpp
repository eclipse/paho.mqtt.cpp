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
// end namespace 'mqtt'
}
