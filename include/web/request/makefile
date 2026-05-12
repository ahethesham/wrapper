ifndef __HTTP_REQUEST_MAKEFILE__
__HTTP_REQUEST_MAKEFILE__ := 1

HOME=$(shell find ~/ -name agent -type d | head -n 1 )

include ${HOME}/include/formatter/makefile
include ${HOME}/include/json/makefile
include ${HOME}/include/web/headers/makefile

CFLAGS ?= -Wall -Wextra -MMD -MP

WEB_REQ_OBJS=${HOME}/include/web/request/build/http_request_line_v1.o \
	 ${HOME}/include/web/request/build/http_request_v1.o

WEB_REQ_TEST_OBJ := ${HOME}/include/web/request/tests/mock.o
WEB_REQ_TEST_FILE := ${HOME}/include/web/request/tests/mock.cc
WEB_REQ_DEP := $(WEB_REQ_TEST_OBJ:.o=.d) $(WEB_REQ_OBJS:.o=.d)
INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ) )

WEB_REQ_LINKERS := -L${HOME}/include/formatter/libs -lformatter \
		-L${HOME}/include/json/libs -ljson_parser \
		${HEADERS_LINKERS}

WEB_REQ_LIB_FILE := ${HOME}/include/web/request/libs/libhttp_request.so
WEB_REQ_BIN_FILE := ${HOME}/include/web/request/bin/mock

.PHONY: request_all request_clean mock_request

request_all: json_all formatter_all headers_all request_app
	echo "built and compiled ${WEB_REQ_LINKERS} successfully"

request_app: ${WEB_REQ_OBJS}
	${CXX} -std=c++23 --shared -fPIC $^ ${WEB_REQ_LINKERS} -o ${WEB_REQ_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${HOME}/include/web/request/libs/ -lhttp_request)
	
mock_request: ${TEST_OBJ}  request_all 
	${CXX} -std=c++23 $< ${INCLUDES} ${LINKERS} -o ${WEB_REQ_BIN_FILE}


mock_http_request_line: ${TEST_OBJ} ${HOME}/include/web/request/build/http_request_line_v1.o
	${CXX} -std=c++23 $^ ${INCLUDES} ${LINKERS} -o ${WEB_REQ_BIN_FILE}

${WEB_REQ_TEST_OBJ}: ${WEB_REQ_TEST_FILE}
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

${HOME}/include/web/request/build/%.o: ${HOME}/include/web/request/impl/%.cc
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

-include $(WEB_REQ_DEPS)

request_clean: 
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$(ls ./tests/*.o | wc -l) -gt 0 ] ; then rm -f ./tests/*.o ; fi

endif
