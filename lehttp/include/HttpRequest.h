#pragma once

#include <evhttp.h>
#include <event2/keyvalq_struct.h>
#include "LEhttp.h"
#include "HttpResponse.h"

namespace lehttp {
    class HttpRequest {
    public:

        explicit HttpRequest(HttpMethod method, evhttp_request *req) : method(method), req(req) {}

        virtual ~HttpRequest() {
            if (evhttp_request_is_owned(req) == 1) {
                evhttp_request_free(req);
            }
        }

        std::unordered_map<std::string, std::string> headers() {
            struct evkeyval *kv = evhttp_request_get_input_headers(req)->tqh_first;
            std::unordered_map<std::string, std::string> headers = {};
            while (kv != nullptr) {
                headers.emplace(kv->key, kv->value);
                kv = kv->next.tqe_next;
            }
            return headers;
        }

        std::string header(std::string const &key) {
            struct evkeyvalq *headers = evhttp_request_get_input_headers(req);
            auto h = evhttp_find_header(headers, key.c_str());
            if (h == nullptr) {
                return "";
            } else {
                return std::string(h);
            }
        }

        std::string queryParam(std::string const &key) {
            auto uri = evhttp_request_get_evhttp_uri(req);
            auto uriChars = evhttp_uri_get_query(uri);
            struct evkeyvalq kvq = {};
            evhttp_parse_query_str(uriChars, &kvq);
            auto h = evhttp_find_header(&kvq, key.c_str());
            if (h == nullptr) {
                return "";
            } else {
                return std::string(h);
            }
        }

        std::unordered_map<std::string, std::string> queryParams() {
            auto uri = evhttp_request_get_evhttp_uri(req);
            auto uriChars = evhttp_uri_get_query(uri);
            struct evkeyvalq kvq = {};
            evhttp_parse_query_str(uriChars, &kvq);
            struct evkeyval *kv = kvq.tqh_first;
            std::unordered_map<std::string, std::string> params = {};
            while (kv != nullptr) {
                params.emplace(kv->key, kv->value);
                kv = kv->next.tqe_next;
            }
            return params;
        }

        std::string const &getBody() {
            if (body.length() == 0) {
                struct evbuffer *buf = evhttp_request_get_input_buffer(req);
                size_t size = evbuffer_get_length(buf);
                char* data = nullptr;
                data = static_cast<char *>(alloca(size + 1));
                data[size] = 0;
                evbuffer_copyout(buf, data, size);
                body = std::string(data);
            }
            return body;
        }

        HttpRequest &setBody(std::string const &body) {
            throw;
            //todo: (WK)
            //return *this;
        }

        HttpRequest &addHeader(std::string const &key, std::string const &value) {
            auto headers = evhttp_request_get_input_headers(req);
            evhttp_add_header(headers, key.c_str(), value.c_str());
            return *this;
        }

        HttpMethod getMethod() const {
            return method;
        }

    private:
        struct evhttp_request *req;
        HttpMethod method;
        std::string body = {};
    };
}
