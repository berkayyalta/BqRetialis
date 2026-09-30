// Made by Berkay

#ifndef PS_STATUS_H
#define PS_STATUS_H

#include "../bk/bk_types.h"

enum PsStatus: UInt32
{
    PS_STATUS_SUCCESS           = 0,
    PS_STATUS_INVALID_PARAMETER = 1,
    PS_STATUS_OUT_OF_MEMORY     = 2,
    PS_STATUS_OUT_OF_RESOURCES  = 3,
    PS_STATUS_NOT_FOUND         = 4,
    PS_STATUS_BUSY              = 5,
    PS_STATUS_INVALID_STATE     = 6,
    PS_STATUS_INTERNAL_ERROR    = 7
};

#endif
