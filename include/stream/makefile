ifndef __STREAM_MAKEFILE__
__STREAM_MAKEFILE__:=1

#since this is a L0 module ...any object from this module should not depend on any objects above level

AGENT_HOME:=/home/ahethesham/agent
CFLAGS ?= -Wall -Wextra -MMD -MP

FILE_IO_HANDLE=${AGENT_HOME}/include/stream/build/file_io_handle.o
FILE_READER=${AGENT_HOME}/include/stream/build/file_reader_v1.o
FILE_WRITER=${AGENT_HOME}/include/stream/build/file_writer_v1.o
HTTP_READER=${AGENT_HOME}/include/stream/build/http_reader_v1.o
HTTP_WRITER=${AGENT_HOME}/include/stream/build/http_writer_v1.o
SOCKET_IO_HANDLE=${AGENT_HOME}/include/stream/build/socket_io_handle.o
SSL_SOCKET_IO_HANDLE=$(AGENT_HOME)/include/stream/build/ssl_socket_io_handle.o

STREAM_MOCK_OBJ := $(AGENT_HOME)/include/stream/tests/mock.o
STREAM_BIN_FILE := $(AGENT_HOME)/include/stream/bin/mock
STREAM_MOCK_FILE := $(AGENT_HOME)/include/stream/tests/mock.cc

STREAM_LIB=${AGENT_HOME}/include/stream/libs/libstream.so
FILE_IO_LIB=${AGENT_HOME}/include/stream/libs/libfile_io.so
SOCKET_IO_LIB=${AGENT_HOME}/include/stream/libs/libsocket_io.so
IO_HANDLE_LIB := ${AGENT_HOME}/include/stream/libs/libio_handle.so

STREAM_ALL_OBJS=${FILE_IO_HANDLE} \
	${FILE_READER} \
	${FILE_WRITER} \
	${HTTP_READER}  \
	$(HTTP_WRITER) \
	${SOCKET_IO_HANDLE} \
	$(SSL_SOCKET_IO_HANDLE)

STREAM_DEP := $(STREAM_ALL_OBJS:.o=.d) $(STREAM_MOCK_OBJ:.o=.d)

IO_HANDLE_OBJS := ${FILE_IO_HANDLE} \
				  ${SOCKET_IO_HANDLE} \
				  $(SSL_SOCKET_IO_HANDLE) \
				  ${HTTP_READER} \
				  ${HTTP_WRITER}

INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${AGENT_HOME} -type f -name "*.h") ) ))

.PHONY: stream_all stream_clean socket_io_all

stream_all: stream_app
	echo "compiled and generated ${STREAM_LIB} successfully "

io_handle_all: ${IO_HANDLE_OBJS}
	${CXX} -std=c++23 --shared $^ ${LINKERS} -o ${IO_HANDLE_LIB}
	echo "compiled and generated ${IO_HANDLE_LIB} successfully"
	$(eval LINKERS=$(LINKERS) -L${AGENT_HOME}/include/stream/libs/ -lio_handle -L/usr/lib/x86_64-linux-gnu/ -lssl -lcrypto)

# core target responsible for generating libstream.so
stream_app: ${STREAM_ALL_OBJS}
	${CXX} -std=c++23 --shared -fPIC $^ -o ${STREAM_LIB} 
	$(eval LINKERS=$(LINKERS) -L${AGENT_HOME}/include/stream/libs/ -lstream -L/usr/lib/x86_64-linux-gnu/ -lssl -lcrypto )

${AGENT_HOME}/include/stream/build/%.o: ${AGENT_HOME}/include/stream/impl/%.cc 
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@

stream_mock: $(STREAM_MOCK_OBJ) stream_app
	$(CXX) -std=c++23 -fPIC $< $(LINKERS) -o $(STREAM_BIN_FILE)

$(STREAM_MOCK_OBJ): $(STREAM_MOCK_FILE)
	${CXX} $(CFLAGS) -std=c++23 -fPIC -c $< ${INCLUDES} -o $@
#--------------------------------------------------------------
# for users who specifically need a file_writer and file_reader
file_io_all: ${FILE_READER} ${FILE_WRITER} ${FILE_IO_HANDLE}
	${CXX} -std=c++23 --shared -fPIC $^ ${LINKERS} ${INCLUDES} -o ${FILE_IO_LIB}
	$(eval LINKERS=$(LINKERS) -L${AGENT_HOME}/include/stream/libs/ -lfile_io)

socket_io_all: io_handle_all
	echo "generated socket handle object "
-include $(STREAM_DEP)

stream_clean:
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi
	if [ $$(ls ./tests/*.o | wc -l ) -gt 0 ]; then rm ./tests/*.o ; fi


endif
