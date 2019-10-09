// subscribe_options.cpp

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

#include "mqtt/subscribe_options.h"

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

#if __cplusplus < 201703L
	constexpr bool subscribe_options::SUBSCRIBE_NO_LOCAL;
	constexpr bool subscribe_options::SUBSCRIBE_LOCAL;

	constexpr bool subscribe_options::NO_RETAIN_AS_PUBLISHED;
	constexpr bool subscribe_options::RETAIN_AS_PUBLISHED;
#endif

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

