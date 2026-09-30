// Made by Berkay

#include "cp_private.h"

static CpApicLedger s_apic_ledger;

CpStatus CpApicRead(UInt32* value, CpApicRegisterOffset register_offset)
{
    if (value == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    if (s_apic_ledger.is_initialized == 0)
    {
        return CP_STATUS_NOT_INITIALIZED;
    }

    if (s_apic_ledger.is_x2apic != 0)
    {
        UInt32 msr = 0x800U + ((UInt32)register_offset >> 4);
        UInt32 low = 0;
        UInt32 high = 0;
        __asm__ volatile ("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
        *value = low;
        return CP_STATUS_SUCCESS;
    }

    volatile UInt32* register_address = (volatile UInt32*)(s_apic_ledger.virtual_base + (UInt64)register_offset);

    *value = *register_address;

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicWrite(CpApicRegisterOffset register_offset, UInt32 value)
{
    if (s_apic_ledger.is_initialized == 0)
    {
        return CP_STATUS_NOT_INITIALIZED;
    }

    if (s_apic_ledger.is_x2apic != 0)
    {
        UInt32 msr = 0x800U + ((UInt32)register_offset >> 4);
        __asm__ volatile ("wrmsr" : : "c"(msr), "a"(value), "d"(0U) : "memory");
        return CP_STATUS_SUCCESS;
    }

    volatile UInt32* register_address = (volatile UInt32*)(s_apic_ledger.virtual_base + (UInt64)register_offset);

    *register_address = value;

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicGetId(UInt8* apic_id)
{
    if (apic_id == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    UInt32 raw_id = 0;

    CpStatus status = CpApicRead(&raw_id, CP_APIC_REG_ID);

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    if (s_apic_ledger.is_x2apic != 0)
    {
        *apic_id = (UInt8)(raw_id & 0xFFU);
    }
    else
    {
        *apic_id = (UInt8)((raw_id >> 24) & 0xFFU);
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicSendEoi(void)
{
    return CpApicWrite(CP_APIC_REG_EOI, 0);
}

CpStatus CpApicSendIpi(UInt8 destination_apic_id, UInt8 vector, UInt32 flags)
{
    if (s_apic_ledger.is_initialized == 0)
    {
        return CP_STATUS_NOT_INITIALIZED;
    }

    CpApicWrite(CP_APIC_REG_ERROR_STATUS, 0);

    if (s_apic_ledger.is_x2apic != 0)
    {
        UInt32 icr_low  = flags | (UInt32)vector;
        UInt32 icr_high = ((flags & 0x000C0000U) != 0U) ? 0U : (UInt32)destination_apic_id;
        __asm__ volatile ("wrmsr" : : "c"(0x830U), "a"(icr_low), "d"(icr_high) : "memory");
        return CP_STATUS_SUCCESS;
    }

    CpApicWrite(CP_APIC_REG_ICR_HIGH, ((UInt32)destination_apic_id) << 24);
    CpApicWrite(CP_APIC_REG_ICR_LOW, flags | (UInt32)vector);

    UInt32 icr_low = 0;

    for (UInt32 i = 0; i < 100000; i++)
    {
        CpApicRead(&icr_low, CP_APIC_REG_ICR_LOW);

        if ((icr_low & CP_APIC_ICR_DELIVERY_PENDING) == 0)
        {
            return CP_STATUS_SUCCESS;
        }

        __asm__ volatile ("pause");
    }

    return CP_STATUS_HARDWARE_FAILURE;
}

CpStatus CpApicIn8(UInt8* value, UInt16 port)
{
    if (value == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    UInt8 val = 0;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port) : "memory");
    *value = val;

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicOut8(UInt16 port, UInt8 value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port) : "memory");

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicCalibrateTimer(void)
{
    if (s_apic_ledger.is_initialized == 0)
    {
        return CP_STATUS_NOT_INITIALIZED;
    }

    UInt32 max_leaf = 0;
    UInt32 dummy_b  = 0;
    UInt32 dummy_c  = 0;
    UInt32 dummy_d  = 0;
    __asm__ volatile ("cpuid" : "=a"(max_leaf), "=b"(dummy_b), "=c"(dummy_c), "=d"(dummy_d) : "a"(0));

    if (max_leaf >= 0x15)
    {
        UInt32 denom      = 0;
        UInt32 numer      = 0;
        UInt32 crystal_hz = 0;
        __asm__ volatile ("cpuid" : "=a"(denom), "=b"(numer), "=c"(crystal_hz), "=d"(dummy_d) : "a"(0x15), "c"(0));

        if (crystal_hz != 0 && denom != 0 && numer != 0)
        {
            UInt64 tsc_freq_hz = ((UInt64)crystal_hz * (UInt64)numer) / (UInt64)denom;
            s_apic_ledger.ticks_per_ms = (UInt32)(tsc_freq_hz / 16000ULL);

            if (s_apic_ledger.ticks_per_ms > 0)
            {
                return CP_STATUS_SUCCESS;
            }
        }
    }

    CpApicWrite(CP_APIC_REG_TIMER_DIVIDE_CONFIG, CP_APIC_TIMER_DIVIDE_BY_16);
    CpApicWrite(CP_APIC_REG_LVT_TIMER, CP_APIC_TIMER_MODE_ONE_SHOT | CP_APIC_TIMER_MASKED);
    CpApicWrite(CP_APIC_REG_TIMER_INITIAL_COUNT, 0xFFFFFFFF);

    UInt8 gate_state = 0;
    CpApicIn8(&gate_state, 0x61);
    CpApicOut8(0x61, (UInt8)((gate_state & 0xFD) | 0x01));

    CpApicOut8(0x43, 0xB2);
    CpApicOut8(0x42, 0x9C);
    CpApicOut8(0x42, 0x2E);

    CpApicIn8(&gate_state, 0x61);
    CpApicOut8(0x61, (UInt8)(gate_state & 0xFE));
    CpApicOut8(0x61, (UInt8)(gate_state | 0x01));

    UInt8 status_byte = 0;
    do
    {
        __asm__ volatile ("pause");
        CpApicIn8(&status_byte, 0x61);
    }
    while ((status_byte & 0x20) == 0);

    UInt32 current_count = 0;
    CpApicRead(&current_count, CP_APIC_REG_TIMER_CURRENT_COUNT);

    CpApicWrite(CP_APIC_REG_LVT_TIMER, CP_APIC_TIMER_MASKED);
    CpApicWrite(CP_APIC_REG_TIMER_INITIAL_COUNT, 0);

    UInt32 elapsed_ticks = 0xFFFFFFFF - current_count;
    s_apic_ledger.ticks_per_ms = elapsed_ticks / 10;

    if (s_apic_ledger.ticks_per_ms == 0)
    {
        s_apic_ledger.ticks_per_ms = 1;
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicDelay(UInt32 milliseconds)
{
    if (s_apic_ledger.is_initialized == 0)
    {
        return CP_STATUS_NOT_INITIALIZED;
    }

    for (UInt32 i = 0; i < milliseconds; i++)
    {
        CpApicWrite(CP_APIC_REG_TIMER_DIVIDE_CONFIG, CP_APIC_TIMER_DIVIDE_BY_16);
        CpApicWrite(CP_APIC_REG_LVT_TIMER, CP_APIC_TIMER_MODE_ONE_SHOT | CP_APIC_TIMER_MASKED);
        CpApicWrite(CP_APIC_REG_TIMER_INITIAL_COUNT, s_apic_ledger.ticks_per_ms);

        UInt32 current_count = 0;

        do
        {
            __asm__ volatile ("pause");
            CpApicRead(&current_count, CP_APIC_REG_TIMER_CURRENT_COUNT);
        }
        while (current_count > 0);
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicStartTimer(UInt8 vector, UInt32 frequency)
{
    if (s_apic_ledger.is_initialized == 0)
    {
        return CP_STATUS_NOT_INITIALIZED;
    }

    if (frequency == 0 || vector < 32)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    if (s_apic_ledger.virtual_base < 0xFFFF800000000000ULL)
    {
        s_apic_ledger.virtual_base = 0xFFFF800000000000ULL + s_apic_ledger.physical_base;
    }

    UInt64 total_ticks_per_sec = (UInt64)s_apic_ledger.ticks_per_ms * 1000;
    UInt32 initial_count       = (UInt32)(total_ticks_per_sec / frequency);

    if (initial_count == 0)
    {
        initial_count = 1;
    }

    CpApicWrite(CP_APIC_REG_TIMER_DIVIDE_CONFIG, CP_APIC_TIMER_DIVIDE_BY_16);
    CpApicWrite(CP_APIC_REG_LVT_TIMER, CP_APIC_TIMER_MODE_PERIODIC | (UInt32)vector);
    CpApicWrite(CP_APIC_REG_TIMER_INITIAL_COUNT, initial_count);

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicStopTimer(void)
{
    if (s_apic_ledger.is_initialized == 0)
    {
        return CP_STATUS_NOT_INITIALIZED;
    }

    CpApicWrite(CP_APIC_REG_LVT_TIMER, CP_APIC_TIMER_MASKED);
    CpApicWrite(CP_APIC_REG_TIMER_INITIAL_COUNT, 0);

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicEnable(void)
{
    if (s_apic_ledger.is_initialized == 0)
    {
        return CP_STATUS_NOT_INITIALIZED;
    }

    if (s_apic_ledger.is_x2apic != 0)
    {
        UInt32 low  = 0;
        UInt32 high = 0;
        __asm__ volatile ("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1BU));
        low |= (1U << 11) | (1U << 10);
        __asm__ volatile ("wrmsr" : : "c"(0x1BU), "a"(low), "d"(high) : "memory");
    }

    CpApicWrite(CP_APIC_REG_ERROR_STATUS, 0);
    CpApicWrite(CP_APIC_REG_TASK_PRIORITY, 0);
    CpApicWrite(CP_APIC_REG_SPURIOUS, CP_APIC_SPURIOUS_ENABLE | CP_APIC_SPURIOUS_VECTOR);

    return CP_STATUS_SUCCESS;
}

CpStatus CpApicInit(void)
{
    BkAcpiMadt* madt = BkGetAcpiMadt();
    if (madt == NULL)
    {
        return CP_STATUS_HARDWARE_FAILURE;
    }

    CpMadt* cp_madt = (CpMadt*)madt;
    UInt64 physical_base = (UInt64)cp_madt->local_apic_address;

    UInt8* current_ptr = (UInt8*)cp_madt + sizeof(CpMadt);
    UInt8* end_ptr     = (UInt8*)cp_madt + cp_madt->header.length;
    while ((current_ptr + sizeof(CpMadtRecordHeader)) <= end_ptr)
    {
        CpMadtRecordHeader* record = (CpMadtRecordHeader*)current_ptr;
        if (record->record_length < sizeof(CpMadtRecordHeader) || (current_ptr + record->record_length) > end_ptr)
        {
            break;
        }
        if (record->record_type == CP_MADT_TYPE_LOCAL_APIC_OVERRIDE &&
            record->record_length >= sizeof(CpMadtLocalApicOverride))
        {
            CpMadtLocalApicOverride* lapic_override = (CpMadtLocalApicOverride*)current_ptr;
            physical_base = lapic_override->local_apic_address;
            break;
        }
        current_ptr += record->record_length;
    }

    if (physical_base == 0)
    {
        return CP_STATUS_HARDWARE_FAILURE;
    }

    UInt32 eax = 1U;
    UInt32 ebx = 0;
    UInt32 ecx = 0;
    UInt32 edx = 0;
    __asm__ volatile ("cpuid" : "+a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx));

    s_apic_ledger.is_x2apic = ((ecx & (1U << 21)) != 0U) ? 1 : 0;

    UInt64 virtual_base = physical_base;

    s_apic_ledger.physical_base  = physical_base;
    s_apic_ledger.virtual_base   = virtual_base;
    s_apic_ledger.ticks_per_ms   = 0;
    s_apic_ledger.is_initialized = 1;

    CpStatus status = CpApicEnable();

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    return CpApicCalibrateTimer();
}
