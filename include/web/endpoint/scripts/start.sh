#!/bin/bash --

AGENT_HOME=$(find ~/ -type d -name agent | head -n 1)


LIBS=$(find ${AGENT_HOME}/include -type f -name "*.so")

LIB_DIR=""

for lib in ${LIBS}
do
    LIB_DIR="${LIB_DIR}:$(dirname "${lib}")"
done

LD_LIBRARY_PATH=$LD_LIBRARY_PATH${LIB_DIR}

"${AGENT_HOME}"/include/web/endpoint/bin/mock


