// Made by Berkay

#include "cp_private.h"

static CpTssLedger s_tss_ledger;

void CpTssSetKernelStack(UInt32 core_index, UInt64 stack_top_address)
{
    if (core_index >= CP_TSS_MAX_CORES)
    {
        return;
    }

    s_tss_ledger.hw_registers[core_index].rsp[0] = stack_top_address;
}

void CpTssSetEntry(UInt32 core_index, UInt32 entry_index, UInt64 stack_top_address)
{
    if (core_index >= CP_TSS_MAX_CORES || entry_index == 0 || entry_index > CP_TSS_IST_COUNT)
    {
        return;
    }

    s_tss_ledger.hw_registers[core_index].ist[entry_index - 1] = stack_top_address;
}

UInt64 CpTssGetDefaultKernelStack(UInt32 core_index)
{
    if (core_index >= CP_TSS_MAX_CORES)
    {
        return 0;
    }

    return (UInt64)&s_tss_ledger.kernel_stacks[core_index][CP_TSS_STACK_SIZE];
}

void CpTssLoad(UInt32 core_index)
{
    if (core_index >= CP_TSS_MAX_CORES)
    {
        return;
    }

    UInt8* raw_tss = (UInt8*)&s_tss_ledger.hw_registers[core_index];

    for (UInt64 i = 0; i < sizeof(CpTssRegister); i++)
    {
        raw_tss[i] = 0;
    }

    CpTssSetKernelStack(core_index, (UInt64)&s_tss_ledger.kernel_stacks[core_index][CP_TSS_STACK_SIZE]);
    CpTssSetEntry(core_index, CP_TSS_IST_EMERGENCY, (UInt64)&s_tss_ledger.emergency_stacks[core_index][CP_TSS_STACK_SIZE]);

    s_tss_ledger.hw_registers[core_index].iopb_offset = sizeof(CpTssRegister);

    CpGdtSetTssEntry
    (
        core_index,
        5,
        (UInt64)&s_tss_ledger.hw_registers[core_index],
        sizeof(CpTssRegister) - 1,
        CP_GDT_ACCESS_TSS,
        CP_GDT_GRANULARITY_NONE
    );

    __asm__ volatile
    (
        "ltr %w0"
        :
        : "r" ((UInt16)CP_GDT_SELECTOR_TSS)
        : "memory"
    );
}
