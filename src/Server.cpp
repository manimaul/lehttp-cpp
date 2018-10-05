#include <evdns.h>
#include <iostream>
#include <evhttp.h>
#include "LEhttp.h"

using namespace lehttp;

Server::Server() : Server(Config()) {}

Server::Server(Config cfg) : config(std::move(cfg)) {
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

void handleNotFound(struct evhttp_request *req, void *) {
    struct evbuffer *evb = evbuffer_new();
    evhttp_send_reply(req, HttpStatus::NotFound, HttpStatus::reasonPhrase(HttpStatus::NotFound).c_str(), evb);
}

void handleRequest(struct evhttp_request *req, void *arg) {
    auto handler = *reinterpret_cast<Handler *>(arg);
    auto request = HttpRequest();
    auto response = handler(request);
    struct evbuffer *evb = evbuffer_new();
    //todo: stream response with evbuffer
    evbuffer_add_printf(evb, "%s", response.getBody().c_str());
    struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
    for (auto const &pair : response.getHeaders()) {
        evhttp_add_header(headers, pair.first.c_str(), pair.second.c_str());
    }
    evhttp_send_reply(req, response.status, HttpStatus::reasonPhrase(response.status).c_str(), evb);
}

Server &Server::addRoute(std::string const &routePattern,
                         HttpMethod const &method,
                         Handler handler) {
    evhttp_set_cb(http, routePattern.c_str(), handleRequest, (void *) &handler);
    return *this;
}

void Server::listenAndServe() {
    evhttp_set_gencb(http, handleNotFound, nullptr);

    httpBoundSocket = evhttp_bind_socket_with_handle(http, config.ipAddress.c_str(), config.port);
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
