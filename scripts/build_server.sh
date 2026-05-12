#!/bin/bash -- 

#compile the client code
HOME="/home/ahethesham/agent"

SERVER_FILE=${HOME}/app/controller/server.cc

INCLUDES="-I${HOME}/include/json/ \
        -I${HOME}/include/formatter/ \
        -I${HOME}/include/logger/ \
        -I${HOME}/include/token/ \
        -I${HOME}/include/stream/ \
        -I${HOME} "

LIBS="-L${HOME}/include/json/libs/ -ljson_parser    \
    -L${HOME}/include/formatter/libs -lformatter \
    -L${HOME}/include/token/libs -ltokenizer \
        -L${HOME}/include/logger/libs/ -llogger "


g++ -std=c++23 ${SERVER_FILE}  ${INCLUDES} ${LIBS} -o ${HOME}/bin/server
