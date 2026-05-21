#ifndef __BASIC_ENDPOINT_INTERFACE_H__
#define __BASIC_ENDPOINT_INTERFACE_H__

#include <memory>
#include <string>
class basic_endpoint_interface {
    public:
        ~basic_endpoint_interface() = default;

        class iterator_interface{
            public:
                
                virtual iterator_interface & operator++() = 0;

                virtual struct addrinfo * operator*() = 0;
                virtual struct addrinfo *  get() = 0;
                virtual bool operator==(iterator_interface & itr) = 0;
                virtual bool has_next() = 0 ;
                virtual struct addrinfo * get_next() = 0;
        };

        virtual bool resolve() = 0;
        virtual struct addrinfo * get_info() = 0;
        virtual int get_ai_flags() = 0;
        virtual int set_ai_flags(int) = 0;
        virtual std::shared_ptr<iterator_interface> begin() = 0;
        virtual std::shared_ptr<iterator_interface> end() = 0;
        virtual const std::string & ip_address(struct addrinfo * addre) = 0;
        virtual std::string hostname() = 0;
};

#endif
