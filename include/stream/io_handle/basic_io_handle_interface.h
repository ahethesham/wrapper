#ifndef __BASIC_IO_HANDLE_INTERFACE_H__
#define __BASIC_IO_HANDLE_INTERFACE_H__

/*
 * It could be a basic_io_handle_interface< SSL * , sockaddr *> for ssl socket interfaces
 *  It could be a basic_io_handle_interface< int , sockaddr *> for socket
 *  It could be a basic_io_handle_interface< FILE* , char *> for file io
 */

template<typename fd_type_policy ,
         typename endpoint_reference_type_policy >
class basic_io_handle_interface{
    public:

        using fd_type = fd_type_policy;
        using endpoint_reference_type = endpoint_reference_type_policy;
        using self_type = basic_io_handle_interface<fd_type , endpoint_reference_type>;

        virtual ~basic_io_handle_interface(){}

        
        virtual fd_type get() = 0;
        
        virtual fd_type open() = 0;
        
        virtual bool is_closed() = 0;

        virtual basic_io_handle_interface & close() = 0;


};
#endif
