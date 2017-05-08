// iclient_persistence.cpp

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

#include "mqtt/types.h"
#include "mqtt/iclient_persistence.h"
#include <cstring>
#include <cstdlib>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////
// Functions to transition C persistence calls to the C++ persistence object.

// Upon the call to persistence_open(), the 'context' has the address of the
// C++ persistence object, which is reassigned to the 'handle'. Subsequent
// calls have the object address as the handle.

int iclient_persistence::persistence_open(void** handle, const char* clientID, 
										  const char* serverURI, void* context)
{
	try {
		if (handle && clientID && serverURI && context) {
			static_cast<iclient_persistence*>(context)->open(clientID, serverURI);
			*handle = context;
			return MQTTASYNC_SUCCESS;
		}
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

int iclient_persistence::persistence_close(void* handle)
{
	try {
		if (handle) {
			static_cast<iclient_persistence*>(handle)->close();
			return MQTTASYNC_SUCCESS;
		}
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

int iclient_persistence::persistence_put(void* handle, char* key, int bufcount, 
										 char* buffers[], int buflens[])
{
	try {
		if (handle && bufcount > 0 && buffers && buflens) {
			std::vector<string_view> vec;
			for (int i=0; i<bufcount; ++i)
				vec.push_back(string_view(buffers[i], buflens[i]));
			static_cast<iclient_persistence*>(handle)->put(key, vec);
			return MQTTASYNC_SUCCESS;
		}
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

int iclient_persistence::persistence_get(void* handle, char* key, 
										 char** buffer, int* buflen)
{
	try {
		if (handle && key && buffer && buflen) {
			auto sv = static_cast<iclient_persistence*>(handle)->get(key);
			*buffer = const_cast<char*>(sv.data());
			*buflen = (int) sv.length();
			return MQTTASYNC_SUCCESS;
		}
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

int iclient_persistence::persistence_remove(void* handle, char* key)
{
	try {
		if (handle && key) {
			static_cast<iclient_persistence*>(handle)->remove(key);
			return MQTTASYNC_SUCCESS;
		}
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

int iclient_persistence::persistence_keys(void* handle, char*** keys, int* nkeys)
{
	try {
		if (handle && keys && nkeys) {
			auto& k = static_cast<iclient_persistence*>(handle)->keys();
			size_t n = k.size();
			*nkeys = n;
			*keys = (n == 0) ? nullptr : const_cast<char**>(k.c_arr());
			return MQTTASYNC_SUCCESS;
		}
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

int iclient_persistence::persistence_clear(void* handle)
{
	try {
		if (handle) {
			static_cast<iclient_persistence*>(handle)->clear();
			return MQTTASYNC_SUCCESS;
		}
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

int iclient_persistence::persistence_containskey(void* handle, char* key)
{
	try {
		if (handle && key &&
				static_cast<iclient_persistence*>(handle)->contains_key(key))
			return MQTTASYNC_SUCCESS;
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

