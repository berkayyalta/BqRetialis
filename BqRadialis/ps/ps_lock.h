// Made by Berkay

#ifndef PS_LOCK_H
#define PS_LOCK_H

#include "../bk/bk_types.h"

enum PsLockType: UInt32
{
    PS_LOCK_TYPE_SPINLOCK = 0,
    PS_LOCK_TYPE_MUTEX    = 1
};

enum PsLockState: UInt32
{
    PS_LOCK_STATE_UNLOCKED = 0,
    PS_LOCK_STATE_LOCKED   = 1
};

struct PsLock
{
    UInt32           id;
    enum PsLockType  type;
    enum PsLockState state;
    UInt32           owner_thread_id;
    UInt32           recursion_count;
};

#endif
