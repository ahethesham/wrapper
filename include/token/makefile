ifndef __TOKENIZER_MAKEFILE__
__TOKENIZER_MAKEFILE__:=1

#this is a Level 0 object we cannot make these objects dependent on any another 

AGENT_HOME:=/home/ahethesham/agent
CFLAGS ?= -Wall -Wextra -MMD -MP

TOKEN_TEST_MAIN=${AGENT_HOME}/include/token/tests/token_mock.cc
TOKEN_LIB_FILE=${AGENT_HOME}/include/token/libs/libtokenizer.so

TOKEN_OBJS=${AGENT_HOME}/include/token/build/json_grammer_v1.o 

TOKENIZER_BIN_FILE=${AGENT_HOME}/include/token/bin/mock
TOKEN_TEST_OBJ = ${AGENT_HOME}/include/token/tests/mock.o
TOKEN_TEST_FILE = ${AGENT_HOME}/include/token/tests/mock.cc
TOKEN_DEP := $(TOKEN_OBJS:.o=.d) $(TOKEN_TEST_OBJ:.o=.d)
TOKENIZER_LINKER=-L${AGENT_HOME}/include/token/libs -ltokenizer 

INCLUDES := $(addprefix -I,$(sort $(dir $(shell find ${AGENT_HOME} -type f -name "*.h") ) ))

.PHONY: token_all token_clean 

token_all: token_app
	echo "compiled and generated ${TOKEN_LIB_FILE} successfully"

token_app: ${TOKEN_OBJS}
	${CXX} -std=c++23 -fPIC --shared $^ -o ${TOKEN_LIB_FILE}
	$(eval LINKERS=$(LINKERS) ${TOKENIZER_LINKER})

# build the libtokenizer sepereatey and libjson_parser seperately
token_mock: ${TOKEN_TEST_OBJ} token_all 
	${CXX} -std=c++23 $< ${TOKENIZER_LINKER} ${INCLUDES} -o ${TOKENIZER_BIN_FILE}

${TOKEN_TEST_OBJ}: ${TOKEN_TEST_FILE} 
	${CXX} -std=c++23 -fPIC $< -c ${INCLUDES}  -o $@

${AGENT_HOME}/include/token/build/%.o: ${AGENT_HOME}/include/token/impl/%.cc
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

-include $(TOKEN_DEP)

token_clean: 
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
	if [ $$( ls ./tests/*.o | wc -l ) -gt 0 ]; then rm -f ./tests/*.o ; fi


endif
