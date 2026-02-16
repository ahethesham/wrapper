#ifndef __ADDRESS_H__
#define __ADDRESS_H__
class Address {};

namespace ipv4{

    class address : public Address
    {
        public:
            address(const char * hostname , int port );
    };
}

#endif
