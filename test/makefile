HOME=/Users/ahethesham/Desktop/wrapper

INCLUDE_HEADERS := $(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ) \
				   	/opt/homebrew/include/ 


INCLUDES := $(addprefix -I,$(INCLUDE_HEADERS))

LINKERS=-L/opt/homebrew/lib -lssl -lcrypto

json_mock: ${HOME}/test/json_mock.cc
	${CXX} $< ${INCLUDES} -o $@

server: ${HOME}/test/server.cc
	${CXX} -std=c++23 ${LINKERS} $< ${INCLUDES} -o $@

request_mock: ${HOME}/test/request_mock.cc
	${CXX} -std=c++23 $< ${INCLUDES} -o $@

clean:
	rm ${HOME}/test/server
