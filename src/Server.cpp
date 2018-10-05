#include <evdns.h>
#include <iostream>
#include <memory>
#include <evhttp.h>
#include "LEhttp.h"
#include "Server.h"


using namespace lehttp;

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

    notFoundHandler = std::make_shared<Handler>([](auto &request) {
        return HttpResponse {HttpStatus::Code::NotFound};
    });
}


HttpMethod Server::getMethod(struct evhttp_request *req) {
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

std::shared_ptr<Handler> Server::getHandler(struct evhttp_request *req) {
    std::string uri = evhttp_request_get_uri(req);
    std::string sig = uri + "_" + httpMethodString(getMethod(req));
    auto got = handlers.find(sig);
    if (got == handlers.end()) {
        return notFoundHandler;
    } else {
        return got->second;
    }
}

void Server::handleRequest(struct evhttp_request *req, void *thiz) {
    auto server = reinterpret_cast<Server *>(thiz);
    auto handler = *server->getHandler(req);

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
                         Handler const &handler) {
    std::string sig = routePattern + "_" + httpMethodString(method);
    handlers.emplace(sig, std::make_shared<Handler>(handler));
    return *this;
}

void Server::listenAndServe() {
    evhttp_set_gencb(http, handleRequest, this);

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
