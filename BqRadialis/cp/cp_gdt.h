// Made by Berkay

#ifndef CP_GDT_H
#define CP_GDT_H

#include "../bk/bk_types.h"

#define CP_GDT_MAX_CORES   64
#define CP_GDT_ENTRY_COUNT 7

enum CpGdtSelector: UInt16
{
    CP_GDT_SELECTOR_NULL        = 0x00,
    CP_GDT_SELECTOR_KERNEL_CODE = 0x08,
    CP_GDT_SELECTOR_KERNEL_DATA = 0x10,
    CP_GDT_SELECTOR_USER_DATA   = 0x18,
    CP_GDT_SELECTOR_USER_CODE   = 0x20,
    CP_GDT_SELECTOR_TSS         = 0x28
};

enum CpGdtAccess: UInt8
{
    CP_GDT_ACCESS_NULL        = 0x00,
    CP_GDT_ACCESS_TSS         = 0x89,
    CP_GDT_ACCESS_KERNEL_DATA = 0x92,
    CP_GDT_ACCESS_KERNEL_CODE = 0x9A,
    CP_GDT_ACCESS_USER_DATA   = 0xF2,
    CP_GDT_ACCESS_USER_CODE   = 0xFA
};

enum CpGdtGranularity: UInt8
{
    CP_GDT_GRANULARITY_NONE  = 0x00,
    CP_GDT_GRANULARITY_64BIT = 0x20
};

struct CpGdtEntry
{
    UInt16 limit_low;
    UInt16 base_low;
    UInt8  base_middle;
    UInt8  access_rights;
    UInt8  granularity;
    UInt8  base_high;
}
__attribute__((packed));

struct CpGdtRegister
{
    UInt16 limit;
    UInt64 base;
}
__attribute__((packed));

struct CpGdtLedger
{
    struct CpGdtEntry    entries[CP_GDT_MAX_CORES][CP_GDT_ENTRY_COUNT];
    struct CpGdtRegister hw_registers[CP_GDT_MAX_CORES];
}
__attribute__((aligned(16)));

#endif
