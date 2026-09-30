// Made by Berkay

#include "cp_private.h"

static CpSmpLedger s_smp_ledger;

CpStatus CpSmpGetCoreCount(UInt32* core_count)
{
    if (core_count == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    *core_count = s_smp_ledger.core_count;

    return CP_STATUS_SUCCESS;
}

CpStatus CpSmpGetOnlineCount(UInt32* online_count)
{
    if (online_count == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    *online_count = s_smp_ledger.online_count;

    return CP_STATUS_SUCCESS;
}

CpStatus CpSmpGetCurrentCoreIndex(UInt32* core_index)
{
    if (core_index == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    if (s_smp_ledger.is_gs_ready != 0)
    {
        UInt32 fast_index = 0;
        __asm__ volatile ("movl %%gs:0x10, %0" : "=r"(fast_index));

        if (fast_index < s_smp_ledger.core_count)
        {
            *core_index = fast_index;
            return CP_STATUS_SUCCESS;
        }
    }

    UInt8 apic_id = 0;

    CpStatus status = CpApicGetId(&apic_id);

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    for (UInt32 i = 0; i < s_smp_ledger.core_count; i++)
    {
        if (s_smp_ledger.cores[i].apic_id == apic_id)
        {
            *core_index = i;
            return CP_STATUS_SUCCESS;
        }
    }

    return CP_STATUS_OUT_OF_BOUNDS;
}

CpStatus CpSmpGetCore(CpSmpCore** core, UInt32 core_index)
{
    if (core == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    if (core_index >= s_smp_ledger.core_count)
    {
        return CP_STATUS_OUT_OF_BOUNDS;
    }

    *core = &s_smp_ledger.cores[core_index];

    return CP_STATUS_SUCCESS;
}

CpStatus CpSmpBindCoreState(UInt32 core_index)
{
    if (core_index >= s_smp_ledger.core_count)
    {
        return CP_STATUS_OUT_OF_BOUNDS;
    }

    UInt64 gs_base = (UInt64)&s_smp_ledger.cores[core_index];
    UInt32 gs_low  = (UInt32)(gs_base & 0xFFFFFFFFULL);
    UInt32 gs_high = (UInt32)(gs_base >> 32);

    __asm__ volatile ("wrmsr" : : "c"(0xC0000101U), "a"(gs_low), "d"(gs_high) : "memory");
    __asm__ volatile ("wrmsr" : : "c"(0xC0000102U), "a"(gs_low), "d"(gs_high) : "memory");

    UInt32 efer_low  = 0;
    UInt32 efer_high = 0;
    __asm__ volatile ("rdmsr" : "=a"(efer_low), "=d"(efer_high) : "c"(0xC0000080U));
    efer_low |= 0x00000001U;
    __asm__ volatile ("wrmsr" : : "c"(0xC0000080U), "a"(efer_low), "d"(efer_high) : "memory");

    UInt32 star_low  = 0U;
    UInt32 star_high = (((UInt32)(CP_GDT_SELECTOR_KERNEL_DATA | 0x03U)) << 16) |
                       ((UInt32)CP_GDT_SELECTOR_KERNEL_CODE);
    __asm__ volatile ("wrmsr" : : "c"(0xC0000081U), "a"(star_low), "d"(star_high) : "memory");

    UInt64 lstar_addr = (UInt64)CpSyscallStub;
    UInt32 lstar_low  = (UInt32)(lstar_addr & 0xFFFFFFFFULL);
    UInt32 lstar_high = (UInt32)(lstar_addr >> 32);
    __asm__ volatile ("wrmsr" : : "c"(0xC0000082U), "a"(lstar_low), "d"(lstar_high) : "memory");

    __asm__ volatile ("wrmsr" : : "c"(0xC0000084U), "a"(0x00000200U), "d"(0U) : "memory");

    return CP_STATUS_SUCCESS;
}

CpStatus CpSmpApEntry(UInt32 core_index)
{
    CpIdtLoad(core_index);
    CpGdtLoad(core_index);
    CpTssLoad(core_index);
    CpSmpBindCoreState(core_index);
    CpApicEnable();
    CpFpuEnable(core_index);

    s_smp_ledger.cores[core_index].state = CP_SMP_CORE_STATE_ONLINE;
    __asm__ volatile
    (
        "lock incl %0"
        : "+m" (s_smp_ledger.online_count)
        :
        : "memory"
    );

    CpEnableInterrupts();

    while (1)
    {
        __asm__ volatile ("hlt");
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpSmpBootCore(UInt32 core_index, UInt64 trampoline_address, UInt32 page_table_address)
{
    UInt64 stack_top_address = CpTssGetDefaultKernelStack(core_index);

    if (stack_top_address == 0)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    UInt64 stage64_offset = (UInt64)CpSmpTrampolineStage64 - (UInt64)CpSmpTrampolineStart;

    CpSmpMailbox mailbox;

    mailbox.page_table_address = page_table_address;
    mailbox.stage64_address    = (UInt32)(trampoline_address + stage64_offset);
    mailbox.code_selector      = CP_GDT_SELECTOR_KERNEL_CODE;
    mailbox.reserved_0         = 0;
    mailbox.core_index         = core_index;
    mailbox.stack_top_address  = stack_top_address;
    mailbox.entry_address      = (UInt64)CpSmpApEntry;
    mailbox.is_ready           = 0;
    mailbox.reserved_1         = 0;
    mailbox.gdt_entries[0]     = 0x0000000000000000ULL;
    mailbox.gdt_entries[1]     = 0x00209A0000000000ULL;
    mailbox.gdt_entries[2]     = 0x0000920000000000ULL;
    mailbox.gdt_limit          = sizeof(mailbox.gdt_entries) - 1;
    mailbox.gdt_base           = trampoline_address + CP_SMP_MAILBOX_OFFSET + 0x28;

    volatile UInt8* destination = (volatile UInt8*)(trampoline_address + CP_SMP_MAILBOX_OFFSET);
    volatile UInt8* source      = (volatile UInt8*)&mailbox;

    for (UInt64 i = 0; i < sizeof(CpSmpMailbox); i++)
    {
        destination[i] = source[i];
    }

    s_smp_ledger.cores[core_index].state = CP_SMP_CORE_STATE_BOOTING;

    UInt8 target_apic_id = s_smp_ledger.cores[core_index].apic_id;
    UInt8 sipi_vector    = (UInt8)((trampoline_address >> 12) & 0xFF);

    CpApicSendIpi(target_apic_id, 0, CP_APIC_ICR_DELIVERY_INIT | CP_APIC_ICR_LEVEL_ASSERT);
    CpApicDelay(10);
    CpApicSendIpi(target_apic_id, sipi_vector, CP_APIC_ICR_DELIVERY_STARTUP | CP_APIC_ICR_LEVEL_ASSERT);

    for (UInt32 elapsed_ms = 0; elapsed_ms < 100; elapsed_ms++)
    {
        if (*((volatile UInt8*)&s_smp_ledger.cores[core_index].state) == CP_SMP_CORE_STATE_ONLINE)
        {
            return CP_STATUS_SUCCESS;
        }

        CpApicDelay(1);
    }

    if (*((volatile UInt32*)(trampoline_address + CP_SMP_MAILBOX_OFFSET + 0x20)) == 0)
    {
        s_smp_ledger.cores[core_index].state = CP_SMP_CORE_STATE_OFFLINE;
    }

    return CP_STATUS_HARDWARE_FAILURE;
}

void CpSmpSetKernelStack(UInt32 core_index, UInt64 stack_top_address)
{
    if (core_index >= s_smp_ledger.core_count)
    {
        return;
    }

    s_smp_ledger.cores[core_index].kernel_stack = stack_top_address;
}

CpStatus CpSmpInit(void)
{
    BkAcpiMadt* madt = BkGetAcpiMadt();
    if (madt == NULL)
    {
        return CP_STATUS_HARDWARE_FAILURE;
    }

    CpMadt* cp_madt = (CpMadt*)madt;

    UInt8 bsp_apic_id = 0;
    CpStatus status = CpApicGetId(&bsp_apic_id);
    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 processor_count = 0;
    UInt8* current_ptr     = (UInt8*)cp_madt + sizeof(CpMadt);
    UInt8* end_ptr         = (UInt8*)cp_madt + cp_madt->header.length;

    while ((current_ptr + sizeof(CpMadtRecordHeader)) <= end_ptr)
    {
        CpMadtRecordHeader* record = (CpMadtRecordHeader*)current_ptr;
        if (record->record_length < sizeof(CpMadtRecordHeader) || (current_ptr + record->record_length) > end_ptr)
        {
            break;
        }

        if (record->record_type == CP_MADT_TYPE_LOCAL_APIC &&
            record->record_length >= sizeof(CpMadtLocalApic))
        {
            CpMadtLocalApic* lapic = (CpMadtLocalApic*)current_ptr;
            if ((lapic->flags & (CP_MADT_LAPIC_ENABLED | CP_MADT_LAPIC_ONLINE_CAP)) != 0)
            {
                if (processor_count < CP_SMP_MAX_CORES)
                {
                    s_smp_ledger.cores[processor_count].core_index     = processor_count;
                    s_smp_ledger.cores[processor_count].processor_id   = lapic->processor_id;
                    s_smp_ledger.cores[processor_count].apic_id        = lapic->apic_id;
                    s_smp_ledger.cores[processor_count].is_bsp         = (lapic->apic_id == bsp_apic_id) ? 1 : 0;
                    s_smp_ledger.cores[processor_count].state          = (lapic->apic_id == bsp_apic_id) ? CP_SMP_CORE_STATE_ONLINE : CP_SMP_CORE_STATE_OFFLINE;
                    s_smp_ledger.cores[processor_count].thread_context = NULL;
                    s_smp_ledger.cores[processor_count].kernel_stack   = CpTssGetDefaultKernelStack(processor_count);

                    processor_count++;
                }
            }
        }

        current_ptr += record->record_length;
    }

    if (processor_count == 0)
    {
        return CP_STATUS_HARDWARE_FAILURE;
    }

    s_smp_ledger.core_count   = processor_count;
    s_smp_ledger.online_count = 1;

    for (UInt32 i = 0; i < processor_count; i++)
    {
        if (s_smp_ledger.cores[i].is_bsp != 0)
        {
            CpSmpBindCoreState(i);
            break;
        }
    }

    s_smp_ledger.is_gs_ready = 1;

    if (processor_count == 1)
    {
        return CP_STATUS_SUCCESS;
    }

    UInt64 trampoline_address = CP_SMP_TRAMPOLINE_ADDRESS;
    UInt64 trampoline_size    = (UInt64)CpSmpTrampolineEnd - (UInt64)CpSmpTrampolineStart;

    volatile UInt8* trampoline_ptr = (volatile UInt8*)trampoline_address;

    for (UInt64 i = 0; i < 4096; i++)
    {
        trampoline_ptr[i] = 0;
    }

    volatile UInt8* source_ptr = (volatile UInt8*)CpSmpTrampolineStart;

    for (UInt64 i = 0; i < trampoline_size; i++)
    {
        trampoline_ptr[i] = source_ptr[i];
    }

    UInt64 cr3_value = 0;

    __asm__ volatile ("mov %%cr3, %0" : "=r" (cr3_value));

    UInt8 any_core_booting = 0;

    for (UInt32 i = 0; i < processor_count; i++)
    {
        if (s_smp_ledger.cores[i].is_bsp == 0)
        {
            CpSmpBootCore(i, trampoline_address, (UInt32)cr3_value);

            if (s_smp_ledger.cores[i].state == CP_SMP_CORE_STATE_BOOTING)
            {
                any_core_booting = 1;
            }
        }
    }

    if (any_core_booting == 0)
    {
        for (UInt64 i = 0; i < 4096; i++)
        {
            trampoline_ptr[i] = 0;
        }
    }

    return CP_STATUS_SUCCESS;
}
