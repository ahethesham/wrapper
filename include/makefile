ifndef __ALPHA_MAKEFILE__
__ALPHA_MAKEFILE__ := 1
HOME := $(shell find ~/ -type d -name agent | head -n 1 )

include ${HOME}/include/json/makefile
include ${HOME}/include/logger/makefile

LIB_FILE:=${HOME}/include/libs/libalpha.so

.PHONY: clean_all app

all: json_clean stream_clean logger_clean json_all logger_all app
	${CXX} -std=c++23 ${OBJS} ${INCLUDES} ${LINKERS} -o ${LIB_FILE}
	@echo "compiled and generated ${LIB_FILE} successfully "

app: 
	$(eval OBJS=$(shell find ${HOME}/include -type f -name *.o))

clean:
	rm -f ./libs/*

endif
