#ifndef __STREAM_H__
#define __STREAM_H__

#include <string>
#include "logger.h"
#include "stream_fwd.h"

#if 0
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
    fd_type     fd_;

    public:
        basic_stream(fd_type descriptor) : fd_(descriptor) {
            log("initializing reader");
            reader_ = new reader_type(descriptor);
            log("initializing writer ");
            writer_ = new writer_type(descriptor);
        }
        
        reader_custom_protocol read( ){
            // trigger the read in the dependency object
            return reader_->read();
        }

        writer_custom_protocol write( std::string & data ){
            return writer_->write(data.c_str());
        }

        writer_size_type write(writer_custom_protocol & data){
            return writer_->write(data);
        }

        void reset(){
            reader_->reset();
        }
};
#else

template < typename istream_policy ,
           typename ostream_policy >
class basic_stream{
    
    istream_policy * istream_;
    ostream_policy * ostream_;
    // read to the input
    using isocket_type = istream_policy::socket_type ;
    using istream_return_type =  istream_policy::return_type ;
    using ostream_protocol_type = ostream_policy::protocol_type;
    // write to the output
    typedef ostream_policy::return_type ostream_return_type;

    public:
        basic_stream(isocket_type socket) : istream_(new istream_policy(socket) ) ,
                                      ostream_(new ostream_policy(socket))  {} 

        basic_stream(isocket_type * socket) : istream_(new istream_policy(socket) ) ,
                                      ostream_(new ostream_policy(socket))  {} 

        istream_return_type read(){
            return istream_->read();
        }

        basic_stream(const char * file_path) : istream_(new istream_policy(file_path)) ,
                                               ostream_(new ostream_policy(file_path)){}

        
        ostream_return_type write(){
            return ostream_->write();
        }
        ostream_return_type write(ostream_protocol_type & body){
            return ostream_->write(body);
        }

};

#endif

#endif
