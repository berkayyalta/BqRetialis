// Made by Berkay

#ifndef FS_VOL_H
#define FS_VOL_H

#include "../bk/bk_types.h"

enum FsVolLimit: UInt32
{
    FS_VOL_MAX_VOLUMES   = 32,
    FS_VOL_PATH_CAPACITY = 128
};

enum FsVolType: UInt32
{
    FS_VOL_TYPE_NONE   = 0,
    FS_VOL_TYPE_RAMFS  = 1,
    FS_VOL_TYPE_INITRD = 2,
    FS_VOL_TYPE_DEVICE = 3
};

enum FsVolState: UInt32
{
    FS_VOL_STATE_FREE      = 0,
    FS_VOL_STATE_UNMOUNTED = 1,
    FS_VOL_STATE_MOUNTED   = 2,
    FS_VOL_STATE_READ_ONLY = 3
};

struct FsVolVolume
{
    UInt32              id;
    UInt32              driver_id;
    UInt32              root_node_id;
    enum FsVolType      type;
    enum FsVolState     state;
    UInt64              total_size;
    UInt64              used_size;
    UInt8               mount_path[FS_VOL_PATH_CAPACITY];
    UInt8               is_allocated;
};

struct FsVolLedger
{
    struct FsVolVolume volumes[FS_VOL_MAX_VOLUMES];
    UInt32             volume_count;
    UInt32             lock_id;
    volatile UInt32    ledger_lock;
    UInt8              is_initialized;
};

#endif
