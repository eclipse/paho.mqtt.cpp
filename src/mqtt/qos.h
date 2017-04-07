/////////////////////////////////////////////////////////////////////////////
/// @file qos.h
/// Declaration of MQTT QoS class
/// @date Jul 8, 2016
/// @author Guilherme Maciel Ferreira
/////////////////////////////////////////////////////////////////////////////

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
 *    Guilherme M. Ferreira - initial implementation and documentation
 ******************************************************************************/

#ifndef __mqtt_qos_h
#define __mqtt_qos_h

#include <ostream>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Quality of Service values.
 *
 * One of the following delivery values:
 *
 * - "At most once", where messages are delivered according to the best efforts
 *   of the underlying TCP/IP network. Message loss or duplication can occur.
 *   This level can be used in systems where it does not matter if an individual
 *   message is lost as the next one will be published soon after.
 *
 * - "At least once", where messages are assured to arrive at least once, but
 *   duplicates may occur.
 *
 * - "Exactly once", where message are assured to arrive exactly once. This
 *   level must be used in systems where duplicate or lost messages can lead
 *   to incorrect results.
 *
 */
enum class QoS : int
{
	QOS0 = 0,	// Fire and Forget
	QOS1 = 1,	// Acknowledged delivery
	QOS2 = 2,	// Assured delivery, exactly once
};

/**
 * Determines if the QOS value is a valid one.
 * @param qos The QOS value.
 * @throw std::invalid_argument If the qos value is invalid.
 */
void validate_qos(QoS qos);

/**
 * Determines if the QOS value is a valid one.
 * @param qos The QOS value.
 * @throw std::invalid_argument If the qos value is invalid.
 */
void validate_qos(int qos);

/**
 * Insertion operator for mqtt::QoS
 * @param os Output stream
 * @param qos The QoS to print
 * @return The output stream passed as argument
 */
std::ostream& operator<<(std::ostream& os, QoS qos);

/**
 * Comparison operator
 * @param qosa The first QoS
 * @param qosb The second QoS
 * @return true if both QoSes have the same numeric value
 */
bool operator==(QoS qosa, QoS qosb);

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_qos_h

