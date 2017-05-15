// disconnect_options.cpp

#include "mqtt/disconnect_options.h"
#include <utility>
#include <cstring>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

const MQTTAsync_disconnectOptions disconnect_options::DFLT_C_STRUCT = MQTTAsync_disconnectOptions_initializer;

disconnect_options::disconnect_options() : opts_(DFLT_C_STRUCT)
{
}

disconnect_options::disconnect_options(int timeout, const token_ptr& tok)
		: disconnect_options()
{
	set_timeout(timeout);
	set_token(tok);
}

disconnect_options::disconnect_options(const disconnect_options& opt)
			: opts_(opt.opts_), tok_(opt.tok_)
{
}

disconnect_options::disconnect_options(disconnect_options&& opt)
			: opts_(opt.opts_), tok_(std::move(opt.tok_))
{
}

disconnect_options& disconnect_options::operator=(const disconnect_options& opt)
{
	opts_ = opt.opts_;
	tok_ = opt.tok_;
	return *this;
}

disconnect_options& disconnect_options::operator=(disconnect_options&& opt)
{
	opts_ = opt.opts_;
	tok_ = std::move(opt.tok_);
	return *this;
}

void disconnect_options::set_token(const token_ptr& tok)
{
	tok_ = tok;
	opts_.context = tok_.get();

	if (tok) {
		opts_.onSuccess = &token::on_success;
		opts_.onFailure = &token::on_failure;
	}
	else {
		opts_.onSuccess = nullptr;
		opts_.onFailure = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

