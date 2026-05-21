#!/bin/bash
HOME=/home/ahethesham/agent

[ $( ls ${HOME}/logs/ | wc -l ) -gt 0 ] && rm -f ${HOME}/logs/* || echo "Log directory empty"

LIBS=${HOME}/include/config/libs:${HOME}/include/json/libs:${HOME}/include/stream/libs:${HOME}/include/logger/libs:${HOME}/include/token/libs:${HOME}/include/formatter/libs/

LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBS}

BINARY="${HOME}/include/json/bin/${1}"
CONFIG=${HOME}/config/agent_dev.cfg

$BINARY $CONFIG
