ifndef __FORMATTER__MAKEFILE__
__FORMATTER__MAKEFILE__ := 1

#this is a level 0 object L0 .. and is not supposed to be dependent on any other object

AGENT_HOME:=/home/ahethesham/agent

CFLAGS ?= -Wall -Wextra -MMD -MP

FORMATTER_TEST_MAIN=${AGENT_HOME}/include/formatter/tests/mock.cc
FORMATTER_OBJS=${AGENT_HOME}/include/formatter/build/json_formatter_v1.o \
	${AGENT_HOME}/include/formatter/build/http_formatter_v1.o

FORMATTER_DEP = $(FORMATTER_OBJS:.o=.d)
FORMATTER_TEST_OBJ=${AGENT_HOME}/include/formatter/tests/mock.o
FORMATTER_DEP += $(FORMATTER_TEST_OBJ:.o=.d)
FORMATTER_LIB_FILE=${AGENT_HOME}/include/formatter/libs/libformatter.so

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${AGENT_HOME} -type f -name "*.h") ) ))


.PHONY: formatter_all formatter_clean

formatter_all: formatter_app
	echo "compiled and generated ${FORMATTER_LIB_FILE} successfully"

formatter_app: ${FORMATTER_OBJS}
	${CXX} -std=c++23 -fPIC --shared $^ ${INCLUDES}  -o ${FORMATTER_LIB_FILE}
	$(eval LINKERS=$(LINKERS) -L${AGENT_HOME}/include/formatter/libs/ -lformatter)

${AGENT_HOME}/include/formatter/build/%.o: ${AGENT_HOME}/include/formatter/impl/%.cc 
	${CXX} ${CFLAGS} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

${FORMATTER_TEST_OBJ}: ${FORMATTER_TEST_MAIN}
	${CXX} ${CFLAGS} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@ 

-include $(FORMATTER_DEP)

formatter_clean: 
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi
	if [ $$(ls ./tests/*.o) ]; then rm ./tests/*.o ; fi

endif
