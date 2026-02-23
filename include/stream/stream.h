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
    typedef  reader reader_type;
    typedef typename reader::size_type  reader_size_type;
    typedef typename writer::size_type  writer_size_type;
    typedef typename reader::custom_protocol reader_custom_protocol;
    typedef typename writer::custom_protocol writer_custom_protocol;
    typedef  writer writer_type;
    typedef  fd     fd_type;

    reader_type * reader_;
    writer_type * writer_;
    fd_type     &  fd_;

    public:
        basic_stream(fd_type descriptor) : fd_(descriptor) {
            reader_ = new reader_type(descriptor);
            writer_ = new writer_type(descriptor);
        }
//        basic_stream(fd_type && descriptor) : fd_(descriptor) {
//            reader_ = new reader_type(descriptor);
//            writer_ = new writer_type(descriptor);
//        }
//        basic_stream(fd_type & descriptor) : fd_(descriptor) {
//            reader_ = new reader_type(descriptor);
//            writer_ = new writer_type(descriptor);
//        }
        
        reader_custom_protocol read( ){
            // trigger the read in the dependency object
            return reader_->read();
        }

        writer_custom_protocol write( std::string & data ){
            return writer_->write(data);
        }

        writer_size_type write(writer_custom_protocol & data){
            return writer_->write(data);
        }

        void reset(){
            reader_->reset();
        }
};


#endif
