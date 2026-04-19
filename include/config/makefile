
HOME=$(shell find ~ -type d -name agent | head -n 1)

TEST_MAIN=${HOME}/include/config/tests/mock.cc

OBJS=${HOME}/include/config/build/ini_container.o\
${HOME}/include/config/build/ini_config_parser.o 


TEST_OBJ=${HOME}/include/config/bin/main.o

INCLUDE_HEADERS := $(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ) \
				   	/usr/include/ 

export LINKERS=${LINKERS} \
		-L${HOME}/include/stream/libs/ -lstream \
		-L${HOME}/include/config/libs/ -lini_config_parser 


INCLUDES := $(addprefix -I,$(INCLUDE_HEADERS))

STREAM_DIR=${HOME}/include/stream/
LOGGER_DIR=${HOME}/include/Logger/

#LINKERS=-L/usr/lib/x86_64-linux-gnu -lssl -lcrypto
.PHONY: clean

all: ${OBJS}


mock_config: ${OBJS} 
	$(MAKE) -C ${STREAM_DIR} all
	$(MAKE) -C ${LOGGER_DIR} file_logger
	${CXX}  -std=c++23 -fPIC --shared  $^ -o ${HOME}/include/config/libs/libini_config_parser.so
	${CXX} -std=c++23 ${TEST_MAIN} ${INCLUDES} ${LINKERS} -o ${HOME}/include/config/bin/$@

${HOME}/include/config/build/%.o: ${HOME}/include/config/impl/%.cc 
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@


${TEST_OBJ}: ${TEST_MAIN}
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o  $@


clean: 
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi
