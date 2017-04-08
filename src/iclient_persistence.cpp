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

// This is an internal class for wrapping a buffer into a persistable type.
// Note that it does not copy the buffer or take possession of it, and thus
// is only useful for a subset of circumstances where the buffer is
// guaranteed to live longer than the wrapper object, and performance is
// important.

class persistence_wrapper : virtual public ipersistable
{
	const byte_buffer hdr_;
	const byte_buffer payload_;

public:
	persistence_wrapper(const void* payload, size_t payloadlen) 
			: payload_(static_cast<const byte*>(payload), 
					   static_cast<const byte*>(payload) + payloadlen) {}
	persistence_wrapper(const byte_buffer& payload)
			: payload_(payload) {}
	persistence_wrapper(const void* hdr, size_t hdrlen,
						const void* payload, size_t payloadlen)
			: hdr_(static_cast<const byte*>(hdr), 
				   static_cast<const byte*>(hdr) + hdrlen),
				payload_(static_cast<const byte*>(payload), 
						 static_cast<const byte*>(payload) + payloadlen) {}
	persistence_wrapper(const byte_buffer& hdr, const byte_buffer& payload)
			: hdr_(hdr), payload_(payload) {}

	const byte* get_header_bytes() const override { return hdr_.data(); }
	size_t get_header_length() const override { return hdr_.size(); }
	size_t get_header_offset() const override { return 0; }

	const byte* get_payload_bytes() const override { return payload_.data(); }
	size_t get_payload_length() const override { return payload_.size(); }
	size_t get_payload_offset() const override { return 0; }

	std::vector<byte> get_header_byte_arr() const override {
		return std::vector<byte>(hdr_.data(), hdr_.data()+hdr_.size());
	}
	std::vector<byte> get_payload_byte_arr() const override {
		return std::vector<byte>(payload_.data(), payload_.data()+payload_.size());
	}
};

/////////////////////////////////////////////////////////////////////////////
// Functions to transition C persistence calls to the C++ persistence object.

// Upon the call to persistence_open(), the 'context' has the address of the
// C++ persistence object, which is reassigned to the 'handle'. Subsequent
// calls have the object address as the handle.

int iclient_persistence::persistence_open(void** handle, const char* clientID, 
										  const char* serverURI, void* context)
{
	try {
		if (context) {
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
		if (handle && bufcount > 0) {
			ipersistable_ptr p;
			if (bufcount == 1)
				p = std::make_shared<persistence_wrapper>(buffers[0], buflens[0]);
			else if (bufcount == 2)
				p = std::make_shared<persistence_wrapper>(buffers[0], buflens[0],
														  buffers[1], buflens[1]);
			else {
				std::string buf;
				for (int i=0; i<bufcount; ++i) {
					if (buffers[i] && buflens[i] > 0)
						buf.append(buffers[i], buflens[i]);
				}
				if (buf.empty())	// No data!
					return MQTTCLIENT_PERSISTENCE_ERROR;
				p = std::make_shared<persistence_wrapper>(&buf[0], buf.size());
			}
			static_cast<iclient_persistence*>(handle)->put(key, p);
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
		if (handle) {
			ipersistable_ptr p = static_cast<iclient_persistence*>(handle)->get(key);

			size_t	hdrlen = p->get_header_length(),
					payloadlen = p->get_payload_length();

			if (!p->get_header_bytes()) hdrlen = 0;
			if (!p->get_payload_bytes()) payloadlen = 0;

			// TODO: Check range
			*buflen = static_cast<int>(hdrlen + payloadlen);
			char* buf = static_cast<char*>(malloc(*buflen));
			std::memcpy(buf, p->get_header_bytes(), hdrlen);
			std::memcpy(buf+hdrlen, p->get_payload_bytes(), payloadlen);
			*buffer = buf;
			return MQTTASYNC_SUCCESS;
		}
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

int iclient_persistence::persistence_remove(void* handle, char* key)
{
	try {
		if (handle) {
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
			std::vector<std::string> k(
				static_cast<iclient_persistence*>(handle)->keys());
			size_t n = k.size();
			*nkeys = n;		// TODO: Check range
			if (n == 0)
				*keys = nullptr;
			else {
				*keys = static_cast<char**>(malloc(n*sizeof(char*)));
				for (size_t i=0; i<n; ++i) {
					size_t len = k[i].size();
					(*keys)[i] = static_cast<char*>(malloc(len+1));
					std::memcpy((*keys)[i], k[i].data(), len);
					(*keys)[i][len] = '\0';
				}
			}
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
		if (handle &&
				static_cast<iclient_persistence*>(handle)->contains_key(key))
			return MQTTASYNC_SUCCESS;
	}
	catch (...) {}

	return MQTTCLIENT_PERSISTENCE_ERROR;
}

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

