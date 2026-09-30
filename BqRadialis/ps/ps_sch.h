// Made by Berkay

#ifndef PS_SCH_H
#define PS_SCH_H

#include "../bk/bk_types.h"

enum PsSchLimit: UInt32
{
    PS_SCH_MAX_CORES          = 64,
    PS_SCH_QUEUE_CAPACITY     = 256,
    PS_SCH_PRIORITY_LEVELS    = 5,
    PS_SCH_DEFAULT_TIME_SLICE = 10,
    PS_SCH_TIMER_FREQUENCY    = 100,
    PS_SCH_TIMER_VECTOR       = 0x40,
    PS_SCH_YIELD_VECTOR       = 0xFC
};

struct PsSchQueue
{
    UInt32            thread_ids[PS_SCH_QUEUE_CAPACITY];
    UInt32            head;
    UInt32            tail;
    UInt32            count;
};

struct PsSchCore
{
    UInt32            core_index;
    UInt32            current_thread_id;
    UInt32            idle_thread_id;
    UInt32            remaining_ticks;
    struct PsSchQueue ready_queues[PS_SCH_PRIORITY_LEVELS];
    UInt8             is_active;
};

struct PsSchLedger
{
    struct PsSchCore  cores[PS_SCH_MAX_CORES];
    UInt32            core_count;
    UInt64            total_ticks;
    volatile UInt32   ledger_lock;
    UInt8             is_initialized;
};

#endif
