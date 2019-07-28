// dummy_callback.h
// Dummy implementation of mqtt::callback for Unit Test.

/*******************************************************************************
 * Copyright (c) 2017 Guilherme M. Ferreira <guilherme.maciel.ferreira@gmail.com>
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

#ifndef __mqtt_dummy_callback_h
#define __mqtt_dummy_callback_h

#include "mqtt/callback.h"

namespace mqtt {
namespace test {

/////////////////////////////////////////////////////////////////////////////

class dummy_callback : public mqtt::callback
{
public:
	bool connection_lost_called { false };
	bool message_arrived_called { false };
	bool delivery_complete_called { false };

	void connection_lost(const std::string& cause) override {
		connection_lost_called = true;
	}

	void message_arrived(mqtt::const_message_ptr msg) override {
		message_arrived_called = true;
	}

	void delivery_complete(mqtt::delivery_token_ptr tok) override {
		delivery_complete_called = true;
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace test
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_dummy_callback_h
