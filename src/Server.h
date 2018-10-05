#pragma once

#include <functional>
#include "Config.h"
#include "HttpMethod.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

namespace lehttp {

    typedef std::function<HttpResponse(HttpRequest &request)> Handler;

    class Server {

    public:
        Server();

        explicit Server(Config config);

        virtual ~Server();

        void listenAndServe();

        Server &addRoute(std::string const &routePattern,
                         HttpMethod const &method,
                         Handler handler);

    private:
        Config const config;
        struct event_base *evBase = nullptr;
        struct evhttp *http = nullptr;
        struct evhttp_bound_socket *httpBoundSocket = nullptr;
    };

}
