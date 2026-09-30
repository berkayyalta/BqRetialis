// Made by Berkay

#ifndef HW_STATUS_H
#define HW_STATUS_H

#include "../bk/bk_types.h"

enum HwStatus: UInt32
{
    HW_STATUS_SUCCESS          = 0,
    HW_STATUS_INVALID_ARGUMENT = 1,
    HW_STATUS_DEVICE_NOT_FOUND = 2,
    HW_STATUS_DEVICE_BUSY      = 3,
    HW_STATUS_TIMEOUT          = 4,
    HW_STATUS_IO_FAILURE       = 5
};

#endif
