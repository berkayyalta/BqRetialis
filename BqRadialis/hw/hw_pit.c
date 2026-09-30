// Made by Berkay

#include "hw_private.h"

static HwPitLedger s_pit_ledger;

void HwPitLoad(void)
{
    s_pit_ledger.frequency = 0;
    s_pit_ledger.divisor   = 0;
    s_pit_ledger.ticks     = 0;

    HwPitSetFrequency(HW_PIT_DEFAULT_FREQUENCY);
}

void HwPitSetFrequency(UInt32 frequency)
{
    if (frequency < HW_PIT_MIN_FREQUENCY || frequency > HW_PIT_BASE_FREQUENCY)
    {
        return;
    }

    UInt32 calculated_divisor = HW_PIT_BASE_FREQUENCY / frequency;

    if (calculated_divisor == 0)
    {
        calculated_divisor = 1;
    }

    if (calculated_divisor > 0xFFFFU)
    {
        calculated_divisor = 0xFFFFU;
    }

    UInt16 divisor = (UInt16)calculated_divisor;

    HwIoOut8(HW_PIT_COMMAND_PORT, HW_PIT_COMMAND_SQUARE_WAVE);
    HwIoWait();

    HwIoOut8(HW_PIT_CHANNEL0_DATA, (UInt8)(divisor & 0xFFU));
    HwIoWait();

    HwIoOut8(HW_PIT_CHANNEL0_DATA, (UInt8)((divisor >> 8) & 0xFFU));
    HwIoWait();

    s_pit_ledger.frequency = frequency;
    s_pit_ledger.divisor   = divisor;
}

UInt32 HwPitGetFrequency(void)
{
    return s_pit_ledger.frequency;
}

void HwPitIncrementTicks(void)
{
    s_pit_ledger.ticks++;
}

UInt64 HwPitGetTicks(void)
{
    return s_pit_ledger.ticks;
}
