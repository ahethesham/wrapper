#!/bin/bash


HOME="/home/ahethesham/agent/include"
LINKERS="${HOME}/stream/libs/:${HOME}/Logger/libs:${HOME}/config/libs"

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LINKERS}

${HOME}/config/bin/mock_config /home/ahethesham/agent/config/agent_dev.cfg
