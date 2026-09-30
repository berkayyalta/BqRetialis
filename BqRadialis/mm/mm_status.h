// Made by Berkay

#ifndef MM_STATUS_H
#define MM_STATUS_H

#include "../bk/bk_types.h"

enum MmStatus: UInt32
{
    MM_STATUS_SUCCESS           = 0,
    MM_STATUS_INVALID_ARGUMENT  = 1,
    MM_STATUS_OUT_OF_BOUNDS     = 2,
    MM_STATUS_UNALIGNED_ADDRESS = 3,
    MM_STATUS_OUT_OF_MEMORY     = 4,
    MM_STATUS_ALREADY_FREE      = 5,
    MM_STATUS_ALREADY_MAPPED    = 6,
    MM_STATUS_NOT_MAPPED        = 7
};

#endif
