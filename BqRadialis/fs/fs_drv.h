// Made by Berkay

#ifndef FS_DRV_H
#define FS_DRV_H

#include "../bk/bk_types.h"

#include "fs_status.h"

enum FsDrvLimit: UInt32
{
    FS_DRV_MAX_DRIVERS   = 32,
    FS_DRV_NAME_CAPACITY = 32
};

enum FsDrvType: UInt32
{
    FS_DRV_TYPE_NONE      = 0,
    FS_DRV_TYPE_RAMFS     = 1,
    FS_DRV_TYPE_BLOCK     = 2,
    FS_DRV_TYPE_CHARACTER = 3,
    FS_DRV_TYPE_VIRTUAL   = 4
};

enum FsDrvState: UInt32
{
    FS_DRV_STATE_FREE       = 0,
    FS_DRV_STATE_REGISTERED = 1,
    FS_DRV_STATE_ACTIVE     = 2,
    FS_DRV_STATE_FAILED     = 3
};

struct FsDrvDriver
{
    UInt32              id;
    enum FsDrvType      type;
    enum FsDrvState     state;
    UInt8               name[FS_DRV_NAME_CAPACITY];
    enum FsStatus       (*read)(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read);
    enum FsStatus       (*write)(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written);
    enum FsStatus       (*control)(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument);
    UInt8               is_allocated;
};

struct FsDrvLedger
{
    struct FsDrvDriver drivers[FS_DRV_MAX_DRIVERS];
    UInt32             driver_count;
    UInt32             lock_id;
    volatile UInt32    ledger_lock;
    UInt8              is_initialized;
};

#endif
