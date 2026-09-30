// Made by Berkay

#ifndef CP_SMP_H
#define CP_SMP_H

#include "../bk/bk_types.h"

#define CP_SMP_MAX_CORES          64
#define CP_SMP_TRAMPOLINE_ADDRESS 0x00008000ULL
#define CP_SMP_MAILBOX_OFFSET     0x0F00
#define CP_SMP_STAGE64_OFFSET     0x0040

enum CpSmpCoreState: UInt8
{
    CP_SMP_CORE_STATE_OFFLINE = 0,
    CP_SMP_CORE_STATE_BOOTING = 1,
    CP_SMP_CORE_STATE_ONLINE  = 2
};

struct CpSmpMailbox
{
    UInt32 page_table_address;
    UInt32 stage64_address;
    UInt16 code_selector;
    UInt16 reserved_0;
    UInt32 core_index;
    UInt64 stack_top_address;
    UInt64 entry_address;
    UInt32 is_ready;
    UInt32 reserved_1;
    UInt64 gdt_entries[3];
    UInt16 gdt_limit;
    UInt64 gdt_base;
}
__attribute__((packed));

struct CpSmpCore
{
    UInt64              kernel_stack;
    UInt64              user_stack;
    UInt32              core_index;
    UInt8               processor_id;
    UInt8               apic_id;
    enum CpSmpCoreState state;
    UInt8               is_bsp;
    void*               thread_context;
};

struct CpSmpLedger
{
    UInt32           core_count;
    UInt32           online_count;
    UInt8            is_gs_ready;
    struct CpSmpCore cores[CP_SMP_MAX_CORES];
}
__attribute__((aligned(16)));

extern UInt8 CpSmpTrampolineStart[];
extern UInt8 CpSmpTrampolineStage64[];
extern UInt8 CpSmpTrampolineEnd[];

#endif
