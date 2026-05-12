#!/bin/bash -- 

#compile the client code
HOME="/home/ahethesham/agent"

CLIENT_FILE=${HOME}/client/client.cc

INCLUDES="-I${HOME}/include/json/ \
        -I${HOME}/include/logger/ \
        -I${HOME} "

LIBS="-L${HOME}/include/json/libs/ -ljson_parser    \
        -L${HOME}/include/logger/libs/ -llogger "


g++ -std=c++23 ${CLIENT_FILE}  ${INCLUDES} ${LIBS} -o ${HOME}/bin/client


