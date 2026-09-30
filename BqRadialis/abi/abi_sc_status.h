// Made by Berkay

#ifndef ABI_SC_STATUS_H
#define ABI_SC_STATUS_H

#include "../bk/bk_types.h"

enum AbiScStatus: UInt32
{
    ABI_SC_STATUS_SUCCESS          = 0,
    ABI_SC_STATUS_INVALID_ARGUMENT = 1,
    ABI_SC_STATUS_ACCESS_DENIED    = 2,
    ABI_SC_STATUS_NOT_FOUND        = 3,
    ABI_SC_STATUS_OUT_OF_MEMORY    = 4,
    ABI_SC_STATUS_OUT_OF_BOUNDS    = 5,
    ABI_SC_STATUS_BUSY             = 6,
    ABI_SC_STATUS_NOT_READY        = 7,
    ABI_SC_STATUS_NOT_SUPPORTED    = 8,
    ABI_SC_STATUS_ALREADY_EXISTS   = 9,
    ABI_SC_STATUS_IO_ERROR         = 10,
    ABI_SC_STATUS_END_OF_FILE      = 11,
    ABI_SC_STATUS_INVALID_SYSCALL  = 12,
    ABI_SC_STATUS_HARDWARE_FAILURE = 13,
    ABI_SC_STATUS_INTERNAL_ERROR   = 14
};

typedef enum AbiScStatus AbiScStatus;

#endif
