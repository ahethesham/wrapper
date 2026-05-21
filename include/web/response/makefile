ifndef __HTTP_RESPONSE_MAKEFILE__
__HTTP_RESPONSE_MAKEFILE__="__HTTP_RESPONSE_MAKEFILE__"

AGENT_HOME:=/home/ahethesham/agent

#http response depends upon the fomatter , headers and json objects 
include ${AGENT_HOME}/include/web/headers/makefile
include ${AGENT_HOME}/include/formatter/makefile
include ${AGENT_HOME}/include/json/makefile

CFLAGS ?= -Wall -Wextra -MMD -MP

RESPONSE_LIB_FILE := ${AGENT_HOME}/include/web/response/libs/libhttp_response.so
RESPONSE_BIN_FILE := ${AGENT_HOME}/include/web/response/bin/mock
RESPONSE_OBJ_FILES := ${AGENT_HOME}/include/web/response/build/http_response_line_v1.o \
					  ${AGENT_HOME}/include/web/response/build/http_response_v1.o

RESPONSE_TEST_OBJ_FILE := $(AGENT_HOME)/include/web/response/tests/mock.o
RESPONSE_TEST_FILE := $(RESPONSE_TEST_OBJ_FILE:.o=.cc)

RESPONSE_DEP := $(RESPONSE_TEST_OBJ_FILE:.o=.d) $(RESPONSE_OBJ_FILES:.o=.d)

RESPONSE_LINKERS := -L${AGENT_HOME}/include/web/response/libs/ -lhttp_response 
RESPONSE_TEST_DIR := ${AGENT_HOME}/include/web/response/tests


.PHONY: response_all response_clean 

response_all: json_all formatter_all headers_all response_app
	echo "generated ${RESPONSE_LIB_FILE} successfully"

response_app: ${RESPONSE_OBJ_FILES}
	${CXX} -std=c++23 -fPIC -shared $^ ${INCLUDES} ${LINKERS} -o ${RESPONSE_LIB_FILE}
	$(eval LINKERS=$(LINKERS) ${RESPONSE_LINKERS})


response_mock: $(RESPONSE_TEST_OBJ_FILE) response_all
	${CXX} -std=c++23 $< ${INCLUDES} ${LINKERS} ${RESPONSE_LINKERS} -o ${RESPONSE_BIN_FILE}

${RESPONSE_TEST_DIR}/%.o: ${RESPONSE_TEST_DIR}/%.cc
	${CXX} $(CFLAGS) -std=c++23 -c -fPIC $< ${INCLUDES} ${LINKERS} -o $@

${AGENT_HOME}/include/web/response/build/%.o: ${AGENT_HOME}/include/web/response/impl/%.cc
	${CXX}  $(CFLAGS) -std=c++23 -c -fPIC $< ${INCLUDES} -o $@

-include $(RESPONSE_DEP)

response_clean:
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$(ls ./tests/*.o | wc -l) -gt 0 ] ; then rm -f ./tests/*.o ; fi


endif
