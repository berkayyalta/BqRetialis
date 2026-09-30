// Made by Berkay

#ifndef CP_IDT_H
#define CP_IDT_H

#include "../bk/bk_types.h"

#define CP_IDT_ENTRY_COUNT   256
#define CP_IDT_IST_NONE      0x00
#define CP_IDT_IST_EMERGENCY 0x01

enum CpIdtGateType: UInt8
{
    CP_IDT_GATE_INTERRUPT = 0x8E,
    CP_IDT_GATE_TRAP      = 0x8F,
    CP_IDT_GATE_USER      = 0xEE
};

struct CpIdtEntry
{
    UInt16 offset_low;
    UInt16 code_segment_selector;
    UInt8  interrupt_stack_table_index;
    UInt8  type_attributes;
    UInt16 offset_middle;
    UInt32 offset_high;
    UInt32 reserved;
}
__attribute__((packed));

struct CpIdtRegister
{
    UInt16 limit;
    UInt64 base;
}
__attribute__((packed));

struct CpIdtLedger
{
    struct CpIdtEntry    entries[CP_IDT_ENTRY_COUNT];
    struct CpIdtRegister hw_register;
}
__attribute__((aligned(16)));

extern void   CpIsrStubDefault(void);
extern void   CpSyscallStubDefault(void);
extern UInt64 CpIsrStubTable[];

#endif
