/////////////////////////////////////////////////////////////////////////////
/// @file exception.h
/// Declaration of MQTT exception class
/// @date May 1, 2013
/// @author Frank Pagliughi
/////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Copyright (c) 2013-2019 Frank Pagliughi <fpagliughi@mindspring.com>
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
#include "mqtt/types.h"
#include <vector>
#include <memory>
#include <exception>
#include <stdexcept>

namespace mqtt {

using bad_cast = std::bad_cast;

/////////////////////////////////////////////////////////////////////////////

/**
 * Base mqtt::exception.
 * This wraps the error codes which originate from the underlying C library.
 */
class exception : public std::runtime_error
{
protected:
	/** The error return code from the C library */
	int rc_;
	/** The reason code from the server */
	ReasonCode reasonCode_;
	/** The error message from the C library */
	string msg_;

public:
	/**
	 * Creates an MQTT exception.
	 * @param rc The error return code from the C library.
	 */
	explicit exception(int rc)
		: exception(rc, string_error(rc)) {}
	/**
	 * Creates an MQTT exception.
	 * @param rc The error return code from the C library.
	 * @param msg The text message for the error.
	 */
	exception(int rc, const string& msg)
		: std::runtime_error(printable_error(rc, msg)),
			rc_(rc), reasonCode_(ReasonCode::SUCCESS), msg_(msg) {}
	/**
	 * Creates an MQTT exception.
	 * @param rc The error return code from the C library.
	 * @param reasonCode The reason code from the server
	 * @param msg The text message for the error.
	 */
	exception(int rc, ReasonCode reasonCode, const string& msg)
		: std::runtime_error(printable_error(rc, msg)),
			rc_(rc), reasonCode_(reasonCode), msg_(msg) {}
	/**
	 * Gets an error message from an error code.
	 * @param rc The error code from the C lib
	 * @return A string explanation of the error
	 */
	static string string_error(int rc) {
		const char *msg = ::MQTTAsync_strerror(rc);
		return msg ? string(msg) : string();
	}
	/**
	 * Gets a detailed error message for an error code.
	 * @param rc The error code from the C lib
	 * @param msg An optional additional message. If none is provided, the
	 *  		  string_error message is used.
	 * @return A string error message that includes the error code and an
	 *  	   explanation message.
	 */
	static string printable_error(int rc, const string& msg=string()) {
		string s = "MQTT error [" + std::to_string(rc) + "]: ";
		if (msg.empty())
			s += string_error(rc);
		else
			s += msg;
		return s;
	}
	/**
	 * Returns the return code for this exception.
	 */
	int get_return_code() const { return rc_; }
	/**
	 * Returns the reason code for this exception.
	 */
	int get_reason_code() const { return reasonCode_; }
	/**
	 * Returns the error message for this exception.
	 */
	string get_message() const { return msg_; }
	/**
	 * Gets a string representation of this exception.
	 * @return A string representation of this exception.
	 */
	string to_string() const { return string(what()); }
};

/////////////////////////////////////////////////////////////////////////////

class missing_response : public std::runtime_error
{
public:
	missing_response(const string& rsp)
		: std::runtime_error("Missing "+rsp+" response") {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * This exception is thrown by the implementor of the persistence interface
 * if there is a problem reading or writing persistent data.
 */
class persistence_exception : public exception
{
public:
	/**
	 * Creates an MQTT persistence exception.
	 */
	persistence_exception() : exception(MQTTCLIENT_PERSISTENCE_ERROR) {}
	/**
	 * Creates an MQTT persistence exception.
	 * @param code The error code from the C library.
	 */
	explicit persistence_exception(int code) : exception(code) {}
	/**
	 * Creates an MQTT persistence exception.
	 * @param msg The text message for the error.
	 */
	explicit persistence_exception(const string& msg)
				: exception(MQTTCLIENT_PERSISTENCE_ERROR, msg) {}
	/**
	 * Creates an MQTT persistence exception.
	 * @param code The error code
	 * @param msg The text message for the error.
	 */
	persistence_exception(int code, const string& msg)
				: exception(code, msg) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Thrown when a client is not authorized to perform an operation, or if
 * there is a problem with the security configuration.
 */
class security_exception : public exception
{
public:
	/**
	 * Creates an MQTT security exception
	 * @param code The error code.
	 */
	explicit security_exception(int code) : exception(code) {}
	/**
	 * Creates an MQTT security exception
	 * @param code The error code.
	 * @param msg The text message for the error.
	 */
	security_exception(int code, const string& msg) : exception(code, msg) {}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_token_h
