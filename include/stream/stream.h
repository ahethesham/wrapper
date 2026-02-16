#ifndef __STREAM_H__
#define __STREAM_H__

#include <string>
#include <utility>

/*
 * This is not a thread safe code 
 */
template <typename fd ,
          typename reader ,
          typename writer>
class basic_stream{
    typedef typename reader::value_type reader_type;
    typedef typename reader::size_type  reader_size_type;
    typedef typename writer::size_type  writer_size_type;
    typedef typename writer::value_type writer_type;
    typedef typename fd::value_type     fd_type;

    reader_type * reader_;
    writer_type * writer_;
    fd_type     &  fd_;

    public:
        basic_stream(fd_type descriptor) : fd_(std::move(descriptor)) {}
        basic_stream(fd_type && descriptor) : fd_(descriptor) {}
        basic_stream(fd_type & descriptor) : fd_(descriptor) {}
        
        reader_size_type read( ){
            // trigger the read in the dependency object
            return reader_->read();
        }

        writer_size_type write( std::string & data ){
            return writer_->write(data);
        }
        void reset(){
            reader_->reset();
        }
};

#endif
