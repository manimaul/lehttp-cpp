#pragma once

#include <functional>
#include <memory>
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
                         Handler const &handler);

    private:
        Config const config;
        struct event_base *evBase = nullptr;
        struct evhttp *http = nullptr;
        struct evhttp_bound_socket *httpBoundSocket = nullptr;
        std::unordered_map<std::string, std::shared_ptr<std::unordered_map<HttpMethod, Handler>>> handlers;

        static HttpMethod getMethod(struct evhttp_request *req);

        static void handleRequest(struct evhttp_request *req, void *);

        static void handleNotFound(struct evhttp_request *req, void *);
    };

}
