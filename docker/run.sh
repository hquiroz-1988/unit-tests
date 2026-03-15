#!/bin/bash

TAG=hq/cpputest-runner

HOST_WORKDIR=${PWD}
WORKDIR=/home
COMMAND=${1:-/bin/bash}

sudo docker run \
  --user $(id -u):$(id -g) \
  --rm \
  --name cpputest-runner \
  --volume "${HOST_WORKDIR}":"${WORKDIR}" \
  --workdir "${WORKDIR}" \
  $TAG \
  $COMMAND

