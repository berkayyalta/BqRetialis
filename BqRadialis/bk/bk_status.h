// Made by Berkay

#ifndef BK_STATUS_H
#define BK_STATUS_H

#include "bk_types.h"

enum BkStatus: UInt32
{
    BK_STATUS_SUCCESS          = 0,
    BK_STATUS_CP_FAILURE       = 1,
    BK_STATUS_HW_FAILURE       = 2,
    BK_STATUS_MM_FAILURE       = 3,
    BK_STATUS_ACPI_FAILURE     = 4,
    BK_STATUS_INVALID_ARGUMENT = 5,
    BK_STATUS_DEVICE_NOT_FOUND = 6,
    BK_STATUS_IO_FAILURE       = 7
};

#endif
