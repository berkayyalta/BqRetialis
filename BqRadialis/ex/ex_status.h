// Made by Berkay

#ifndef EX_STATUS_H
#define EX_STATUS_H

#include "../bk/bk_types.h"

enum ExStatus: UInt32
{
    EX_STATUS_SUCCESS          = 0,
    EX_STATUS_OK               = 0,
    EX_STATUS_INVALID_ARGUMENT = 1,
    EX_STATUS_ACCESS_DENIED    = 2,
    EX_STATUS_INVALID_SYSCALL  = 3,
    EX_STATUS_OUT_OF_MEMORY    = 4,
    EX_STATUS_NOT_INITIALIZED  = 5,
    EX_STATUS_NOT_FOUND        = 6,
    EX_STATUS_BUSY             = 7,
    EX_STATUS_NOT_SUPPORTED    = 8,
    EX_STATUS_INTERNAL_ERROR   = 9
};

#endif
