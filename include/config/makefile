ifndef __CONFIG_MAKEFILE__
__CONFIG_MAKEFILE__ := 1

AGENT_HOME:=/home/ahethesham/agent

include ${AGENT_HOME}/include/logger/makefile
include ${AGENT_HOME}/include/stream/makefile

CFLAGS ?= -Wall -Wextra -MMD -MP

CONFIG_TEST_MAIN=${AGENT_HOME}/include/config/tests/mock.cc
CONFIG_TEST_OBJ=${AGENT_HOME}/include/config/tests/mock.o
CONFIG_OBJS=${AGENT_HOME}/include/config/build/ini_container.o \
			${AGENT_HOME}/include/config/build/ini_config_parser.o 
CONFIG_DEPS = $(CONFIG_OBJS:.o=.d) $(CONFIG_TEST_OBJ:.o=.d)

CONFIG_LIB_FILE=${AGENT_HOME}/include/config/libs/libconfig_parser.so

CONFIG_LINKERS=-L${AGENT_HOME}/include/stream/libs/ -lstream 

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${AGENT_HOME} -type f -name "*.h") ) ))

STREAM_DIR=${AGENT_HOME}/include/stream/
LOGGER_DIR=${AGENT_HOME}/include/Logger/

#LINKERS=-L/usr/lib/x86_64-linux-gnu -lssl -lcrypto
.PHONY: config_clean config_all

config_all: file_io_all logger_all config_app
	echo "compiled and generated ${CONFIG_LIB_FILE} successfully"

config_app: ${CONFIG_OBJS}
	${CXX}  -std=c++23 -fPIC -shared  $^ ${LINKERS} -I{INCLUDES} -o ${CONFIG_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${AGENT_HOME}/include/config/libs/ -lconfig_parser)
	

config_mock: $(CONFIG_TEST_OBJ) config_all
	${CXX} -std=c++23 $< ${INCLUDES} ${LINKERS} -o ${AGENT_HOME}/include/config/bin/mock

${AGENT_HOME}/include/config/tests/%.o: ${AGENT_HOME}/include/config/tests/%.cc
	$(CXX) $(CFLAGS) -std=c++23 -fPIC $< -c $(INCLUDES)  -o $@

${AGENT_HOME}/include/config/build/%.o: ${AGENT_HOME}/include/config/impl/%.cc 
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@


${CONFIG_TEST_OBJ}: ${CONFIG_TEST_MAIN}
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o  $@

-include $(CONFIG_DEPS)

config_clean: 
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi

endif
