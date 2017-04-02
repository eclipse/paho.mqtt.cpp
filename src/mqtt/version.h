/////////////////////////////////////////////////////////////////////////////
/// @file version.h
/// Declaration of MQTT version info
/// @date Dec 31, 2016
/// @author Luis Castedo
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
 *    L - initial implementation and documentation
 *******************************************************************************/

#ifndef __mqtt_version_h
#define __mqtt_version_h

#include <string>

namespace mqtt {

extern const uint32_t    VERSION;
extern const std::string VERSION_STR, COPYRIGHT;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif // __mqtt_version_h

