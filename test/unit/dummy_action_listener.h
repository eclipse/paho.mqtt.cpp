// dummy_action_listener.h
// Dummy implementation of mqtt::iaction_listener for Unit Test.

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

#ifndef __mqtt_dummy_action_listener_h
#define __mqtt_dummy_action_listener_h

#include "mqtt/token.h"

namespace mqtt {
namespace test {

/////////////////////////////////////////////////////////////////////////////

class dummy_action_listener : public mqtt::iaction_listener
{
public:
	bool on_success_called { false };
	bool on_failure_called { false };

	void on_success(const mqtt::token& token) override {
		on_success_called = true;
	}

	void on_failure(const mqtt::token& token) override {
		on_failure_called = true;
	}

};

/////////////////////////////////////////////////////////////////////////////
// end namespace test
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif //  __mqtt_dummy_action_listener_h
