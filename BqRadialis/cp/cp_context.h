// Made by Berkay

#ifndef CP_CONTEXT_H
#define CP_CONTEXT_H

#include "../bk/bk_types.h"

enum CpContextMode: UInt8
{
    CP_CONTEXT_MODE_KERNEL = 0,
    CP_CONTEXT_MODE_USER   = 1
};

struct CpContext
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

#endif
