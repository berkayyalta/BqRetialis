// Made by Berkay

#ifndef CP_FPU_H
#define CP_FPU_H

#include "../bk/bk_types.h"

#define CP_FPU_MAX_CORES  64
#define CP_FPU_STATE_SIZE 512

enum CpFpuCr0Flag: UInt64
{
    CP_FPU_CR0_MP = (1ULL << 1),
    CP_FPU_CR0_EM = (1ULL << 2),
    CP_FPU_CR0_TS = (1ULL << 3)
};

enum CpFpuCr4Flag: UInt64
{
    CP_FPU_CR4_OSFXSR     = (1ULL << 9),
    CP_FPU_CR4_OSXMMEXCPT = (1ULL << 10)
};

struct CpFpuState
{
    UInt8 bytes[CP_FPU_STATE_SIZE] __attribute__((aligned(16)));
};

struct CpFpuLedger
{
    struct CpFpuState active_states[CP_FPU_MAX_CORES] __attribute__((aligned(16)));
}
__attribute__((aligned(16)));

#endif
