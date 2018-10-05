
#include <iostream>
#include "LEhttp.h"

using namespace lehttp;

int main() {
    auto config = Config("0.0.0.0", 8181);

    Server(config)
            .addRoute("/foo", HttpMethod::Get, [](auto &request) {
                return HttpResponse {HttpStatus::Code::Ok}
                        .setBody("hello path GET foo")
                        .addHeader("hi", "there");
            })
            .addRoute("/foo", HttpMethod::Post, [](auto &request) {
                return HttpResponse {HttpStatus::Code::Ok}
                        .setBody("hello path POST foo")
                        .addHeader("hi", "there");
            })
            .addRoute("/json", HttpMethod::Get, [](auto &request) {
                return HttpResponse {HttpStatus::Code::Ok}
                        .contentTypeJson()
                        .setBody("{}");
            })
            .listenAndServe();

    return EXIT_SUCCESS;
}