ifndef __HTTP_BUILDER_MAKEFILE__
__HTTP_BUILDER_MAKEFILE__ = ""

HOME=$(shell find ~/ -name agent -type d | head -n 1 )

include ${HOME}/include/web/request/makefile
include ${HOME}/include/web/response/makefile

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ))
CFLAGS ?= -Wall -Wextra -MMD -MP

HTTP_BUILDER_OBJS:=${HOME}/include/builder/http/build/http_request_builder_v1.o \
				   ${HOME}/include/builder/http/build/http_response_builder_v1.o

HTTP_BUILDER_LIB_FILE:=${HOME}/include/builder/http/libs/libhttp_builder.so
HTTP_BUILDER_BIN:=${HOME}/include/builder/http/bin/mock

HTTP_BUILDER_LINKERS:=-L${HOME}/include/builder/http/libs/ -lhttp_builder

HTTP_BUILDER_TEST_OBJ=${HOME}/include/builder/http/tests/mock.o
HTTP_BUILDER_DEPS = $(HTTP_BUILDER_OBJS:.o=.d) $(HTTP_BUILDER_TEST_OBJ:.o=.d)
HTTP_BUILDER_TEST_FILE=${HOME}/include/builder/http/tests/mock.cc


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

${HOME}/include/builder/http/build/%.o: ${HOME}/include/builder/http/impl/%.cc
	${CXX} $(CFLAGS) -std=c++23 -c -fPIC $< ${INCLUDES} ${HTTP_BUILDER_LINKER}  -o $@

-include $(HTTP_BUILDER_DEPS)

builder_clean:
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$( ls ./tests/*.o | wc -l ) -gt 0 ]; then rm -f ./tests/*.o ; fi



endif
