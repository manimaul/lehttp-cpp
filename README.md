# LEHTTP-CPP (WIP)

----------------

Example:
```cpp
auto config = XConfig("0.0.0.0", 8181);
Server(config)
    .addRoute("/", HttpMethod::Get, [](auto &request) {
        return HttpResponse {HttpStatus::Code::Ok, "Root Route"};
    })
    .addRoute("/demo", HttpMethod::Get, [](auto &request) {
        return HttpResponse {HttpStatus::Code::Ok, "Demo Route"};
    })
    .listenAndServe();
```

### Getting Started

Install Dependencies MacOS
```bash
brew install cmake
brew install libevent
```

Install Dependencies Debian Linux
```bash
todo
```

Build and Run
```bash
mkdir build && cd build
cmake ..
make
./lehttp
```

