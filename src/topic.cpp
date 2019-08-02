// topic.cpp

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

#include "mqtt/topic.h"
#include "mqtt/async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

delivery_token_ptr topic::publish(const void* payload, size_t n)
{
	return cli_.publish(name_, payload, n, qos_, retained_);
}

delivery_token_ptr topic::publish(const void* payload, size_t n,
								  int qos, bool retained)
{
	return cli_.publish(name_, payload, n, qos, retained);
}

delivery_token_ptr topic::publish(binary_ref payload)
{
	return cli_.publish(name_, std::move(payload), qos_, retained_);
}

delivery_token_ptr topic::publish(binary_ref payload, int qos, bool retained)
{
	return cli_.publish(name_, std::move(payload), qos, retained);
}

token_ptr topic::subscribe(const subscribe_options& opts)
{
	return cli_.subscribe(name_, qos_, opts);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}



