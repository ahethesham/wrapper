HOME=$(shell find ~ -type d -name agent | head -n 1)

TEST_MAIN=${HOME}/include/token/tests/token_mock.cc

GRAMMER_FILE=${HOME}/include/token/build/json_grammer_v1.o
TOKENIZER_FILE=${HOME}/include/token/build/json_tokenizer_v1.o
TOKEN_FILE=${HOME}/include/token/build/token_v1.o

LIB_DIR=${HOME}/include/token/libs/

OBJS=${GRAMMER_FILE} ${TOKENIZER_FILE} ${TOKEN_FILE}

TEST_OBJ=${HOME}/include/token/build/main.o

INCLUDE_HEADERS := $(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ) \
				   	/usr/include/ 


INCLUDES := $(addprefix -I,$(INCLUDE_HEADERS))


all: ${OBJS}
	${CXX} -std=c++23 -fPIC --shared $^ -o ${LIB_DIR}/libjson_tokenizer.so
	
grammer: ${GRAMMER_FILE}
	echo "compiled ${GRAMMER_FILE} "

tokenizer_file: ${TOKENIZER_FILE}
	echo "compiled ${TOKENIZER_FILE}"

token_file: ${TOKEN_FILE}
	echo "compiled ${TOKEN_FILE}"

${HOME}/include/token/build/%.o: ${HOME}/include/token/impl/%.cc
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

json_tokenizer: all

clean: 
	if [ $$( ls ./build/ | wc -l ) -gt 0 ]; then rm -f ./build/* ; fi
	if [ $$( ls ./libs/ | wc -l ) -gt 0 ]; then rm -f ./libs/* ; fi
	if [ $$( ls ./bin/ | wc -l ) -gt 0 ]; then rm -f ./bin/* ; fi
