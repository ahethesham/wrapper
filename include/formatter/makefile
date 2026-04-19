
HOME=$(shell find ~ -type d -name agent | head -n 1)

TEST_MAIN=${HOME}/include/formatter/tests/mock.cc

OBJS=${HOME}/include/formatter/build/json_formatter_v1.o \
	${HOME}/include/formatter/build/http_formatter_v1.o

TEST_OBJ=${HOME}/include/formatter/tests/mock.o

INCLUDE_HEADERS := $(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ) \
				   	/usr/include/ 

BIN_DIR=${HOME}/include/formatter/bin
LIB_DIR=${HOME}/include/formatter/libs

INCLUDES := $(addprefix -I,$(INCLUDE_HEADERS))

LINKERS=

mock_json_formatter: ${TEST_OBJ} ${OBJS} 
	${CXX} -std=c++23  $^ ${INCLUDES} ${LINKERS} -o ${BIN_DIR}/mock


all: ${OBJS}
	${CXX} -std=c++23 -fPIC --shared $^ ${INCLUDES} ${LINKERS} -o ${LIB_DIR}/libformatter.so

${HOME}/include/formatter/build/%.o: ${HOME}/include/formatter/impl/%.cc 
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

${TEST_OBJ}: ${TEST_MAIN}
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@ 


clean: 
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi
