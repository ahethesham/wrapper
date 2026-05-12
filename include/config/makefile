ifndef __CONFIG_MAKEFILE__
	__CONFIG_MAKEFILE__ := 1

HOME=$(shell find ~ -type d -name agent | head -n 1)

include ${HOME}/include/logger/makefile
include ${HOME}/include/stream/makefile

CFLAGS ?= -Wall -Wextra -MMD -MP

CONFIG_TEST_MAIN=${HOME}/include/config/tests/mock.cc
CONFIG_TEST_OBJ=${HOME}/include/config/bin/main.o
CONFIG_OBJS=${HOME}/include/config/build/ini_container.o \
			${HOME}/include/config/build/ini_config_parser.o 
CONFIG_DEPS = $(CONFIG_OBJS:.o=.d) $(CONFIG_TEST_OBJ:.o=.d)

CONFIG_LIB_FILE=${HOME}/include/config/libs/libconfig_parser.so

CONFIG_LINKERS=-L${HOME}/include/stream/libs/ -lstream 

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ))

STREAM_DIR=${HOME}/include/stream/
LOGGER_DIR=${HOME}/include/Logger/

#LINKERS=-L/usr/lib/x86_64-linux-gnu -lssl -lcrypto
.PHONY: config_clean config_all

config_all: file_io_all logger_all config_app
	echo "compiled and generated ${CONFIG_LIB_FILE} successfully"

config_app: ${OBJS}
	${CXX}  -std=c++23 -fPIC -shared  $^ ${LINKERS} -I{INCLUDES} -o ${CONFIG_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${HOME}/include/config/libs/ -lconfig_parser)
	

mock_config: $(CONFIG_TEST_OBJ) config_all
	${CXX} -std=c++23 $< ${INCLUDES} ${LINKERS} -o ${HOME}/include/config/bin/mock

${HOME}/include/config/tests/%.o: ${HOME}/include/config/tests/%.cc
	$(CXX) $(CFLAGS) -std=c++23 -fPIC $< -c $(INCLUDES)  -o $@

${HOME}/include/config/build/%.o: ${HOME}/include/config/impl/%.cc 
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@


${TEST_OBJ}: ${TEST_MAIN}
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o  $@

-include $(CONFIG_DEPS)

config_clean: 
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi

endif
