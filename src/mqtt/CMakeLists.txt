#*******************************************************************************
#  Copyright (c) 2016
# 
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the Eclipse Public License v1.0
#  and Eclipse Distribution License v1.0 which accompany this distribution. 
# 
#  The Eclipse Public License is available at 
#     http://www.eclipse.org/legal/epl-v10.html
#  and the Eclipse Distribution License is available at 
#    http://www.eclipse.org/org/documents/edl-v10.php.
# 
#  Contributors:
#     Guilherme Maciel Ferreira - initial version
#*******************************************************************************/

## Note: on OS X you should install XCode and the associated command-line tools

## install headers
set(COMMON_HDR
    async_client.h
    buffer_ref.h
    buffer_view.h
    callback.h
    client.h
    connect_options.h
    delivery_token.h
    disconnect_options.h
    exception.h
    iaction_listener.h
    iasync_client.h
    iclient_persistence.h
    message.h
    response_options.h
    string_collection.h
    thread_queue.h
    token.h
    topic.h
    types.h
    will_options.h)

if(PAHO_WITH_SSL)
    set(COMMON_HDR
        ${COMMON_HDR}
        ssl_options.h)
endif()

install(
    FILES ${COMMON_HDR}
    DESTINATION include/mqtt
)
