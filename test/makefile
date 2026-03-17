HOME=$(shell pwd)/..

INCLUDE_HEADERS := $(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ) \
				   	/usr/include/ 


INCLUDES := $(addprefix -I,$(INCLUDE_HEADERS))

LINKERS=-L/usr/lib/x86_64-linux-gnu -lssl -lcrypto

json_mock: ${HOME}/test/json_mock.cc
	${CXX} -std=c++23 $< ${INCLUDES} -o $@

server: ${HOME}/test/server.cc
	${CXX} -std=c++23  $< ${INCLUDES} ${LINKERS} -o $@ 

request_mock: ${HOME}/test/request_mock.cc
	${CXX} -std=c++23 $< ${INCLUDES} -o $@

config_mock: ${HOME}/test/config_mock.cc
	${CXX} -std=c++23 $< ${INCLUDES} -o $@

clean:
	rm ${HOME}/test/config_mock
