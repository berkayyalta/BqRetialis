// Made by Berkay

#ifndef FS_FIL_H
#define FS_FIL_H

#include "../bk/bk_types.h"

enum FsFilLimit: UInt32
{
    FS_FIL_MAX_FILES = 256
};

enum FsFilMode: UInt32
{
    FS_FIL_MODE_READ     = 1U << 0,
    FS_FIL_MODE_WRITE    = 1U << 1,
    FS_FIL_MODE_APPEND   = 1U << 2,
    FS_FIL_MODE_CREATE   = 1U << 3,
    FS_FIL_MODE_TRUNCATE = 1U << 4
};

enum FsFilSeek: UInt32
{
    FS_FIL_SEEK_SET     = 0,
    FS_FIL_SEEK_CURRENT = 1,
    FS_FIL_SEEK_END     = 2
};

enum FsFilState: UInt32
{
    FS_FIL_STATE_CLOSED = 0,
    FS_FIL_STATE_OPEN   = 1,
    FS_FIL_STATE_ERROR  = 2
};

struct FsFilFile
{
    UInt32           id;
    UInt32           node_id;
    UInt32           volume_id;
    UInt32           process_id;
    enum FsFilMode   mode;
    enum FsFilState  state;
    UInt64           offset;
    UInt64           size;
    UInt8            is_allocated;
};

struct FsFilLedger
{
    struct FsFilFile files[FS_FIL_MAX_FILES];
    UInt32           file_count;
    UInt32           lock_id;
    volatile UInt32  ledger_lock;
    UInt8            is_initialized;
};

#endif
