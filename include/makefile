ifndef __ALPHA_MAKEFILE__
__ALPHA_MAKEFILE__ := 1

AGENT_HOME:=/home/ahethesham/agent

include ${AGENT_HOME}/include/json/makefile
include ${AGENT_HOME}/include/logger/makefile

LIB_FILE:=${AGENT_HOME}/include/libs/libalpha.so

.PHONY: clean_all app

all: json_clean stream_clean logger_clean json_all logger_all app
	${CXX} -std=c++23 ${OBJS} ${INCLUDES} ${LINKERS} -o ${LIB_FILE}
	@echo "compiled and generated ${LIB_FILE} successfully "

app: 
	$(eval OBJS=$(shell find ${AGENT_HOME}/include -type f -name *.o))

clean:
	rm -f ./libs/*

endif
