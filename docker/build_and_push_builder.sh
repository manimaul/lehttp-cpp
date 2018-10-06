#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

pushd $DIR/..

DOCKER_TAG=manimaul/libevent-builder:stretch

docker build -f ./docker/builder/Dockerfile -t $DOCKER_TAG .
docker push $DOCKER_TAG

popd