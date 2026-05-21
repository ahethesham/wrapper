#!/bin/bash --

LIBS=""

while read -r file
do
    LIBS="${LIBS}:$(dirname $file)"
done < <(find ~/agent/include -type f -name "*.so") 

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${LIBS}

BINARY_FILE="/home/ahethesham/agent/include/stream/bin/mock"

$BINARY_FILE

