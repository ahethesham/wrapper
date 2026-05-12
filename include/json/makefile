ifndef __JSON_MAKEFILE__
__JSON_MAKEFILE__ := 1


HOME = $(shell find ~ -type d -name agent | head -n 1)
#this is a level 1 L1 object which should only depend on the object interfaces on level 0
# i know we will figure out the logger object later ( since it is also a level 1)

include ${HOME}/include/stream/makefile
include ${HOME}/include/token/makefile
include ${HOME}/include/logger/makefile
include ${HOME}/include/formatter/makefile
include ${HOME}/include/config/makefile

CFLAGS ?= -Wall -Wextra -MMD -MP

JSON_ARRAY_FILE=${HOME}/include/json/build/json_array_v1.o
JSON_OBJECT_FILE=${HOME}/include/json/build/json_object_v1.o
JSON_STRING_FILE=${HOME}/include/json/build/json_string_v1.o
JSON_INTEGER_FILE=${HOME}/include/json/build/json_integer_v1.o
JSON_BOOLEAN_FILE=${HOME}/include/json/build/json_boolean_v1.o
JSON_NULL_FILE=${HOME}/include/json/build/json_null_v1.o

JSON_LIB_FILE=${HOME}/include/json/libs/libjson_parser.so

JSON_OBJS=${JSON_OBJECT_FILE} \
	${JSON_ARRAY_FILE} \
	${JSON_STRING_FILE} \
	${JSON_INTEGER_FILE} \
	${JSON_BOOLEAN_FILE}   \
	${JSON_NULL_FILE}

JSON_TEST_MAIN=${HOME}/include/json/tests/mock.cc
JSON_TEST_OBJ=${HOME}/include/json/tests/mock.o

JSON_DEP := $(JSON_OBJS:.o=.d) $(JSON_TEST_OBJ:.o=.d)
INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME} -type f -name "*.h") ))) 



.PHONY: json_all json_clean

#libjson_parser.so depends on libstream.so and liblogger.so which will have file io and logger definitions
json_all: file_io_all logger_all token_all formatter_all config_all json_app
	echo "compiled and successfully built ${LIB_FILE}"

json_app: ${JSON_OBJS}
	${CXX} -std=c++23 -shared  -fPIC $^ ${LINKERS} -o ${JSON_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${HOME}/include/json/libs/ -ljson_parser)

json_mock: ${JSON_TEST_OBJ} json_all 
	${CXX} -std=c++23 $< ${INCLUDES} ${LINKERS} -o ${HOME}/include/json/bin/$@

${HOME}/include/json/build/%.o: ${HOME}/include/json/impl/%.cc 
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
