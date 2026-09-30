// Made by Berkay

#include "sys_bk.h"

BqStatus BqGetKernelLayout(BkKernelLayout* layout)
{
    if (layout == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetKernelLayoutForm form;
    BqStatus status = BqSyscall(ABI_SC_BK_GET_KERNEL_LAYOUT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *layout = form.layout;
    }

    return status;
}

BqStatus BqGetMemoryMap(BkMemoryMap* memory_map)
{
    if (memory_map == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetMemoryMapForm form;
    BqStatus status = BqSyscall(ABI_SC_BK_GET_MEMORY_MAP, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *memory_map = form.memory_map;
    }

    return status;
}

BqStatus BqGetAcpiRoot(BkAcpiRoot* acpi_root)
{
    if (acpi_root == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiRootForm form;
    BqStatus status = BqSyscall(ABI_SC_BK_GET_ACPI_ROOT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *acpi_root = form.acpi_root;
    }

    return status;
}

BqStatus BqGetAcpiMadt(BkAcpiMadt** madt)
{
    if (madt == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiMadtForm form;
    form.madt = NULL;
    BqStatus status = BqSyscall(ABI_SC_BK_GET_ACPI_MADT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *madt = form.madt;
    }

    return status;
}

BqStatus BqGetAcpiFadt(BkAcpiFadt** fadt)
{
    if (fadt == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiFadtForm form;
    form.fadt = NULL;
    BqStatus status = BqSyscall(ABI_SC_BK_GET_ACPI_FADT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *fadt = form.fadt;
    }

    return status;
}

BqStatus BqGetAcpiHpet(BkAcpiHpet** hpet)
{
    if (hpet == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiHpetForm form;
    form.hpet = NULL;
    BqStatus status = BqSyscall(ABI_SC_BK_GET_ACPI_HPET, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *hpet = form.hpet;
    }

    return status;
}

BqStatus BqGetAcpiMcfg(BkAcpiMcfg** mcfg)
{
    if (mcfg == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiMcfgForm form;
    form.mcfg = NULL;
    BqStatus status = BqSyscall(ABI_SC_BK_GET_ACPI_MCFG, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *mcfg = form.mcfg;
    }

    return status;
}

BqStatus BqGetFramebuffer(BkFramebuffer* framebuffer)
{
    if (framebuffer == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetFramebufferForm form;
    BqStatus status = BqSyscall(ABI_SC_BK_GET_FRAMEBUFFER, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *framebuffer = form.framebuffer;
    }

    return status;
}

BqStatus BqGetBootInfo(BkBootInfo* boot_info)
{
    if (boot_info == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetBootInfoForm form;
    BqStatus status = BqSyscall(ABI_SC_BK_GET_BOOT_INFO, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *boot_info = form.boot_info;
    }

    return status;
}
