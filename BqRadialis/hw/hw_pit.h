// Made by Berkay

#ifndef HW_PIT_H
#define HW_PIT_H

#include "../bk/bk_types.h"

#define HW_PIT_CHANNEL0_DATA        0x40
#define HW_PIT_COMMAND_PORT         0x43

#define HW_PIT_COMMAND_SQUARE_WAVE  0x36

#define HW_PIT_BASE_FREQUENCY       1193182U
#define HW_PIT_DEFAULT_FREQUENCY    1000U
#define HW_PIT_MIN_FREQUENCY        19U

struct HwPitLedger
{
    UInt32 frequency;
    UInt16 divisor;
    UInt64 ticks;
};

#endif
