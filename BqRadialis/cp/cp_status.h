// Made by Berkay

#ifndef CP_STATUS_H
#define CP_STATUS_H

#include "../bk/bk_types.h"

enum CpStatus: UInt32
{
    CP_STATUS_SUCCESS             = 0,
    CP_STATUS_INVALID_ARGUMENT    = 1,
    CP_STATUS_OUT_OF_BOUNDS       = 2,
    CP_STATUS_SLOT_OCCUPIED       = 3,
    CP_STATUS_UNSUPPORTED_FEATURE = 4,
    CP_STATUS_NOT_INITIALIZED     = 5,
    CP_STATUS_HARDWARE_FAILURE    = 6
};

#endif
