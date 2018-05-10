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

        HttpResponse &setBody(const std::string &body) {
            HttpResponse::body = body;
            return *this;
        }

        HttpResponse &addHeader(std::string key, std::string value) {
            auto header = std::pair<std::string, std::string>(std::move(key), std::move(value));
            headers.insert(header);
            return *this;
        }

    private:
        std::string body;
        std::unordered_map<std::string, std::string> headers;
    };
}
