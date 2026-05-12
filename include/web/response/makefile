ifndef __HTTP_RESPONSE_MAKEFILE__
__HTTP_RESPONSE_MAKEFILE__="__HTTP_RESPONSE_MAKEFILE__"

HOME=$(shell find ~/ -type d -name agent | head -n 1 )

#http response depends upon the fomatter , headers and json objects 
include ${HOME}/include/web/headers/makefile
include ${HOME}/include/formatter/makefile
include ${HOME}/include/json/makefile

CFLAGS ?= -Wall -Wextra -MMD -MP

RESPONSE_LIB_FILE := ${HOME}/include/web/response/libs/libhttp_response.so
RESPONSE_BIN_FILE := ${HOME}/include/web/response/bin/mock
RESPONSE_OBJ_FILES := ${HOME}/include/web/response/build/http_response_line_v1.o \
					  ${HOME}/include/web/response/build/http_response_v1.o

RESPONSE_TEST_OBJ_FILE := $(HOME)/include/web/response/tests/mock.o
RESPONSE_TEST_FILE := $(RESPONSE_TEST_OBJ_FILE:.o=.cc)

RESPONSE_DEP := $(RESPONSE_TEST_OBJ_FILE:.o=.d) $(RESPONSE_OBJ_FILES:.o=.d)

RESPONSE_LINKERS := -L${HOME}/include/web/response/libs/ -lhttp_response 
RESPONSE_TEST_DIR := ${HOME}/include/web/response/tests


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

${HOME}/include/web/response/build/%.o: ${HOME}/include/web/response/impl/%.cc
	${CXX}  $(CFLAGS) -std=c++23 -c -fPIC $< ${INCLUDES} -o $@

-include $(RESPONSE_DEP)

response_clean:
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$(ls ./tests/*.o | wc -l) -gt 0 ] ; then rm -f ./tests/*.o ; fi


endif
