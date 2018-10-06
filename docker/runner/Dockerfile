FROM  manimaul/libevent-builder:stretch as builder
MAINTAINER manimaul@gmail.com

COPY CMakeLists.txt /build/CMakeLists.txt
COPY src/ /build/src/
COPY cmake/ /build/cmake/
#COPY resources/ build/resources
RUN cd build && \
    mkdir _build && \
    cd _build && \
    cmake .. && \
    make -j $(nproc)

#### End Builder

FROM debian:stretch-slim
RUN apt-get update && \
    apt-get install -y libevent-2.0-5 libevent-openssl-2.0-5 libevent-extra-2.0-5 libevent-pthreads-2.0-5
#    libevent-openssl
COPY --from=builder /build.old /usr/local/bin/
CMD lehttp
