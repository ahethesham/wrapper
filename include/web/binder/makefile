ifndef __BINDER_MAKEFILE__
__BINDER_MAKEFILE__ := 1

HOME := $(shell find ~/ -type d -name agent | head -n 1 )

include ${HOME}/include/stream/makefile
include ${HOME}/include/web/endpoint/makefile

BINDER_OBJS:=${HOME}/include/web/binder/build/binder_listener_v1.o
BINDER_TEST_FILE:=${HOME}/include/web/binder/tests/mock.cc
BINDER_TEST_OBJ:=${HOME}/include/web/binder/tests/mock.o
BINDER_BIN_FILE:=${HOME}/include/web/binder/bin/mock
BINDER_LIB_FILE:=${HOME}/include/web/binder/libs/libbinder.so


.PHONY: binder_all binder_clean

binder_all: io_handle_all endpoint_all binder_all
	@echo "built and compiled ${BINDER_LIB_FILE} successfully"

binder_app: ${BINDER_OBJS}
	${CXX} -std=c++23 --shared -fPIC $^ ${LINKERS} ${INCLUDES} -o ${BINDER_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${HOME}/include/web/binder/libs -lbinder)

binder_mock: ${BINDER_TEST_OBJ} binder_all
	${CXX} -std=c++23 $< ${LINKERS} ${INCLUDES} -o ${BINDER_BIN_FILE}

${HOME}/include/web/binder/build/%.o: ${HOME}/include/web/binder/build/%.cc
	${CXX} -std=c++23 -c -fPIC $< ${LINKERS} ${INCLUDES} -o $@

${BINDER_TEST_OBJ}: ${BINDER_TEST_FILE}
	${CXX} -std=c++23 -c -fPIC $< ${LINKERS} ${INCLUDES} -o $@

binder_clean:
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$( ls ./tests/*.o | wc -l ) -gt 0 ]; then rm -f ./tests/*.o ; fi


endif

