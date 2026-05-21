ifndef __SOCKET_OPTIONS_MAKEFILE__
__SOCKET_OPTIONS_MAKEFILE__ := 1

AGENT_HOME:=/home/ahethesham/agent

#this is going to be level 0 module

CFLAGS ?= -Wall -Wextra -MMD -MP

SOCKET_OPTIONS_TEST_MAIN := $(AGENT_HOME)/include/socket/tests/mock.cc
SOCKET_OPTIONS_TEST_OBJ := $(SOCKET_OPTIONS_TEST_MAIN:.cc=.o)
SOCKET_OPTIONS_BIN := $(AGENT_HOME)/include/socket/bin

SOCKET_OPTIONS_OBJS := $(AGENT_HOME)/include/socket/build/socket_options_v1.o
SOCKET_OPTIONS_DEP := $(SOCKET_OPTIONS_OBJS:.o=.d) $(SOCKET_OPTIONS_TEST_OBJ:.o=.d)
SOCKET_OPTIONS_LIB := $(AGENT_HOME)/include/socket/libs/libsocket_options.so

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${AGENT_HOME} -type f -name "*.h") ) ))

.PHONY: socket_options_all socket_options_clean

socket_options_all: socket_options_app
	@echo "compiled and generated $(SOCKET_OPTIONS_LIB) successfully"

socket_options_app: $(SOCKET_OPTIONS_OBJS)
	$(CXX) -std=c++23 --shared -fPIC $^ $(LINKERS) -o $(SOCKET_OPTIONS_LIB)
	$(eval LINKERS=$(LINKERS) -L$(AGENT_HOME)/include/socket/libs -lsocket_options)

$(AGENT_HOME)/include/socket/build/%.o: $(AGENT_HOME)/include/socket/impl/%.cc
	$(CXX) -std=c++23 $(CFLAGS) -fPIC -c $< $(INCLUDES) -o $@

socket_options_mock: $(SOCKET_OPTIONS_TEST_OBJ) socket_options_all
	$(CXX) -std=c++23 -fPIC $< $(LINKERS) -o $(SOCKET_OPTIONS_BIN)/mock

$(SOCKET_OPTIONS_TEST_OBJ): $(SOCKET_OPTIONS_TEST_MAIN)
	$(CXX) -std=c++23 -fPIC $(CFLAGS) -c $< $(LINKERS) -o $@

-include $(SOCKET_OPTIONS_DEP)

socket_options_clean:
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi
	if [ $$(ls ./tests/*.o) ]; then rm ./tests/*.o ; fi

endif

