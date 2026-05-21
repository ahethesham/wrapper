ifndef __LOGGER_MAKEFILE__
__LOGGER_MAKEFILE__  :=""

AGENT_HOME:=/home/ahethesham/agent

include ${AGENT_HOME}/include/stream/makefile

LOGGER_TEST_MAIN=${AGENT_HOME}/include/logger/tests/mock.cc
LOGGER_OBJS=${AGENT_HOME}/include/logger/build/file_logger_v1.o 
LOGGER_TEST_OBJ=${AGENT_HOME}/include/logger/tests/mock.o
LOGGER_BIN_DIR=${AGENT_HOME}/include/logger/bin
LOGGER_DEP := $(LOGGER_OBJS:.o=.d) $(LOGGER_TEST_OBJS:.o=.d)
INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${AGENT_HOME} -type f -name "*.h") ) ))

LOGGER_LIB_FILE=${AGENT_HOME}/include/logger/libs/liblogger.so

.PHONY: logger_all logger_clean

logger_all: file_io_all logger_app
	echo "compiled and generated ${LOGGER_LIB_FILE} successfully"
 

logger_mock: ${LOGGER_TEST_OBJ} logger_all 
	${CXX} -std=c++23  $< ${INCLUDES} ${LINKERS} -o ${LOGGER_BIN_DIR}/mock


logger_app: ${LOGGER_OBJS}
	${CXX} -std=c++23 -shared -fPIC  $^ ${INCLUDES} ${LINKERS} -o ${LOGGER_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${AGENT_HOME}/include/logger/libs/ -llogger)

${AGENT_HOME}/include/logger/build/%.o: ${AGENT_HOME}/include/logger/impl/%.cc 
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

${LOGGER_TEST_OBJ}: ${LOGGER_TEST_MAIN}
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@ 

-include $(LOGGER_DEP)


logger_clean: 
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi
	if [ $$(ls ./tests/*.o | wc -l) -gt 0 ]; then rm ./tests/*.o ; fi

endif
