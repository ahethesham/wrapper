#ifndef __WEB_FWD_H__
#define __WEB_FWD_H__

#include "http_request.h"
#include "http_response.h"
#include "tcp_socket.h"
typedef ClientFactory<
            tcp_resolver,
            tcp,
            http_connector,
            http_stream ,
            http::http_request ,
            http::http_response
        > httpClient;

#endif
