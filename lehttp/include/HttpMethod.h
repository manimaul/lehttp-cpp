#pragma once

#include <string>

namespace lehttp {
    enum HttpMethod {
        Options,
        Get,
        Head,
        Post,
        Put,
        Patch,
        Delete,
        Trace,
        Connect
    };

    static std::string httpMethodString(HttpMethod const &method) {
        switch (method) {
            case Options:
                return "OPTIONS";
            case Get:
                return "GET";
            case Head:
                return "HEAD";
            case Post:
                return "POST";
            case Put:
                return "PUT";
            case Patch:
                return "PATCH";
            case Delete:
                return "DELETE";
            case Trace:
                return "TRACE";
            case Connect:
                return "CONNECT";
            default:
                return "";
        }
    }
}
