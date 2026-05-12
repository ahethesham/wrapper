#ifndef __V4_ENDPOINT_H__
#define __V4_ENDPOINT_H__

#include "basic_endpoint_interface.h"
#include <memory>

/*
 * should be able to resolve the hostname to a list of ipv4 addresses and that list should be iterable
 */
class v4_tcp_endpoint_v1 : public basic_endpoint_interface{
    public:
        // user will only be giving the hostname we need to resolve it to
        // the particular ip address
        v4_tcp_endpoint_v1(const char * host , int port );

        bool resolve() override;
        struct addrinfo * get_info() override;
        int get_ai_flags() override;
        int set_ai_flags(int flags) override;
        const std::string & ip_address(struct addrinfo * addr) override;

        class iterator : public basic_endpoint_interface::iterator_interface{
            public:
                iterator(struct addrinfo * list);
                iterator(iterator &);
                iterator(iterator &&) ;
                
                iterator & operator++() override;

                struct addrinfo * operator*() override;
                struct addrinfo * get() override;
                bool operator==(iterator_interface & itr) override;
                bool has_next() override;
                struct addrinfo * get_next() override;
            private:
                struct addrinfo * next();
                struct addrinfo * end();
                struct addrinfo * current_;
        };

        iterator & begin() ;
        iterator & end();
        std::string hostname() override;
               
    public:
        class impl;
        std::shared_ptr<impl> impl_;
};

#endif
