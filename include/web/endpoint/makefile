ifndef __ENDPOINT_MAKEFILE__
__ENDPOINT_MAKEFILE__ := 1

# this is a level 0 object 
HOME = $(shell find ~/ -type d -name agent | head -n 1 )

CFLAGS ?= -Wall -Wextra -MMD -MP

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME}/include/ -type f -name "*.h"))))

ENDPOINT_OBJS := ${HOME}/include/web/endpoint/build/v4_tcp_endpoint_v1.o
ENDPOINT_LIB_FILE := ${HOME}/include/web/endpoint/libs/libendpoint.so
ENDPOINT_TEST_OBJ :=  ${HOME}/include/web/endpoint/tests/mock.o
ENDPOINT_TEST_FILE :=  ${HOME}/include/web/endpoint/tests/mock.cc
ENDPOINT_TEST_BIN_FILE := ${HOME}/include/web/endpoint/bin/mock
ENDPOINT_DEP := $(ENDPOINT_OBJS:.o=.d) $(endpoint_test_obj:.o=.d)

.PHONY: endpoint_all endpoint_clean

endpoint_all: endpoint_app
	echo "compiled and built ${ENDPOINT_LIB_FILE} successfully"

endpoint_app: ${ENDPOINT_OBJS}
	${CXX} -std=c++23 --shared $^ ${LINKERS} ${INCLUDES} -o ${ENDPOINT_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${HOME}/include/web/endpoint/libs/ -lendpoint)

endpoint_mock: ${ENDPOINT_TEST_OBJ} endpoint_app
	${CXX} -std=c++23  $< ${LINKERS} ${INCLUDES} -o ${ENDPOINT_TEST_BIN_FILE}

${HOME}/include/web/endpoint/build/%.o: ${HOME}/include/web/endpoint/impl/%.cc
	${CXX} -std=c++23 -fPIC $(CFLAGS) -c $< ${LINKERS} ${INCLUDES} -o $@

${ENDPOINT_TEST_OBJ}: ${ENDPOINT_TEST_FILE}
	${CXX}  -std=c++23 -c -fPIC $(CFLAGS) $< ${LINKERS} ${INCLUDES} -o $@

-include $(ENDPOINT_DEP)

endpoint_clean: 
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$( ls ./tests/*.o | wc -l ) -gt 0 ]; then rm -f ./tests/*.o ; fi

endif
