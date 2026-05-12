#!/bin/bash

HOME=$(find ~/ -type d -name agent | head -n 1 )
LIBS=$(find ${HOME} -type f -name "*.so")
LIB_DIRS=""
for file in ${LIBS}
do
LIB_DIRS="${LIB_DIRS}:$(dirname ${file})"
done
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIB_DIRS
BIN=$HOME/include/web/response/bin/mock
${BIN}
