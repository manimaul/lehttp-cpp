#include <evdns.h>
#include <iostream>
#include <memory>
#include <evhttp.h>
#include "LEhttp.h"
#include "Server.h"


using namespace lehttp;

HttpMethod getMethod(struct evhttp_request *req) {
    switch (evhttp_request_get_command(req)) {
        case EVHTTP_REQ_GET: return HttpMethod::Get;
        case EVHTTP_REQ_POST: return HttpMethod::Post;
        case EVHTTP_REQ_HEAD: return HttpMethod::Head;
        case EVHTTP_REQ_PUT: return HttpMethod::Put;
        case EVHTTP_REQ_DELETE: return HttpMethod::Delete;
        case EVHTTP_REQ_OPTIONS: return HttpMethod::Options;
        case EVHTTP_REQ_TRACE: return HttpMethod::Trace;
        case EVHTTP_REQ_CONNECT: return HttpMethod::Connect;
        case EVHTTP_REQ_PATCH: return HttpMethod::Patch;
        default: return HttpMethod::Get;
    }
}

void handleNotFound(struct evhttp_request *req, void *) {
    std::string uri = evhttp_request_get_uri(req);

    struct evbuffer *evb = evbuffer_new();
    evhttp_send_reply(req, HttpStatus::NotFound, HttpStatus::reasonPhrase(HttpStatus::NotFound).c_str(), evb);
    evbuffer_free(evb);
}

void handleRequest(struct evhttp_request *req, void *arg) {
    auto pathHandlers = reinterpret_cast<std::unordered_map<HttpMethod, Handler> *>(arg);
    auto method = getMethod(req);
    auto got = pathHandlers->find(method);
    if (got == pathHandlers->end()) {
        handleNotFound(req, arg);
    } else {
        auto request = HttpRequest(method, req);
        auto handler = got->second;
        auto response = handler(request);
        struct evbuffer *evb = evbuffer_new();
        //todo: stream response with evbuffer
        evbuffer_add_printf(evb, "%s", response.getBody().c_str());
        struct evkeyvalq *headers = evhttp_request_get_output_headers(req);
        auto h = response.getHeaders();
        for (auto const &pair : h) {
            evhttp_add_header(headers, pair.first.c_str(), pair.second.c_str());
        }
        evhttp_send_reply(req, response.status, HttpStatus::reasonPhrase(response.status).c_str(), evb);
        evbuffer_free(evb);
    }
}

Server::Server() : Server(Config()) {}

Server::Server(Config cfg) : config(std::move(cfg)), handlers() {
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

Server &Server::addRoute(std::string const &routePattern,
                         HttpMethod const &method,
                         Handler const &handler) {
    auto got = handlers.find(routePattern);
    if (got == handlers.end()) {
        auto map = std::make_shared<std::unordered_map<HttpMethod, Handler>>();
        map->emplace(method, handler);
        handlers.emplace(routePattern, map);
        evhttp_set_cb(http, routePattern.c_str(), handleRequest, (void *) map.get());
    } else {
        auto map = got->second;
        auto pair = map->emplace(method, handler);
        evhttp_set_cb(http, routePattern.c_str(), handleRequest, (void *) map.get());
    }
    return *this;
}

void Server::listenAndServe() {
    evhttp_set_gencb(http, handleNotFound, nullptr);
    evhttp_set_max_headers_size(http, config.maxContentLength);
    evhttp_set_max_body_size(http, config.maxContentLength);
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
