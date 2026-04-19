#ifndef __INI_CONFIG_PARSER_H__
#define __INI_CONFIG_PARSER_H__
/*
 * Users will be including this 
 */

#include "basic_config_interface.h"
#include "basic_parser_interface.h"
#include "buffer.h"
#include "stream_fwd.h"
#include <map>
#include <stdexcept>

/*
 * TODO : make this follow a pimpl architecture
 */
class ini_container : basic_container_interface<std::string , std::string>{
    
    using handler = std::function<bool ( buffer_v1 * buffer )> ;
    public:
        using basic_container_interface::value_type;
        using basic_container_interface::key_type;

        ini_container(std::string & parent) ;
        ini_container(){}
        value_type & operator[](key_type  key) override;
        bool continue_reading();
        void at_eof(buffer_v1 * buffer);
        std::string parent();
        
        ini_container & push(key_type & key ,  value_type & value ) override;
        
        bool parse(buffer_v1 * buffer );

    private: 
        bool extract_parent(buffer_v1 * buffer );
        bool extract_pair(buffer_v1 * buffer);
        void trim(buffer_v1 * buffer , char * sep);
        void skip_comment(buffer_v1 * buffer );
        std::string parent_;
        handler cb_;
        std::map<std::string , std::string > children_;
};

/*
 * Concrete class of ini config parser impl in impl/ini_config_parser.cc
 */

template <typename container_policy >
class ini_config_parser : public basic_config_interface<file_reader_interface , container_policy , buffer_v1> ,
                        public basic_parser_interface<buffer_v1>{
    public:
        using container_type = container_policy;

        ini_config_parser(file_reader_interface * reader );
        //ini_config_parser(file_io_handle_interface::endpoint_reference_type  file_path_);

        bool continue_reading() override;
        void  parse(buffer_v1 * buffer) override;
        void at_eof(buffer_v1 * buffer) override;
        buffer_v1 * buffer() override;
        
        /*
         * For parent we will return the container and from there they can get the key value
         */
        container_type & operator[](std::string key) override;
        container_type & get(std::string key) override;
    
    private:
        file_reader_interface *  file_reader_;
        std::map<std::string , container_type *> storage_;
        container_type * current_context_;

};
template class ini_config_parser<ini_container>;

using Config = ini_config_parser<ini_container>;
#endif
