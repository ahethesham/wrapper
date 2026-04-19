HOME=$(shell find ~/ -name agent -type d | head -n 1 )
PWD:=$(shell pwd)
# this builder .so is not dependent on any other .so

INCLUDE_HEADERS := $(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) )
INCLUDES := $(addprefix -I,$(INCLUDE_HEADERS))

export LINKERS:=${LINKERS} \
		 -L${HOME}/include/web/request/libs/ -lhttp_request \
		 -L${HOME}/include/formatter/libs    -lhttp_formatter

OBJS:=${PWD}/build/http_request_builder_v1.o
LIB_FILE:=${PWD}/libs/libhttp_request_builder_v1.so
BIN_DIR:=${PWD}/bin/

TEST_FILE=${PWD}/tests/mock.cc
TEST_OBJ=${PWD}/tests/mock.o


all: ${OBJS}
	${CXX} -std=c++23 --shared -fPIC $^ -o ${LIB_FILE}

mock_request_builder: ${PWD}/build/http_request_builder_v1.o ${TEST_OBJ}
	$(MAKE) -C ../../web/request/ all
	$(MAKE) -C ../../formatter/ all
	${CXX} -std=c++23 $^ ${INCLUDES} ${LINKERS} -o ${BIN_DIR}/$@

${PWD}/build/%.o: ${PWD}/impl/%.cc
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

${TEST_OBJ}: ${TEST_FILE}
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

clean:
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi


