#include <evdns.h>
#include <iostream>
#include <evhttp.h>
#include "LEhttp.h"

using namespace lehttp;

Server::Server() : Server(XConfig()) {}

Server::Server(XConfig cfg) : config(std::move(cfg)) {
    evBase = event_base_new();
    if (!evBase) {
        std::cout << "Could not create and event_base - exiting" << std::endl;
        throw;
    }

    http = evhttp_new(evBase);
    if (!http) {
        std::cout << "Could not create and evhttp - exiting" << std::endl;
        throw;
    }
}

void handleRequest(struct evhttp_request *req, void *) {
    struct evbuffer *evb = evbuffer_new();
    evbuffer_add_printf(evb, "<!DOCTYPE html><html><body>Hello lehttp-cpp http server</body></html>");
    evhttp_send_reply(req, 200, "OK", evb);
}

void handleRequestR(struct evhttp_request *req, void *arg) {
    auto handler = *reinterpret_cast<Handler *>(arg);
    auto request = HttpRequest();
    auto response = handler(request);
    struct evbuffer *evb = evbuffer_new();
    //todo: stream response with evbuffer
    evbuffer_add_printf(evb, "%s", response.getBody().c_str());
    evhttp_send_reply(req, response.status, HttpStatus::reasonPhrase(response.status).c_str(), evb);
}

Server &Server::addRoute(std::string const &routePattern,
                         HttpMethod const &method,
                         Handler handler) {
    evhttp_set_cb(http, routePattern.c_str(), handleRequestR, (void *) &handler);
    return *this;
}

void Server::listenAndServe() {
    evhttp_set_gencb(http, handleRequest, nullptr);

    httpBoundSocket = evhttp_bind_socket_with_handle(http, "0.0.0.0", config.port);
    if (!httpBoundSocket) {
        std::cout << "Could not bind to port: " << config.port << " - exiting" << std::endl;
        throw;
    }

    std::cout << "LEHTTP -- " << LEHTTP_VERSION << " serving" << std::endl;

    event_base_dispatch(evBase);
}

Server::~Server() {
    event_base_free(evBase);
    evhttp_free(http);
}
