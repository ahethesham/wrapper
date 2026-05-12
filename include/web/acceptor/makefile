ifndef __WEB_ACCEPTOR_MAKEFILE__
__WEB_ACCEPTOR_MAKEFILE__ := 1

HOME=$(shell find ~/ -type d -name agent | head -n 1 )

include ${HOME}/include/stream/makefile

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME}/include/ -type f -name "*.h" ) ) ))

ACCEPTOR_OBJS := ${HOME}/include/web/acceptor/build/acceptor_v1.o
ACCEPTOR_TEST_OBJ := ${HOME}/include/web/acceptor/tests/mock.o
ACCEPTOR_TEST_BIN_FILE := ${HOME}/include/web/acceptor/bin/mock
ACCEPTOR_LIB := ${HOME}/include/web/acceptor/libs/libacceptor.so

.PHONY: acceptor_all acceptor_clean

acceptor_all: io_handle_all acceptor_app
	echo "compiled and built ${ACCEPTOR_LIB} successfully"

acceptor_app: ${ACCEPTOR_OBJS}
	${CXX} -std=c++23 --shared -fPIC $^ ${LINKERS} -o ${ACCEPTOR_LIB}
	$(eval LINKERS=$(LINKERS) -L{HOME}/include/acceptor/libs -lacceptor)

acceptor_mock: ${ACCEPTOR_TEST_OBJ} acceptor_all
	${CXX} -std=c++23 $< ${INCLUDES} ${LINKERS} -o ${ACCEPTOR_TEST_BIN_FILE}

${HOME}/include/web/acceptor/build/%.o: ${HOME}/include/web/acceptor/impl/%.cc
	${CXX} -std=c++23 -fPIC $< -c ${LINKERS} ${INCLUDES} -o $@

acceptor_clean:
	if [ $$( ls ./build/ | wc -l) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$( ls ./tests/*.o | wc -l) -gt 0 ]; then rm -f ./tests/*.o ; fi


endif
