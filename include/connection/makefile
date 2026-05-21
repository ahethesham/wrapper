ifndef __connection_makefile__
__connection_makefile__ := 1

AGENT_HOME:=/home/ahethesham/agent

include $(AGENT_HOME)/include/stream/makefile
include $(AGENT_HOME)/include/web/endpoint/makefile
include $(AGENT_HOME)/include/logger/makefile
include $(AGENT_HOME)/include/builder/http/makefile
include $(AGENT_HOME)/include/socket/makefile

CFLAGS ?= -Wall -Wextra -MMD -MP

#for mock testing 
CONNECTION_TEST_MAIN := $(AGENT_HOME)/include/connection/tests/mock.cc
CONNECTION_TEST_OBJ := $(CONNECTION_TEST_MAIN:.cc=.o)
CONNECTION_BIN := $(AGENT_HOME)/include/connection/bin/mock
# for building shared libraries for users 
CONNECTION_OBJS := $(AGENT_HOME)/include/connection/build/https_connection_v1.o

CONNECTION_DEP := $(CONNECTION_OBJS:.o=.d) $(CONNECTION_TEST_OBJ:.o=.d)
INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ))
CONNECTION_LIB:= $(AGENT_HOME)/include/connection/libs/libconnection.so

.PHONY: connection_all connection_clean

connection_all: socket_io_all http_builder_all endpoint_all socket_options_all connection_app 
	@echo "compiled and generated $(CONNECTION_LIB) successfully "

connection_app: $(CONNECTION_OBJS) 
	$(CXX) -std=c++23 --shared -fPIC $(CFLAGS) $^ $(LINKERS) -o $(CONNECTION_LIB)
	$(eval LINKERS=$(LINKERS) -L$(AGENT_HOME)/include/connection/libs -lconnection)

$(AGENT_HOME)/include/connection/build/%.o: $(AGENT_HOME)/include/connection/impl/%.cc
	$(CXX) -std=c++23 $(CFLAGS) -fPIC $< -c $(INCLUDES) $(LINKERS) -o $@

connection_mock: $(CONNECTION_TEST_OBJ) connection_all
	$(CXX) -std=c++23 $(CFLAGS) $< $(LINKERS) -o $(CONNECTION_BIN)

$(CONNECTION_TEST_OBJ): $(CONNECTION_TEST_MAIN)
	$(CXX) -std=c++23 -fPIC $(CFLAGS) $< -c $(INCLUDES) -o $@


-include $(CONNECTION_DEP)

connection_clean:
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi
	if [ $$(ls ./tests/*.o) ]; then rm ./tests/*.o ; fi


endif

