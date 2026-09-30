// Made by Berkay

#ifndef PS_THR_H
#define PS_THR_H

#include "../bk/bk_types.h"

enum PsThrLimit: UInt32
{
    PS_THR_MAX_THREADS         = 1024,
    PS_THR_DEFAULT_STACK_PAGES = 4,
    PS_THR_DEFAULT_STACK_SIZE  = 16384
};

enum PsThrState: UInt32
{
    PS_THR_STATE_FREE       = 0,
    PS_THR_STATE_READY      = 1,
    PS_THR_STATE_RUNNING    = 2,
    PS_THR_STATE_BLOCKED    = 3,
    PS_THR_STATE_SLEEPING   = 4,
    PS_THR_STATE_TERMINATED = 5
};

enum PsThrPriority: UInt32
{
    PS_THR_PRIORITY_IDLE     = 0,
    PS_THR_PRIORITY_LOW      = 1,
    PS_THR_PRIORITY_NORMAL   = 2,
    PS_THR_PRIORITY_HIGH     = 3,
    PS_THR_PRIORITY_REALTIME = 4
};

struct PsThrContext
{
    UInt64 cr3;
    UInt64 r15;
    UInt64 r14;
    UInt64 r13;
    UInt64 r12;
    UInt64 r11;
    UInt64 r10;
    UInt64 r9;
    UInt64 r8;
    UInt64 rbp;
    UInt64 rdi;
    UInt64 rsi;
    UInt64 rdx;
    UInt64 rcx;
    UInt64 rbx;
    UInt64 rax;
    UInt64 vector;
    UInt64 error_code;
    UInt64 rip;
    UInt64 cs;
    UInt64 rflags;
    UInt64 rsp;
    UInt64 ss;
    UInt64 kernel_stack_top;
    UInt8  fpu_state[512];
}
__attribute__((packed));

struct PsThrThread
{
    UInt32              thread_id;
    UInt32              process_id;
    enum PsThrState     state;
    enum PsThrPriority  priority;
    UInt32              affinity_core;
    UInt64              kernel_stack_base;
    UInt64              kernel_stack_size;
    UInt64              user_stack_base;
    UInt64              user_stack_size;
    UInt64              sleep_ticks;
    void*               entry_point;
    void*               argument;
    struct PsThrContext context __attribute__((aligned(16)));
    UInt8               is_allocated;
};

struct PsThrLedger
{
    struct PsThrThread  threads[PS_THR_MAX_THREADS];
    UInt32              thread_count;
    volatile UInt32     ledger_lock;
    UInt8               is_initialized;
};

#endif
