// Made by Berkay

#ifndef CP_CTX_H
#define CP_CTX_H

#include "../bk/bk_types.h"

#include "cp_public.h"

#define CP_CTX_MAX_CORES                64
#define CP_CTX_DEFAULT_RFLAGS           0x0202
#define CP_CTX_HW_FRAME_SIZE            184
#define CP_CTX_YIELD_VECTOR             0xFC
#define CP_CTX_PAGE_TABLE_SYNC_VECTOR   0xFD
#define CP_CTX_TLB_SHOOTDOWN_VECTOR     0xFE

enum CpCtxMode: UInt8
{
    CP_CTX_MODE_KERNEL = 0,
    CP_CTX_MODE_USER   = 1
};

struct CpCtxContext
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

struct CpCtxLedger
{
    struct CpCtxContext  active_contexts[CP_CTX_MAX_CORES] __attribute__((aligned(16)));
    struct CpCtxContext* current_contexts[CP_CTX_MAX_CORES];
    struct CpCtxContext* next_contexts[CP_CTX_MAX_CORES];
    UInt64               fault_cr2[CP_CTX_MAX_CORES];
    volatile UInt64      tlb_shootdown_address;
    volatile UInt64      tlb_shootdown_pages;
    volatile UInt32      tlb_shootdown_acks;
    volatile UInt32      tlb_shootdown_lock;
    volatile UInt64      page_table_sync_address;
    volatile UInt32      page_table_sync_acks;
    volatile UInt32      page_table_sync_lock;
    CpInterruptHandler   interrupt_handlers[256];
    CpSyscallHandler     syscall_handler;
}
__attribute__((aligned(16)));

extern void CpCtxLoad(struct CpCtxContext* context);

#endif
