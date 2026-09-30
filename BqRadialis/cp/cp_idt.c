// Made by Berkay

#include "cp_private.h"

static CpIdtLedger s_idt_ledger;

void CpIdtSetEntry(UInt32 entry_index, UInt64 handler_address, UInt16 code_segment_selector, UInt8 type_attributes, UInt8 interrupt_stack_table_index)
{
    if (entry_index >= CP_IDT_ENTRY_COUNT)
    {
        return;
    }

    s_idt_ledger.entries[entry_index].offset_low                  = (UInt16)(handler_address & 0xFFFF);
    s_idt_ledger.entries[entry_index].code_segment_selector       = code_segment_selector;
    s_idt_ledger.entries[entry_index].interrupt_stack_table_index = interrupt_stack_table_index;
    s_idt_ledger.entries[entry_index].type_attributes             = type_attributes;
    s_idt_ledger.entries[entry_index].offset_middle               = (UInt16)((handler_address >> 16) & 0xFFFF);
    s_idt_ledger.entries[entry_index].offset_high                 = (UInt32)((handler_address >> 32) & 0xFFFFFFFF);
    s_idt_ledger.entries[entry_index].reserved                    = 0;
}

void CpIdtLoad(UInt32 core_index)
{
    if (core_index == 0)
    {
        for (UInt32 i = 0; i < CP_IDT_ENTRY_COUNT; i++)
        {
            CpIdtSetEntry(i, CpIsrStubTable[i], CP_GDT_SELECTOR_KERNEL_CODE, CP_IDT_GATE_INTERRUPT, CP_IDT_IST_NONE);
        }

        CpIdtSetEntry(2, CpIsrStubTable[2], CP_GDT_SELECTOR_KERNEL_CODE, CP_IDT_GATE_INTERRUPT, CP_IDT_IST_EMERGENCY);
        CpIdtSetEntry(8, CpIsrStubTable[8], CP_GDT_SELECTOR_KERNEL_CODE, CP_IDT_GATE_INTERRUPT, CP_IDT_IST_EMERGENCY);

        s_idt_ledger.hw_register.limit = (sizeof(CpIdtEntry) * CP_IDT_ENTRY_COUNT) - 1;
        s_idt_ledger.hw_register.base  = (UInt64)&s_idt_ledger.entries;
    }

    __asm__ volatile
    (
        "lidt %0"
        :
        : "m" (s_idt_ledger.hw_register)
        : "memory"
    );
}
