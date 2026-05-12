#ifndef __HTTP_STATUS_CODES_H__
#define __HTTP_STATUS_CODES_H__
#include <string>
struct http_status;

http_status & status(int status);

typedef struct http_status {
    int code;
    std::string message;
    const static inline http_status & clone(const http_status & status){
        return ::status(status.code);
    }
}http_status;

// will return above struct with http message in it

#endif
