#pragma once

#include <utility>
#include <vector>
#include <unordered_map>
#include "HttpStatus.h"

namespace lehttp {
    class HttpResponse {
    public:
        explicit HttpResponse(HttpStatus::Code const status) : status(status) {
            headers = {};
            body = "";
        }

        HttpStatus::Code const status;

        std::unordered_map<std::string, std::string> const &getHeaders() {
            return headers;
        }

        std::string const &getBody() {
            return body;
        }

        HttpResponse &setBody(std::string const &body) {
            HttpResponse::body = body;
            return *this;
        }

        HttpResponse &addHeader(std::string const &key, std::string const &value) {
            headers.emplace(key, value);
            return *this;
        }

        HttpResponse &contentTypeJson() {
            return addHeader("Content-Type", "application/json");
        }

    private:
        std::string body;
        std::unordered_map<std::string, std::string> headers;
    };
}
