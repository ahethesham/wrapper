#!/bin/bash

AGENT_HOME=/home/ahethesham/agent

LIBS=""

while read lib
do
    LIBS="${LIBS}:$(dirname "${lib}")"
done < <(find ${AGENT_HOME}/include -type f -name "*.so")

LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${LIBS}"

BINARY=${AGENT_HOME}/include/connection/bin/mock

CFG="/home/ahethesham/agent/config/agent_dev.cfg"
${BINARY} ${CFG}

