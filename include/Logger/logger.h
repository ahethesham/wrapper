#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <cstdarg>
#include <cstdio>
#include <fcntl.h>
inline void log(const char * fmt , ... ){
    va_list list;
    va_start(list , fmt);
    vfprintf(stdout ,fmt , list);
    fflush(stdout);
    printf("\n");
    fflush(stdout);

    va_end(list);
    return ;
}

#endif
