HOME=/home/ahethesham/agent/

INCLUDES=-I${HOME}/include \
		 -I${HOME}/include/json \
		 -I${HOME}/include/web
LINKERS=

json_mock: ${HOME}/test/json_mock.cc
	${CXX} ${HOME}/test/json_mock.cc ${INCLUDES} -o ${HOME}/test/json_mock

request_mock: ${HOME}/test/request_mock.cc
	${CXX} ${HOME}/test/request_mock.cc ${INCLUDES} -o ${HOME}/test/request_mock

clean:
#	rm ${HOME}/test/json_mock
	rm ${HOME}/test/request_mock
