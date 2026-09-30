// Made by Berkay

#ifndef FS_STATUS_H
#define FS_STATUS_H

#include "../bk/bk_types.h"

enum FsStatus: UInt32
{
    FS_STATUS_SUCCESS           = 0,
    FS_STATUS_INVALID_PARAMETER = 1,
    FS_STATUS_OUT_OF_MEMORY     = 2,
    FS_STATUS_OUT_OF_RESOURCES  = 3,
    FS_STATUS_NOT_FOUND         = 4,
    FS_STATUS_ALREADY_EXISTS    = 5,
    FS_STATUS_NOT_SUPPORTED     = 6,
    FS_STATUS_BUSY              = 7,
    FS_STATUS_END_OF_FILE       = 8,
    FS_STATUS_INVALID_PATH      = 9,
    FS_STATUS_ACCESS_DENIED     = 10,
    FS_STATUS_IO_ERROR          = 11,
    FS_STATUS_INTERNAL_ERROR    = 12
};

#endif
