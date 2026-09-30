// Made by Berkay

#ifndef FS_NODE_H
#define FS_NODE_H

#include "../bk/bk_types.h"

enum FsNodeLimit: UInt32
{
    FS_NODE_NAME_CAPACITY = 64
};

enum FsNodeType: UInt32
{
    FS_NODE_TYPE_NONE          = 0,
    FS_NODE_TYPE_FILE          = 1,
    FS_NODE_TYPE_DIRECTORY     = 2,
    FS_NODE_TYPE_DEVICE        = 3,
    FS_NODE_TYPE_DRIVER_IMAGE  = 4,
    FS_NODE_TYPE_SERVICE_IMAGE = 5
};

enum FsNodeFlags: UInt32
{
    FS_NODE_FLAG_NONE    = 0,
    FS_NODE_FLAG_READ    = 1U << 0,
    FS_NODE_FLAG_WRITE   = 1U << 1,
    FS_NODE_FLAG_EXECUTE = 1U << 2,
    FS_NODE_FLAG_KERNEL  = 1U << 3,
    FS_NODE_FLAG_SYSTEM  = 1U << 4
};

struct FsNode
{
    UInt32          id;
    UInt32          volume_id;
    UInt32          parent_id;
    UInt32          driver_id;
    UInt32          device_id;
    enum FsNodeType type;
    UInt32          flags;
    UInt64          size;
    UInt64          capacity;
    UInt64          data_address;
    UInt8           name[FS_NODE_NAME_CAPACITY];
};

#endif
