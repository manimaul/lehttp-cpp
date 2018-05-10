#pragma once

#include <string>

namespace lehttp {

    class XConfig {
    public:
        explicit XConfig(char const *ipAddress = "0.0.0.0",
                         uint16_t const port = 8080) : ipAddress(ipAddress), port(port) {}

        std::string const ipAddress;
        uint16_t const port;
    };
}
