// Made by Berkay

#ifndef FS_VOLUME_H
#define FS_VOLUME_H

#include "../bk/bk_types.h"

enum FsVolumeLimit: UInt32
{
    FS_VOLUME_PATH_CAPACITY = 128
};

enum FsVolumeType: UInt32
{
    FS_VOLUME_TYPE_NONE   = 0,
    FS_VOLUME_TYPE_RAMFS  = 1,
    FS_VOLUME_TYPE_INITRD = 2,
    FS_VOLUME_TYPE_DEVICE = 3
};

enum FsVolumeState: UInt32
{
    FS_VOLUME_STATE_FREE      = 0,
    FS_VOLUME_STATE_UNMOUNTED = 1,
    FS_VOLUME_STATE_MOUNTED   = 2,
    FS_VOLUME_STATE_READ_ONLY = 3
};

struct FsVolume
{
    UInt32             id;
    UInt32             driver_id;
    UInt32             root_node_id;
    enum FsVolumeType  type;
    enum FsVolumeState state;
    UInt64             total_size;
    UInt64             used_size;
    UInt8              mount_path[FS_VOLUME_PATH_CAPACITY];
};

#endif
