// Made by Berkay

#include "cp_private.h"

static CpFpuLedger s_fpu_ledger;

CpStatus CpFpuSave(void* fpu_buffer)
{
    if (fpu_buffer == NULL || (((UInt64)fpu_buffer) & 0x0F) != 0)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    __asm__ volatile
    (
        "fxsave64 (%0)"
        :
        : "r" (fpu_buffer)
        : "memory"
    );

    return CP_STATUS_SUCCESS;
}

CpStatus CpFpuRestore(void* fpu_buffer)
{
    if (fpu_buffer == NULL || (((UInt64)fpu_buffer) & 0x0F) != 0)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    __asm__ volatile
    (
        "fxrstor64 (%0)"
        :
        : "r" (fpu_buffer)
        : "memory"
    );

    return CP_STATUS_SUCCESS;
}

CpStatus CpFpuGetStateSize(UInt64* size, UInt64* alignment)
{
    if (size != 0) *size = 512;
    if (alignment != 0) *alignment = 16;
    return CP_STATUS_SUCCESS;
}

CpStatus CpFpuEnable(UInt32 core_index)
{
    if (core_index >= CP_FPU_MAX_CORES)
    {
        return CP_STATUS_OUT_OF_BOUNDS;
    }

    UInt64 cr0 = 0;
    UInt64 cr4 = 0;

    __asm__ volatile ("mov %%cr0, %0" : "=r" (cr0));
    cr0 &= ~((UInt64)(CP_FPU_CR0_EM | CP_FPU_CR0_TS));
    cr0 |= (UInt64)CP_FPU_CR0_MP;
    __asm__ volatile ("mov %0, %%cr0" : : "r" (cr0));

    __asm__ volatile ("mov %%cr4, %0" : "=r" (cr4));
    cr4 |= (UInt64)(CP_FPU_CR4_OSFXSR | CP_FPU_CR4_OSXMMEXCPT);
    __asm__ volatile ("mov %0, %%cr4" : : "r" (cr4));

    __asm__ volatile ("fninit");

    return CpFpuSave(&s_fpu_ledger.active_states[core_index]);
}

CpStatus CpFpuInit(void)
{
    UInt8* raw_ledger = (UInt8*)&s_fpu_ledger;

    for (UInt64 i = 0; i < sizeof(CpFpuLedger); i++)
    {
        raw_ledger[i] = 0;
    }

    UInt32 bsp_core_index = 0;

    CpStatus status = CpSmpGetCurrentCoreIndex(&bsp_core_index);

    if (status != CP_STATUS_SUCCESS)
    {
        bsp_core_index = 0;
    }

    return CpFpuEnable(bsp_core_index);
}
