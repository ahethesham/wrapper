#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__

template <typename endpoint>
class basic_endpoint : public endpoint{
    typedef typename endpoint::ip_type      ip_type;
    typedef typename endpoint::port_type    port_type;

    ip_type ip_;
    port_type port_;
    public:
        basic_endpoint(int port , const char * host) : endpoint(port , host){ }
};
#endif
