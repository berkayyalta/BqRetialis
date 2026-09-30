// Made by Berkay

#include "cp_private.h"

static CpGdtLedger s_gdt_ledger;

void CpGdtSetEntry(UInt32 core_index, UInt32 entry_index, UInt8 access_rights, UInt8 granularity)
{
    if (core_index >= CP_GDT_MAX_CORES || entry_index >= CP_GDT_ENTRY_COUNT)
    {
        return;
    }

    s_gdt_ledger.entries[core_index][entry_index].limit_low     = 0x0000;
    s_gdt_ledger.entries[core_index][entry_index].base_low      = 0x0000;
    s_gdt_ledger.entries[core_index][entry_index].base_middle   = 0x00;
    s_gdt_ledger.entries[core_index][entry_index].access_rights = access_rights;
    s_gdt_ledger.entries[core_index][entry_index].granularity   = granularity;
    s_gdt_ledger.entries[core_index][entry_index].base_high     = 0x00;
}

void CpGdtSetTssEntry(UInt32 core_index, UInt32 entry_index, UInt64 base_address, UInt32 limit, UInt8 access_rights, UInt8 granularity)
{
    if (core_index >= CP_GDT_MAX_CORES || entry_index >= (CP_GDT_ENTRY_COUNT - 1))
    {
        return;
    }

    CpTssEntry tss_entry;

    tss_entry.limit_low     = (UInt16)(limit & 0xFFFF);
    tss_entry.base_low      = (UInt16)(base_address & 0xFFFF);
    tss_entry.base_middle   = (UInt8)((base_address >> 16) & 0xFF);
    tss_entry.access_rights = access_rights;
    tss_entry.granularity   = (UInt8)(((limit >> 16) & 0x0F) | (granularity & 0xF0));
    tss_entry.base_high     = (UInt8)((base_address >> 24) & 0xFF);
    tss_entry.base_upper    = (UInt32)((base_address >> 32) & 0xFFFFFFFF);
    tss_entry.reserved      = 0;

    *((CpTssEntry*)&s_gdt_ledger.entries[core_index][entry_index]) = tss_entry;
}

void CpGdtLoad(UInt32 core_index)
{
    if (core_index >= CP_GDT_MAX_CORES)
    {
        return;
    }

    CpGdtSetEntry(core_index, 0, CP_GDT_ACCESS_NULL,        CP_GDT_GRANULARITY_NONE);
    CpGdtSetEntry(core_index, 1, CP_GDT_ACCESS_KERNEL_CODE, CP_GDT_GRANULARITY_64BIT);
    CpGdtSetEntry(core_index, 2, CP_GDT_ACCESS_KERNEL_DATA, CP_GDT_GRANULARITY_NONE);
    CpGdtSetEntry(core_index, 3, CP_GDT_ACCESS_USER_DATA,   CP_GDT_GRANULARITY_NONE);
    CpGdtSetEntry(core_index, 4, CP_GDT_ACCESS_USER_CODE,   CP_GDT_GRANULARITY_64BIT);

    s_gdt_ledger.hw_registers[core_index].limit = (sizeof(CpGdtEntry) * CP_GDT_ENTRY_COUNT) - 1;
    s_gdt_ledger.hw_registers[core_index].base  = (UInt64)&s_gdt_ledger.entries[core_index];

    __asm__ volatile
    (
        "lgdt %0\n\t"
        "mov %w1, %%ds\n\t"
        "mov %w1, %%es\n\t"
        "mov %w1, %%fs\n\t"
        "mov %w1, %%gs\n\t"
        "mov %w1, %%ss\n\t"
        "pushq %q2\n\t"
        "lea 1f(%%rip), %%rax\n\t"
        "pushq %%rax\n\t"
        "lretq\n\t"
        "1:\n\t"
        :
        : "m" (s_gdt_ledger.hw_registers[core_index]),
            "r" ((UInt16)CP_GDT_SELECTOR_KERNEL_DATA),
            "r" ((UInt64)CP_GDT_SELECTOR_KERNEL_CODE)
        : "rax", "memory"
    );
}
