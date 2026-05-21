ifndef __BINDER_MAKEFILE__
__BINDER_MAKEFILE__ := 1

AGENT_HOME:=/home/ahethesham/agent

include ${AGENT_HOME}/include/stream/makefile
include ${AGENT_HOME}/include/web/endpoint/makefile

BINDER_OBJS:=${AGENT_HOME}/include/web/binder/build/binder_listener_v1.o
BINDER_TEST_FILE:=${AGENT_HOME}/include/web/binder/tests/mock.cc
BINDER_TEST_OBJ:=${AGENT_HOME}/include/web/binder/tests/mock.o
BINDER_BIN_FILE:=${AGENT_HOME}/include/web/binder/bin/mock
BINDER_LIB_FILE:=${AGENT_HOME}/include/web/binder/libs/libbinder.so


.PHONY: binder_all binder_clean

binder_all: io_handle_all endpoint_all binder_all
	@echo "built and compiled ${BINDER_LIB_FILE} successfully"

binder_app: ${BINDER_OBJS}
	${CXX} -std=c++23 --shared -fPIC $^ ${LINKERS} ${INCLUDES} -o ${BINDER_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${AGENT_HOME}/include/web/binder/libs -lbinder)

binder_mock: ${BINDER_TEST_OBJ} binder_all
	${CXX} -std=c++23 $< ${LINKERS} ${INCLUDES} -o ${BINDER_BIN_FILE}

${AGENT_HOME}/include/web/binder/build/%.o: ${AGENT_HOME}/include/web/binder/build/%.cc
	${CXX} -std=c++23 -c -fPIC $< ${LINKERS} ${INCLUDES} -o $@

${BINDER_TEST_OBJ}: ${BINDER_TEST_FILE}
	${CXX} -std=c++23 -c -fPIC $< ${LINKERS} ${INCLUDES} -o $@

binder_clean:
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$( ls ./tests/*.o | wc -l ) -gt 0 ]; then rm -f ./tests/*.o ; fi


endif

