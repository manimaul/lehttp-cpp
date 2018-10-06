FROM debian:stretch-slim
MAINTAINER manimaul@gmail.com

## Cmake 3.12 and LibEvent
RUN apt-get update && \
    apt-get install -y build-essential git curl sudo libevent-dev && \
    cd /opt && \
    curl -Lo cmake.tar.gz https://cmake.org/files/v3.12/cmake-3.12.2-Linux-x86_64.tar.gz && \
    tar -xf cmake.tar.gz

ENV PATH="/opt/cmake-3.12.2-Linux-x86_64/bin/:$PATH"
