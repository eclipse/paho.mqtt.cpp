// topic.cpp

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


#include "mqtt/topic.h"
#include "mqtt/async_client.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

idelivery_token_ptr topic::publish(const void* payload, size_t n, 
								   int qos, bool retained)
{
	return cli_->publish(name_, payload, n, qos, retained);
}

idelivery_token_ptr topic::publish(message_ptr msg)
{
	return cli_->publish(name_, msg);
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}



