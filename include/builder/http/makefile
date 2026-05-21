ifndef __HTTP_BUILDER_MAKEFILE__
__HTTP_BUILDER_MAKEFILE__ = ""

AGENT_HOME:=/home/ahethesham/agent

include ${AGENT_HOME}/include/web/request/makefile
include ${AGENT_HOME}/include/web/response/makefile

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${AGENT_HOME} -type f -name "*.h") ) ))
CFLAGS ?= -Wall -Wextra -MMD -MP

HTTP_BUILDER_OBJS:=${AGENT_HOME}/include/builder/http/build/http_request_builder_v1.o \
				   ${AGENT_HOME}/include/builder/http/build/http_response_builder_v1.o

HTTP_BUILDER_LIB_FILE:=${AGENT_HOME}/include/builder/http/libs/libhttp_builder.so
HTTP_BUILDER_BIN:=${AGENT_HOME}/include/builder/http/bin/mock

HTTP_BUILDER_LINKERS:=-L${AGENT_HOME}/include/builder/http/libs/ -lhttp_builder

HTTP_BUILDER_TEST_OBJ=${AGENT_HOME}/include/builder/http/tests/mock.o
HTTP_BUILDER_DEPS = $(HTTP_BUILDER_OBJS:.o=.d) $(HTTP_BUILDER_TEST_OBJ:.o=.d)
HTTP_BUILDER_TEST_FILE=${AGENT_HOME}/include/builder/http/tests/mock.cc


.PHONY: http_builder_all http_builder_clean

http_builder_all: request_all response_all http_builder_app
	echo "built and compiled ${HTTP_BUILDER_LIB_FILE} successfully"

http_builder_app: ${HTTP_BUILDER_OBJS}
	${CXX} -std=c++23 -shared -fPIC $^ ${LINKERS} ${INCLUDES} -o ${HTTP_BUILDER_LIB_FILE}
	$(eval LINKERS=$(LINKERS) ${HTTP_BUILDER_LINKERS})

builder_mock: ${HTTP_BUILDER_TEST_OBJ} http_builder_all
	${CXX} -std=c++23  $< ${LINKERS}  -o ${HTTP_BUILDER_BIN}

${HTTP_BUILDER_TEST_OBJ}: ${HTTP_BUILDER_TEST_FILE}
	${CXX} $(CFLAGS) -std=c++23 -c -fPIC $< ${LINKERS} ${INCLUDES} -o $@

${AGENT_HOME}/include/builder/http/build/%.o: ${AGENT_HOME}/include/builder/http/impl/%.cc
	${CXX} $(CFLAGS) -std=c++23 -c -fPIC $< ${INCLUDES} ${HTTP_BUILDER_LINKER}  -o $@

-include $(HTTP_BUILDER_DEPS)

builder_clean:
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$( ls ./tests/*.o | wc -l ) -gt 0 ]; then rm -f ./tests/*.o ; fi



endif
