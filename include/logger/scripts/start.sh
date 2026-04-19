#!/bin/bash

HOME="/home/ahethesham/agent"

[ $( ls ${HOME}/logs/ | wc -l ) -gt 0 ] && rm -f ${HOME}/logs/* || echo "Log directory empty"

LINKERS="${HOME}/include/stream/libs/:${HOME}/include/Logger/libs"

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LINKERS}

${HOME}/include/Logger/bin/mock ${HOME}/config/agent-dev.cfg
