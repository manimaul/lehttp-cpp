# LEHTTP-CPP (WIP)

----------------

[![CircleCI](https://circleci.com/gh/manimaul/lehttp-cpp.svg?style=svg)](https://circleci.com/gh/manimaul/lehttp-cpp)


Example:
```cpp
auto config = Config("0.0.0.0", 8181);
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

```bash 
git submodule init --update
```

Install Dependencies MacOS
```bash
brew install cmake
brew install libevent
```

Install Dependencies Debian Linux
```bash
sudo apt-get update && \
sudo apt-get install -y build-essential curl libevent-dev && \
pushd /opt && \
curl -Lo cmake.tar.gz https://cmake.org/files/v3.12/cmake-3.12.2-Linux-x86_64.tar.gz && \
tar -xf cmake.tar.gz && \
rm cmake.tar.gz
popd

export PATH="/opt/cmake-3.12.2-Linux-x86_64/bin/:$PATH"
```

Build And Install
```bash
mkdir -p build && \
pushd build && \
cmake .. && \
make && \
sudo make install && \
popd
```

Build And Run Unit Tests
```bash 
mkdir -p build && \
pushd build && \
GTEST=1 cmake .. && \
make && \
./runUnitTests && \
popd
```

