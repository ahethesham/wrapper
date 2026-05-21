#!/bin/bash --
set -e

HOME=$(find ~/ -type d -name agent | head -n 1 )

LIB_DIRS=$(find "${HOME}"/include -type d -name libs )

LIBS=

for dir in ${LIB_DIRS} ;
do
    LIBS="${LIBS}:${dir}"
done

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LIBS}

BINARY=${HOME}/include/builder/http/bin/mock
CFG=${HOME}/config/agent_dev.cfg

${BINARY} "${CFG}"

