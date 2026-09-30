// Made by Berkay

#ifndef CP_TSS_H
#define CP_TSS_H

#include "../bk/bk_types.h"

#define CP_TSS_MAX_CORES     64
#define CP_TSS_IST_COUNT     7
#define CP_TSS_IST_EMERGENCY 1
#define CP_TSS_STACK_SIZE    4096

struct CpTssEntry
{
    UInt16 limit_low;
    UInt16 base_low;
    UInt8  base_middle;
    UInt8  access_rights;
    UInt8  granularity;
    UInt8  base_high;
    UInt32 base_upper;
    UInt32 reserved;
}
__attribute__((packed));

struct CpTssRegister
{
    UInt32 reserved_0;
    UInt64 rsp[3];
    UInt64 reserved_1;
    UInt64 ist[7];
    UInt64 reserved_2;
    UInt16 reserved_3;
    UInt16 iopb_offset;
}
__attribute__((packed));

struct CpTssLedger
{
    UInt8                kernel_stacks[CP_TSS_MAX_CORES][CP_TSS_STACK_SIZE] __attribute__((aligned(16)));
    UInt8                emergency_stacks[CP_TSS_MAX_CORES][CP_TSS_STACK_SIZE] __attribute__((aligned(16)));
    struct CpTssRegister hw_registers[CP_TSS_MAX_CORES];
}
__attribute__((aligned(16)));

#endif
