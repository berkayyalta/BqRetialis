// Made by Berkay

#ifndef CP_APIC_H
#define CP_APIC_H

#include "../bk/bk_types.h"

enum CpApicRegisterOffset: UInt32
{
    CP_APIC_REG_ID                  = 0x0020,
    CP_APIC_REG_VERSION             = 0x0030,
    CP_APIC_REG_TASK_PRIORITY       = 0x0080,
    CP_APIC_REG_EOI                 = 0x00B0,
    CP_APIC_REG_SPURIOUS            = 0x00F0,
    CP_APIC_REG_ERROR_STATUS        = 0x0280,
    CP_APIC_REG_ICR_LOW             = 0x0300,
    CP_APIC_REG_ICR_HIGH            = 0x0310,
    CP_APIC_REG_LVT_TIMER           = 0x0320,
    CP_APIC_REG_LVT_LINT0           = 0x0350,
    CP_APIC_REG_LVT_LINT1           = 0x0360,
    CP_APIC_REG_LVT_ERROR           = 0x0370,
    CP_APIC_REG_TIMER_INITIAL_COUNT = 0x0380,
    CP_APIC_REG_TIMER_CURRENT_COUNT = 0x0390,
    CP_APIC_REG_TIMER_DIVIDE_CONFIG = 0x03E0
};

enum CpApicSpuriousFlag: UInt32
{
    CP_APIC_SPURIOUS_VECTOR = 0x000000FF,
    CP_APIC_SPURIOUS_ENABLE = 0x00000100
};

enum CpApicIcrFlag: UInt32
{
    CP_APIC_ICR_DELIVERY_FIXED        = 0x00000000,
    CP_APIC_ICR_DELIVERY_INIT         = 0x00000500,
    CP_APIC_ICR_DELIVERY_STARTUP      = 0x00000600,
    CP_APIC_ICR_LEVEL_DEASSERT        = 0x00000000,
    CP_APIC_ICR_LEVEL_ASSERT          = 0x00004000,
    CP_APIC_ICR_TRIGGER_EDGE          = 0x00000000,
    CP_APIC_ICR_TRIGGER_LEVEL         = 0x00008000,
    CP_APIC_ICR_DELIVERY_PENDING      = 0x00001000,
    CP_APIC_ICR_SHORTHAND_SELF        = 0x00040000,
    CP_APIC_ICR_SHORTHAND_ALL_INC_SELF = 0x00080000,
    CP_APIC_ICR_SHORTHAND_ALL_EX_SELF = 0x000C0000
};

enum CpApicTimerFlag: UInt32
{
    CP_APIC_TIMER_DIVIDE_BY_16  = 0x00000003,
    CP_APIC_TIMER_MASKED        = 0x00010000,
    CP_APIC_TIMER_MODE_ONE_SHOT = 0x00000000,
    CP_APIC_TIMER_MODE_PERIODIC = 0x00020000
};

struct CpApicLedger
{
    UInt64 physical_base;
    UInt64 virtual_base;
    UInt32 ticks_per_ms;
    UInt8  is_initialized;
    UInt8  is_x2apic;
}
__attribute__((aligned(16)));

#endif
