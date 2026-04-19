HOME ?= $(shell find ~ -type d -name agent | head -n 1)

include ${HOME}/include/stream/makefile
include ${HOME}/include/token/makefile
include ${HOME}/include/logger/makefile


TOKENIZER_DIR=${HOME}/include/token
LOGGER_DIR=${HOME}/include/Logger

TEST_MAIN=${HOME}/include/json/tests/json_mock.cc

JSON_ARRAY_FILE=${HOME}/include/json/build/json_array_v1.o
JSON_OBJECT_FILE=${HOME}/include/json/build/json_object_v1.o
JSON_STRING_FILE=${HOME}/include/json/build/json_string_v1.o
JSON_INTEGER_FILE=${HOME}/include/json/build/json_integer_v1.o
JSON_BOOLEAN_FILE=${HOME}/include/json/build/json_boolean_v1.o
JSON_NULL_FILE=${HOME}/include/json/build/json_null_v1.o

LIB_FILE=${HOME}/include/json/libs/libjson_parser.so
LIB_DIR=${HOME}/include/json/libs

JSON_OBJS=${JSON_OBJECT_FILE} \
	${JSON_ARRAY_FILE} \
	${JSON_STRING_FILE} \
	${JSON_INTEGER_FILE} \
	${JSON_BOOLEAN_FILE}   \
	${JSON_NULL_FILE}


TEST_OBJ=${HOME}/include/json/build/main.o

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME} -type f -name "*.h") ))) 

JSON_LINKERS := -L${HOME}/include/token/libs -ljson_tokenizer \
		-L${HOME}/include/Logger/libs -lfile_logger \
		-L${HOME}/include/stream/libs -lstream \
		-L${HOME}/include/formatter/libs -lformatter \
		-L${HOME}/include/json/libs/ -ljson_parser

ifndef LINKERS
	LINKERS := ${JSON_LINKERS}
else
	LINKERS += ${JSON_LINKERS}
endif

.PHONY: json_all clean

#libjson_parser.so depends on libstream.so and liblogger.so which will have file io and logger definitions
json_all: stream_all logger_all token_all app
	echo "successfully built ${LIB_FILE}"

app: ${JSON_OBJS}
	${CXX} -std=c++23 --shared  -fPIC $^ ${JSON_LINKERS} -o ${LIB_FILE}

mock_json: ${TEST_OBJ} json_all 
	${CXX} -std=c++23 $< ${INCLUDES} ${LINKERS} -o ${HOME}/include/json/bin/$@

${HOME}/include/json/build/%.o: ${HOME}/include/json/impl/%.cc 
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

${TEST_OBJ}: ${TEST_MAIN}
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@


clean:
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
