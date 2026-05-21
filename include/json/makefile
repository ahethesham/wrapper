ifndef __JSON_MAKEFILE__
__JSON_MAKEFILE__ := 1


AGENT_HOME:=/home/ahethesham/agent
#this is a level 1 L1 object which should only depend on the object interfaces on level 0
# i know we will figure out the logger object later ( since it is also a level 1)

include ${AGENT_HOME}/include/stream/makefile
include ${AGENT_HOME}/include/token/makefile
include ${AGENT_HOME}/include/logger/makefile
include ${AGENT_HOME}/include/formatter/makefile
include ${AGENT_HOME}/include/config/makefile

CFLAGS ?= -Wall -Wextra -MMD -MP

JSON_ARRAY_FILE=${AGENT_HOME}/include/json/build/json_array_v1.o
JSON_OBJECT_FILE=${AGENT_HOME}/include/json/build/json_object_v1.o
JSON_STRING_FILE=${AGENT_HOME}/include/json/build/json_string_v1.o
JSON_INTEGER_FILE=${AGENT_HOME}/include/json/build/json_integer_v1.o
JSON_BOOLEAN_FILE=${AGENT_HOME}/include/json/build/json_boolean_v1.o
JSON_NULL_FILE=${AGENT_HOME}/include/json/build/json_null_v1.o

JSON_LIB_FILE=${AGENT_HOME}/include/json/libs/libjson_parser.so

JSON_OBJS=${JSON_OBJECT_FILE} \
	${JSON_ARRAY_FILE} \
	${JSON_STRING_FILE} \
	${JSON_INTEGER_FILE} \
	${JSON_BOOLEAN_FILE}   \
	${JSON_NULL_FILE}

JSON_TEST_MAIN=${AGENT_HOME}/include/json/tests/mock.cc
JSON_TEST_OBJ=${AGENT_HOME}/include/json/tests/mock.o

JSON_DEP := $(JSON_OBJS:.o=.d) $(JSON_TEST_OBJ:.o=.d)
INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${AGENT_HOME} -type f -name "*.h") ))) 



.PHONY: json_all json_clean

#libjson_parser.so depends on libstream.so and liblogger.so which will have file io and logger definitions
json_all: file_io_all logger_all token_all formatter_all config_all json_app
	echo "compiled and successfully built ${LIB_FILE}"

json_app: ${JSON_OBJS}
	${CXX} -std=c++23 -shared  -fPIC $^ ${LINKERS} -o ${JSON_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${AGENT_HOME}/include/json/libs/ -ljson_parser)

json_mock: ${JSON_TEST_OBJ} json_all 
	${CXX} -std=c++23 $< ${INCLUDES} ${LINKERS} -o ${AGENT_HOME}/include/json/bin/$@

${AGENT_HOME}/include/json/build/%.o: ${AGENT_HOME}/include/json/impl/%.cc 
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

${JSON_TEST_OBJ}: ${JSON_TEST_MAIN}
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

-include $(JSON_DEP)

json_clean:
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$( ls ./tests/*.o | wc -l ) -gt 0 ]; then rm -f ./tests/*.o ; fi

endif
