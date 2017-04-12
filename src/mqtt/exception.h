/////////////////////////////////////////////////////////////////////////////
/// @file exception.h
/// Declaration of MQTT exception class
/// @date May 1, 2013
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

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

#ifndef __mqtt_exception_h
#define __mqtt_exception_h

#include "MQTTAsync.h"
#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <stdexcept>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Base mqtt::exception.
 * This wraps the error codes which originate from the underlying C library.
 */
class exception : public std::runtime_error
{
	/** The error code from the C library */
	int code_;
	std::string msg_;

public:
	explicit exception(int reasonCode) : std::runtime_error("mqtt::exception"),
											code_(reasonCode),
											msg_("MQTT exception ") {
		msg_ += std::to_string(code_);

	}
	/**
	 * Returns the detail message for this exception.
	 */
	std::string get_message() const { return std::string(what()); }
	/**
	 * Returns the reason code for this exception.
	 */
	int get_reason_code() const { return code_; }
	/**
	 * Gets a string representation of this exception.
	 * @return A string representation of this exception.
	 */
	std::string to_str() const { return get_message(); }
	/**
	 * Returns an explanatory string for the exception.
	 * @return const char*
	 */
	const char* what() const noexcept override {
		return msg_.c_str();
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * This exception is thrown by the implementor of the persistence interface
 * if there is a problem reading or writing persistent data.
 */
class persistence_exception : public exception
{
public:
	// TODO: Define "reason codes"
	persistence_exception() : exception(MQTTCLIENT_PERSISTENCE_ERROR) {}
	persistence_exception(int reasonCode) : exception(reasonCode) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Thrown when a client is not authorized to perform an operation, or if
   there is a problem with the security configuration.
 */
class security_exception : public exception
{
public:
	security_exception(int reasonCode) : exception(reasonCode) {}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_token_h
