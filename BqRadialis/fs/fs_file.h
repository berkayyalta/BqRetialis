// Made by Berkay

#ifndef FS_FILE_H
#define FS_FILE_H

#include "../bk/bk_types.h"

enum FsFileMode: UInt32
{
    FS_FILE_MODE_READ     = 1U << 0,
    FS_FILE_MODE_WRITE    = 1U << 1,
    FS_FILE_MODE_APPEND   = 1U << 2,
    FS_FILE_MODE_CREATE   = 1U << 3,
    FS_FILE_MODE_TRUNCATE = 1U << 4
};

enum FsFileSeek: UInt32
{
    FS_FILE_SEEK_SET     = 0,
    FS_FILE_SEEK_CURRENT = 1,
    FS_FILE_SEEK_END     = 2
};

enum FsFileState: UInt32
{
    FS_FILE_STATE_CLOSED = 0,
    FS_FILE_STATE_OPEN   = 1,
    FS_FILE_STATE_ERROR  = 2
};

struct FsFile
{
    UInt32           id;
    UInt32           node_id;
    UInt32           volume_id;
    UInt32           process_id;
    enum FsFileMode  mode;
    enum FsFileState state;
    UInt64           offset;
    UInt64           size;
};

#endif
