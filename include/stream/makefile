
HOME ?= $(shell find ~ -type d -name agent | head -n 1)

FILE_IO_HANDLE=${HOME}/include/stream/build/file_io_handle.o
FILE_READER=${HOME}/include/stream/build/file_reader_v1.o
FILE_WRITER=${HOME}/include/stream/build/file_writer_v1.o
HTTP_READER=${HOME}/include/stream/build/http_reader_v1.o
SOCKET_IO_HANDLE=${HOME}/include/stream/build/socket_io_handle.o

STREAM_LIB=${HOME}/include/stream/libs/libstream.so

OBJS=${FILE_IO_HANDLE} \
	${FILE_READER} \
	${FILE_WRITER} \
	${HTTP_READER}  \
	${SOCKET_IO_HANDLE}


INCLUDES ?= $(addprefix -I,$(sort $(dir $(shell find ${HOME} -type f -name "*.h") ) ))

#LINKERS=-L/usr/lib/x86_64-linux-gnu -lssl -lcrypto
stream_all: ${OBJS}
	${CXX} -std=c++23 --shared -fPIC $^ -o ${STREAM_LIB} 


mock_http_reader: ${HOME}/include/stream/build/http_reader_v1.o ${TEST_OBJ}
	${CXX} -std=c++23 $^ ${INCLUDES} -o ${HOME}/include/stream/build/http_reader_v1 
	
mock_file_reader: ${HOME}/include/stream/build/file_reader_v1.o ${HOME}/include/stream/build/file_io_handle.o ${TEST_MAIN}
	${CXX} -std=c++23 --shared ${HOME}/include/stream/build/file_reader_v1.o ${HOME}/include/stream/build/file_io_handle.o  -o ${HOME}/include/stream/libs/libfile_reader.so
	${CXX} -std=c++23  ${TEST_MAIN} ${INCLUDES} -L${HOME}/include/stream/libs/ -lfile_reader   -o ${HOME}/include/stream/bin/file_reader_v1

mock_file_io_handle: ${HOME}/include/stream/build/file_io_handle.o 

file_reader: ${HOME}/include/stream/build/file_reader_v1.o ${HOME}/include/stream/build/file_io_handle.o
	${CXX} -std=c++23 --shared $^ -o ${HOME}/include/stream/libs/libfile_reader.so


file_writer: ${HOME}/include/stream/build/file_io_handle.o  ${HOME}/include/stream/build/file_writer_v1.o
	${CXX} -std=c++23 -fPIC --shared $^ -o ${HOME}/include/stream/libs/libfile_writer.so

mock_file_writer: ${HOME}/include/stream/build/file_io_handle.o ${HOME}/include/stream/build/file_writer_v1.o  ${HOME}/include/stream/build/mock.o
	${CXX} -std=c++23 $^ -o ${HOME}/include/stream/bin/mock

${HOME}/include/stream/build/%.o: ${HOME}/include/stream/impl/%.cc 
	${CXX} -std=c++23 -fPIC -c $< ${INCLUDES} -o $@


clean:
	if [ $$(ls ./libs/ | wc -l ) -gt 0 ]; then rm ./libs/* ; fi
	if [ $$(ls ./build/ | wc -l ) -gt 0 ]; then rm ./build/* ; fi
	if [ $$(ls ./bin/ | wc -l ) -gt 0 ]; then rm ./bin/* ; fi
