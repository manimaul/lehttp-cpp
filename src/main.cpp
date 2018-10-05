
#include <iostream>
#include "LEhttp.h"

using namespace lehttp;

int main() {
    auto config = Config("0.0.0.0", 8181);

    Server(config)
            .addRoute("/", HttpMethod::Get, [](auto &request) {
                return HttpResponse {HttpStatus::Code::Ok}
                        .setBody("Root")
                        .addHeader("X_My_Header", "HeaderValue")
                        .addHeader("X_My_Other_Header", "OtherHeaderValue");
            })
            .addRoute("/demo", HttpMethod::Get, [](auto &request) {
                return HttpResponse {HttpStatus::Code::Ok}
                        .setBody("Demo");
            })
            .addRoute("/json", HttpMethod::Get, [](auto &request) {
                return HttpResponse {HttpStatus::Code::Ok}
                        .contentTypeJson()
                        .setBody("{}");
            })
            .listenAndServe();

    return EXIT_SUCCESS;
}