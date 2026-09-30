// Made by Berkay

#ifndef PS_LCK_H
#define PS_LCK_H

#include "../bk/bk_types.h"

enum PsLckLimit: UInt32
{
    PS_LCK_MAX_SPINLOCKS = 128,
    PS_LCK_MAX_MUTEXES   = 128
};

enum PsLckType: UInt32
{
    PS_LCK_TYPE_SPINLOCK = 0,
    PS_LCK_TYPE_MUTEX    = 1
};

enum PsLckState: UInt32
{
    PS_LCK_STATE_UNLOCKED = 0,
    PS_LCK_STATE_LOCKED   = 1
};

struct PsLckSpinlock
{
    UInt32          id;
    volatile UInt32 lock;
    UInt32          owner_core;
    UInt64          interrupt_state;
    UInt32          recursion_count;
    UInt8           is_allocated;
};

struct PsLckMutex
{
    UInt32          id;
    volatile UInt32 lock;
    UInt32          owner_thread_id;
    UInt32          recursion_count;
    UInt8           is_allocated;
};

struct PsLckLedger
{
    struct PsLckSpinlock spinlocks[PS_LCK_MAX_SPINLOCKS];
    struct PsLckMutex    mutexes[PS_LCK_MAX_MUTEXES];
    volatile UInt32      ledger_lock;
    UInt8                is_initialized;
};

#endif
