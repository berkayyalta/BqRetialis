// Made by Berkay

#ifndef FS_NOD_H
#define FS_NOD_H

#include "../bk/bk_types.h"

enum FsNodLimit: UInt32
{
    FS_NOD_MAX_NODES               = 1024,
    FS_NOD_NAME_CAPACITY           = 64,
    FS_NOD_DEFAULT_BUFFER_CAPACITY = 4096
};

enum FsNodType: UInt32
{
    FS_NOD_TYPE_NONE          = 0,
    FS_NOD_TYPE_FILE          = 1,
    FS_NOD_TYPE_DIRECTORY     = 2,
    FS_NOD_TYPE_DEVICE        = 3,
    FS_NOD_TYPE_DRIVER_IMAGE  = 4,
    FS_NOD_TYPE_SERVICE_IMAGE = 5
};

enum FsNodFlags: UInt32
{
    FS_NOD_FLAG_NONE    = 0,
    FS_NOD_FLAG_READ    = 1U << 0,
    FS_NOD_FLAG_WRITE   = 1U << 1,
    FS_NOD_FLAG_EXECUTE = 1U << 2,
    FS_NOD_FLAG_KERNEL  = 1U << 3,
    FS_NOD_FLAG_SYSTEM  = 1U << 4
};

struct FsNodNode
{
    UInt32          id;
    UInt32          volume_id;
    UInt32          parent_id;
    UInt32          first_child_id;
    UInt32          next_sibling_id;
    UInt32          driver_id;
    UInt32          device_id;
    enum FsNodType  type;
    UInt32          flags;
    UInt64          size;
    UInt64          capacity;
    UInt64          data_address;
    UInt8           is_buffer_owned;
    UInt8           name[FS_NOD_NAME_CAPACITY];
    UInt8           is_allocated;
};

struct FsNodLedger
{
    struct FsNodNode nodes[FS_NOD_MAX_NODES];
    UInt32           node_count;
    UInt32           lock_id;
    volatile UInt32  ledger_lock;
    UInt8            is_initialized;
};

#endif
