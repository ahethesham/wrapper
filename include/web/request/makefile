include ../../formatter/makefile
include ../../json/makefile

ifndef HOME
	HOME=$(shell find ~/ -name agent -type d | head -n 1 )
endif

PWD=${HOME}/include/web/request

OBJS=${PWD}/build/http_headers_v1.o \
	 ${PWD}/build/http_request_line_v1.o \
	 ${PWD}/build/http_request_v1.o

ifndef INCLUDES
INCLUDE_HEADERS := $(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ) 
INCLUDES := $(addprefix -I,$(INCLUDE_HEADERS))
endif

TEST_OBJ := ${PWD}/tests/mock.o
TEST_FILE := ${PWD}/tests/mock.cc

LIB_FILE := ${PWD}/libs/libhttp_request_v1.so
BIN_DIR := ${PWD}/bin/

ifndef LINKERS
LINKERS := -L${HOME}/include/formatter/libs -lformatter \
		-L${HOME}/include/json/libs -ljson_parser
else
LINKERS += -L${HOME}/include/formatter/libs -lformatter \
		-L${HOME}/include/json/libs -ljson_parser \
		-L${HOME}/include/web/request/libs -lhttp_request_v1
endif

.PHONY: request_all clean mock_request

request_all: ${OBJS}
	${CXX} -std=c++23 --shared -fPIC $^ -o ${LIB_FILE}

mock_request: json_all formatter_all request_all mock
	echo "compiled successfully"

mock: ${TEST_OBJ}
	${CXX} -std=c++23 $^ ${INCLUDES} ${LINKERS} -o ${BIN_DIR}/$@

${TEST_OBJ}: ${TEST_FILE}
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

${PWD}/build/%.o: ${PWD}/impl/%.cc
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

clean: 
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
