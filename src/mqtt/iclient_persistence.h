/////////////////////////////////////////////////////////////////////////////
/// @file iclient_persistence.h 
/// Declaration of MQTT iclient_persistence interface
/// @date May 1, 2013 
/// @author Frank Pagliughi 
/////////////////////////////////////////////////////////////////////////////  

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

#ifndef __mqtt_iclient_persistence_h
#define __mqtt_iclient_persistence_h

extern "C" {
	#include "MQTTAsync.h"
}

#include "mqtt/ipersistable.h"
#include <string>
#include <memory>
#include <vector> 

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Represents a persistent data store, used to store outbound and inbound
 * messages while they are in flight, enabling delivery to the QoS
 * specified. You can specify an implementation of this interface using
 * client::client(string, string, iclient_persistence), which the
 * client will use to persist QoS 1 and 2 messages.
 *
 * If the methods defined throw the MqttPersistenceException then the state
 * of the data persisted should remain as prior to the method being called.
 * For example, if put(string, persistable) throws an exception at any
 * point then the data will be assumed to not be in the persistent store.
 * Similarly if remove(string) throws an exception then the data will be
 * assumed to still be held in the persistent store.
 *
 * It is up to the persistence interface to log any exceptions or error
 * information which may be required when diagnosing a persistence failure.
 */
class iclient_persistence
{
	friend class iasync_client;

public:

	/** C-callbacks  */
	static int persistence_open(void** handle, char* clientID, char* serverURI, void* context);
	static int persistence_close(void* handle); 
	static int persistence_put(void* handle, char* key, int bufcount, char* buffers[], int buflens[]);
	static int persistence_get(void* handle, char* key, char** buffer, int* buflen);
	static int persistence_remove(void* handle, char* key);
	static int persistence_keys(void* handle, char*** keys, int* nkeys);
	static int persistence_clear(void* handle);
	static int persistence_containskey(void* handle, char* key);

public:
	/**
	 * Smart/shared pointer to this class.
	 */
	typedef std::shared_ptr<iclient_persistence> ptr_t;
	/**
	 * Virtual destructor.
	 */
	virtual ~iclient_persistence() {}
	/** 
	 * Initialise the persistent store.
	 */          
	virtual void open(const std::string& clientId, const std::string& serverURI) =0;
	/**
	 * Close the persistent store that was previously opened.
	 */
	virtual void close() =0;
	/**
	 * Clears persistence, so that it no longer contains any persisted data.
	 */
	virtual void clear() =0;
	/**
	 * Returns whether or not data is persisted using the specified key. 
	 * @param key 
	 * @return bool 
	 */
	virtual bool contains_key(const std::string& key) =0;
	/**
	 * Gets the specified data out of the persistent store.
	 * @param key 
	 * @return persistable 
	 */
	virtual ipersistable_ptr get(const std::string& key) const =0;
	/**
	 * Returns an Enumeration over the keys in this persistent data store.
	 */
	virtual std::vector<std::string> keys() const =0;
	/**
	 * Puts the specified data into the persistent store.
	 * @param key 
	 * @param persistable 
	 */
	virtual void put(const std::string& key, ipersistable_ptr persistable) =0;
	/**
	 * Remove the data for the specified key. 
	 * @param key 
	 */
	virtual void remove(const std::string& key) =0;
};

/**
 * Shared pointer to a persistence client.
 */
typedef std::shared_ptr<iclient_persistence> iclient_persistence_ptr;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_iclient_persistence_h

