// Made by Berkay

#ifndef FS_DRIVER_H
#define FS_DRIVER_H

#include "../bk/bk_types.h"

#include "fs_status.h"

enum FsDriverLimit: UInt32
{
    FS_DRIVER_NAME_CAPACITY = 32
};

enum FsDriverType: UInt32
{
    FS_DRIVER_TYPE_NONE      = 0,
    FS_DRIVER_TYPE_RAMFS     = 1,
    FS_DRIVER_TYPE_BLOCK     = 2,
    FS_DRIVER_TYPE_CHARACTER = 3,
    FS_DRIVER_TYPE_VIRTUAL   = 4
};

enum FsDriverState: UInt32
{
    FS_DRIVER_STATE_FREE       = 0,
    FS_DRIVER_STATE_REGISTERED = 1,
    FS_DRIVER_STATE_ACTIVE     = 2,
    FS_DRIVER_STATE_FAILED     = 3
};

struct FsDriver
{
    UInt32             id;
    enum FsDriverType  type;
    enum FsDriverState state;
    UInt8              name[FS_DRIVER_NAME_CAPACITY];
    enum FsStatus      (*read)(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read);
    enum FsStatus      (*write)(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written);
    enum FsStatus      (*control)(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument);
};

#endif
