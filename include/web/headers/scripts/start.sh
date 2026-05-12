#!/bin/bash --
HOME=$(find ~/ -type d -name agent | head -n 1 )
SO_LIBS=$(find $HOME/include -type f -name "*.so")

LIBS=""

for file in  ${SO_LIBS}
do
    LIBS="${LIBS}:$(dirname $file)"
done

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIBS

BINARY=$HOME/include/web/headers/bin/mock
CFG=$HOME/config/agent-dev.cfg

$BINARY $CFG
