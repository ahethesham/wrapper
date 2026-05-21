#!/bin/bash

HOME=$( find ~ -type d -name agent | head -n 1 )

LIBS=${HOME}/include/web/request/libs:${HOME}/include/json/libs:${HOME}/include/formatter/libs/:${HOME}/include/json/libs:${HOME}/include/stream/libs:${HOME}/include/logger/libs:${HOME}/include/config/libs:${HOME}/include/token/libs

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${LIBS}

${HOME}/include/web/request/bin/mock ${HOME}/config/agent_dev.cfg


