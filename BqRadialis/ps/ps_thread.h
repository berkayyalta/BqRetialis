// Made by Berkay

#ifndef PS_THREAD_H
#define PS_THREAD_H

#include "../bk/bk_types.h"

#include "ps_context.h"

enum PsThreadState: UInt32
{
    PS_THREAD_STATE_FREE       = 0,
    PS_THREAD_STATE_READY      = 1,
    PS_THREAD_STATE_RUNNING    = 2,
    PS_THREAD_STATE_BLOCKED    = 3,
    PS_THREAD_STATE_SLEEPING   = 4,
    PS_THREAD_STATE_TERMINATED = 5
};

enum PsThreadPriority: UInt32
{
    PS_THREAD_PRIORITY_IDLE     = 0,
    PS_THREAD_PRIORITY_LOW      = 1,
    PS_THREAD_PRIORITY_NORMAL   = 2,
    PS_THREAD_PRIORITY_HIGH     = 3,
    PS_THREAD_PRIORITY_REALTIME = 4
};

typedef void (*PsThreadEntry)(void* argument);

struct PsThread
{
    UInt32                thread_id;
    UInt32                process_id;
    enum PsThreadState    state;
    enum PsThreadPriority priority;
    UInt32                affinity_core;
    UInt64                kernel_stack_base;
    UInt64                kernel_stack_size;
    UInt64                user_stack_base;
    UInt64                user_stack_size;
    UInt64                sleep_ticks;
    struct PsContext      context;
};

#endif
