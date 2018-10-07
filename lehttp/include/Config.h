#pragma once

#include <string>

namespace lehttp {

    class Config {
    public:
        explicit Config(char const *ipAddress = "0.0.0.0",
                        uint16_t const port = 8080,
                        size_t const maxContentLength = 1000 * 1000) : ipAddress(ipAddress), port(port),
                                                                       maxContentLength(maxContentLength) {}

        std::string const ipAddress;
        uint16_t const port;
        size_t const maxContentLength;
    };
}
