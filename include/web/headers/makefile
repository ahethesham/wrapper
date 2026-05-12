
ifndef __HTTP_HEADERS_MAKEFILE__
__HTTP_HEADERS_MAKEFILE__ = ""

# this is a L0 object should not be dependent on any other objects 
# headers_mock for building a mock binary for testing this module
CFLAGS ?= -Wall -Wextra -MMD -MP

HOME := $(shell find ~/ -type d  -name agent | head -n 1 )
INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME} -type f -name "*.h" ))))
HEADERS_LIB_FILE := ${HOME}/include/web/headers/libs/libhttp_headers.so
HEADERS_BIN_FILE := ${HOME}/include/web/headers/bin/mock
HEADERS_TEST_OBJ := ${HOME}/include/web/headers/tests/mock.o
HEADERS_TEST_FILE := ${HOME}/include/web/headers/tests/mock.cc

HEADERS_OBJS := ${HOME}/include/web/headers/build/http_headers_v1.o
HEADERS_DEP := $(HEADERS_OBJS:.o=.d) $(HEADERS_TEST_OBJ:.o=.d)
HEADERS_LINKERS := -L${HOME}/include/web/headers/libs/ -lhttp_headers

.PHONY: headers_clean headers_all

headers_all: headers_app
	echo "compiled and generated ${HEADERS_LIB_FILE} successfully"

headers_app: ${HEADERS_OBJS}
	${CXX} -std=c++23 -fPIC -shared $^ ${INCLUDES} -o ${HEADERS_LIB_FILE}
	$(eval LINKERS=$(LINKERS) $(HEADERS_LINKERS))

headers_mock: ${HEADERS_TEST_OBJ} headers_app
	${CXX} -std=c++23 $< ${INCLUDES} ${HEADERS_LINKERS} -o ${HEADERS_BIN_FILE} 

mock_headers: ${HEADERS_TEST_OBJ} headers_all
	${CXX} -std=c++23 $< ${INCLUDES} ${LINKERS} -o ${HEADERS_BIN_FILE}

${HEADERS_TEST_OBJ}: ${HEADERS_TEST_FILE}
	${CXX} $(CFLAGS) -std=c++23 $< -fPIC -c ${INCLUDES} -o $@

${HOME}/include/web/headers/build/%.o: ${HOME}/include/web/headers/impl/%.cc
	${CXX} $(CFLAGS) -std=c++23 -c -fPIC $< ${INCLUDES} -o $@

-include $(HEADERS_DEP)

headers_clean:
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$(ls ./tests/*.o | wc -l) -gt 0 ] ; then rm -f ./tests/*.o ; fi


endif
