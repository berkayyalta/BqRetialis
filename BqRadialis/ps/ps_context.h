// Made by Berkay

#ifndef PS_CONTEXT_H
#define PS_CONTEXT_H

#include "../bk/bk_types.h"

struct PsContext
{
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
    UInt64 rip;
    UInt64 cs;
    UInt64 rflags;
    UInt64 rsp;
    UInt64 ss;
    UInt64 cr3;
    UInt8  fpu_state[512];
}
__attribute__((packed));

#endif
