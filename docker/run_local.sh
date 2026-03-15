#!/bin/bash

TAG=hq/cpputest-runner

HOST_WORKDIR=${PWD}
WORKDIR=/home
COMMAND=${1:-/bin/bash}

sudo docker run \
  --rm \
  --user $(id -u):$(id -g) \
  --name cpputest-runner \
  --volume "${HOST_WORKDIR}":"${WORKDIR}" \
  --workdir "${WORKDIR}" \
  -it $TAG \
  $COMMAND

