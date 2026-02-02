#include <cstddef>

namespace system
{
    class error_code;
}
namespace web
{
    class io_context
    {
        public:
            class service;
            class id;
            class work;

            io_context() ;
            // run the event loop here 
            size_t run() ;
            // popluate the error code on return 
            size_t run(system::error_code & ec);

            // close the event loop here 
            size_t stop() ;
            // do the cleanup activities and return 
            ~io_context() ;

            void reset() {}

    };
}
 namespace impl
{
    namespace web
    {
        class io_context
        {};
    }
}
