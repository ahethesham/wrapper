ifndef __LOGGER_MAKEFILE
__LOGGER_MAKEFILE=""

HOME = $(shell find ~ -type d -name agent | head -n 1)

include ${HOME}/include/stream/makefile

LOGGER_TEST_MAIN=${HOME}/include/logger/tests/mock.cc
LOGGER_OBJS=${HOME}/include/logger/build/file_logger_v1.o 
LOGGER_TEST_OBJ=${HOME}/include/logger/tests/mock.o
LOGGER_BIN_DIR=${HOME}/include/logger/bin
LOGGER_DEP := $(LOGGER_OBJS:.o=.d) $(LOGGER_TEST_OBJS:.o=.d)
INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ))

LOGGER_LIB_FILE=${HOME}/include/logger/libs/liblogger.so

.PHONY: logger_all logger_clean

logger_all: file_io_all logger_app
	echo "compiled and generated ${LOGGER_LIB_FILE} successfully"
 

logger_mock: ${LOGGER_TEST_OBJ} logger_all 
	${CXX} -std=c++23  $< ${INCLUDES} ${LINKERS} -o ${LOGGER_BIN_DIR}/mock


logger_app: ${LOGGER_OBJS}
	${CXX} -std=c++23 -shared -fPIC  $^ ${INCLUDES} ${LINKERS} -o ${LOGGER_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${HOME}/include/logger/libs/ -llogger)

${HOME}/include/logger/build/%.o: ${HOME}/include/logger/impl/%.cc 
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
