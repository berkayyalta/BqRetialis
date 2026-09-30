// Made by Berkay

#ifndef BK_PANIC_H
#define BK_PANIC_H

#include "bk_types.h"

struct BkPanicReport
{
    const char* message;
    const char* file;
    const char* function;
    UInt32 line;
};

#endif
