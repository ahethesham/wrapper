
HOME ?= $(shell find ~ -type d -name agent | head -n 1)

include ${HOME}/include/stream/makefile

TEST_MAIN=${HOME}/include/logger/tests/mock.cc

OBJS=${HOME}/include/logger/build/file_logger_v1.o 

TEST_OBJ=${HOME}/include/logger/tests/mock.o

INCLUDE_HEADERS := $(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ) 

BIN_DIR=${HOME}/include/Logger/bin

INCLUDES := $(addprefix -I,$(INCLUDE_HEADERS))

LINKERS=-L${HOME}/include/stream/libs/ -lstream 

mock_logger: ${TEST_OBJ} ${OBJS} 
	${CXX} -std=c++23  $^ ${INCLUDES} ${LINKERS} -o ${BIN_DIR}/mock

file_logger: stream_all ${OBJS} 
	${CXX} -std=c++23 -fPIC --shared $^ ${INCLUDES} ${LINKERS} -o ${HOME}/include/Logger/libs/libfile_logger.so

logger_all: stream_all ${OBJS}
	${CXX} -std=c++23 -fPIC --shared $^ ${INCLUDES} ${LINKERS} -o ${HOME}/include/Logger/libs/libfile_logger.so

${HOME}/include/Logger/build/%.o: ${HOME}/include/Logger/impl/%.cc 
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

${TEST_OBJ}: ${TEST_MAIN}
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@ 


clean: 
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi
