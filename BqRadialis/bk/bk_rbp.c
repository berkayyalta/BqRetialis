// Made by Berkay

#include "bk_private.h"

static BkRbpLedger s_rbp_ledger;

void BkRbpLoad(BkRbpBootInfo* boot_info)
{
    if (boot_info == NULL)
    {
        BkPanicReport report =
        {
            .message  = "Boot Info pointer is NULL",
            .file     = __FILE__,
            .function = __func__,
            .line     = __LINE__
        };
        BkPanic(&report);
    }

    s_rbp_ledger.boot_info = *boot_info;

    BkRbpFramebuffer fb = s_rbp_ledger.boot_info.framebuffer;
    if (fb.physical_base != 0 && fb.region_size != 0)
    {
        UInt32 count = s_rbp_ledger.boot_info.memory_map.count;
        s_rbp_ledger.boot_info.memory_map.entries[count].base_address = fb.physical_base;
        s_rbp_ledger.boot_info.memory_map.entries[count].region_size = fb.region_size;
        s_rbp_ledger.boot_info.memory_map.entries[count].hardware_type = BK_RBP_MEMORY_MAP_TYPE_RESERVED;
        s_rbp_ledger.boot_info.memory_map.entries[count].acpi_attributes = BK_RBP_MEMORY_MAP_ACPI_NONE;
        s_rbp_ledger.boot_info.memory_map.count++;
        boot_info->memory_map.count++;
    }
}

BkRbpKernelLayout BkRbpGetKernelLayout(void)
{
    return s_rbp_ledger.boot_info.kernel_layout;
}

BkRbpMemoryMap BkRbpGetMemoryMap(void)
{
    return s_rbp_ledger.boot_info.memory_map;
}

BkRbpAcpiRoot BkRbpGetAcpiRoot(void)
{
    return s_rbp_ledger.boot_info.acpi_root;
}

BkRbpAcpiMadt* BkRbpGetAcpiMadt(void)
{
    return s_rbp_ledger.boot_info.acpi_root.madt;
}

BkRbpAcpiFadt* BkRbpGetAcpiFadt(void)
{
    return s_rbp_ledger.boot_info.acpi_root.fadt;
}

BkRbpAcpiHpet* BkRbpGetAcpiHpet(void)
{
    return s_rbp_ledger.boot_info.acpi_root.hpet;
}

BkRbpAcpiMcfg* BkRbpGetAcpiMcfg(void)
{
    return s_rbp_ledger.boot_info.acpi_root.mcfg;
}

BkRbpFramebuffer BkRbpGetFramebuffer(void)
{
    return s_rbp_ledger.boot_info.framebuffer;
}

BkRbpBootInfo BkRbpGetBootInfo(void)
{
    return s_rbp_ledger.boot_info;
}
