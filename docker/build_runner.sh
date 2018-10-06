#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

pushd $DIR/..

DOCKER_TAG=manimaul/lehttp:git-$(git rev-parse --short=8 HEAD)
echo $DOCKER_TAG

docker build -f docker/runner/Dockerfile -t $DOCKER_TAG .
echo "just built tag:"
echo $DOCKER_TAG

popd