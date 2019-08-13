#*******************************************************************************
#  Copyright (c) 2016-2018
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
#     Frank Pagliughi
#*******************************************************************************/

install(
    FILES
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
				properties.h
        response_options.h
				server_response.h
        ssl_options.h
        string_collection.h
				subscribe_options.h
        thread_queue.h
        token.h
        topic.h
        types.h
        will_options.h
    DESTINATION 
        include/mqtt
)

