// Made by Berkay

#include "bk_private.h"

void BkLoad(BkBootInfo* boot_info)
{
    BkRbpLoad((BkRbpBootInfo*)boot_info);

    CpLoad();
    MmLoad();
    HwLoad();
    PsLoad();
    FsLoad();
    ExLoad();

    BkInit();
}

BkStatus BkInit(void)
{
    CpStatus cp_status = CpInit();
    if (cp_status != CP_STATUS_SUCCESS)
    {
        BkPanicReport report =
        {
            .message  = "Phase 1 CPU initialization failed",
            .file     = __FILE__,
            .function = __func__,
            .line     = __LINE__
        };
        BkPanic(&report);
    }

    MmStatus mm_status = MmInit();
    if (mm_status != MM_STATUS_SUCCESS)
    {
        BkPanicReport report =
        {
            .message  = "Phase 1 Memory Management initialization failed",
            .file     = __FILE__,
            .function = __func__,
            .line     = __LINE__
        };
        BkPanic(&report);
    }

    HwStatus hw_status = HwInit();
    if (hw_status != HW_STATUS_SUCCESS)
    {
        BkPanicReport report =
        {
            .message  = "Phase 1 Hardware initialization failed",
            .file     = __FILE__,
            .function = __func__,
            .line     = __LINE__
        };
        BkPanic(&report);
    }

    PsStatus ps_status = PsInit();
    if (ps_status != PS_STATUS_SUCCESS)
    {
        BkPanicReport report =
        {
            .message  = "Phase 1 Process Subsystem initialization failed",
            .file     = __FILE__,
            .function = __func__,
            .line     = __LINE__
        };
        BkPanic(&report);
    }

    FsStatus fs_status = FsInit();
    if (fs_status != FS_STATUS_SUCCESS)
    {
        BkPanicReport report =
        {
            .message  = "Phase 1 File System initialization failed",
            .file     = __FILE__,
            .function = __func__,
            .line     = __LINE__
        };
        BkPanic(&report);
    }

    ExStatus ex_status = ExInit();
    if (ex_status != EX_STATUS_SUCCESS)
    {
        BkPanicReport report =
        {
            .message  = "Phase 1 Executive Subsystem initialization failed",
            .file     = __FILE__,
            .function = __func__,
            .line     = __LINE__
        };
        BkPanic(&report);
    }

    CpStartApicTimer(0x40, 100);
    CpEnableInterrupts();
    CpTriggerYield();

    for (;;)
    {
        __asm__ volatile("hlt");
    }

    return BK_STATUS_SUCCESS;
}

void BkPanic(BkPanicReport* report)
{
    (void)report;

    __asm__ volatile("cli");

    for (;;)
    {
        __asm__ volatile("hlt");
    }
}

BkKernelLayout BkGetKernelLayout(void)
{
    BkRbpKernelLayout rbp_layout = BkRbpGetKernelLayout();

    BkKernelLayout layout =
    {
        .physical_base = rbp_layout.physical_base,
        .virtual_base  = rbp_layout.virtual_base,
        .region_size   = rbp_layout.region_size
    };

    return layout;
}

BkMemoryMap BkGetMemoryMap(void)
{
    BkRbpMemoryMap rbp_map = BkRbpGetMemoryMap();

    BkMemoryMap map =
    {
        .entries = (BkMemoryMapEntry*)rbp_map.entries,
        .count   = rbp_map.count
    };

    return map;
}

BkAcpiRoot BkGetAcpiRoot(void)
{
    BkRbpAcpiRoot rbp_acpi = BkRbpGetAcpiRoot();

    BkAcpiRoot acpi =
    {
        .madt = (BkAcpiMadt*)rbp_acpi.madt,
        .fadt = (BkAcpiFadt*)rbp_acpi.fadt,
        .hpet = (BkAcpiHpet*)rbp_acpi.hpet,
        .mcfg = (BkAcpiMcfg*)rbp_acpi.mcfg
    };

    return acpi;
}

BkAcpiMadt* BkGetAcpiMadt(void)
{
    return (BkAcpiMadt*)BkRbpGetAcpiMadt();
}

BkAcpiFadt* BkGetAcpiFadt(void)
{
    return (BkAcpiFadt*)BkRbpGetAcpiFadt();
}

BkAcpiHpet* BkGetAcpiHpet(void)
{
    return (BkAcpiHpet*)BkRbpGetAcpiHpet();
}

BkAcpiMcfg* BkGetAcpiMcfg(void)
{
    return (BkAcpiMcfg*)BkRbpGetAcpiMcfg();
}

BkFramebuffer BkGetFramebuffer(void)
{
    BkRbpFramebuffer rbp_framebuffer = BkRbpGetFramebuffer();

    BkFramebuffer framebuffer =
    {
        .physical_base = rbp_framebuffer.physical_base,
        .region_size   = rbp_framebuffer.region_size,
        .width         = rbp_framebuffer.width,
        .height        = rbp_framebuffer.height,
        .pitch         = rbp_framebuffer.pitch,
        .bpp           = rbp_framebuffer.bpp
    };

    return framebuffer;
}

BkBootInfo BkGetBootInfo(void)
{
    BkRbpBootInfo rbp_boot = BkRbpGetBootInfo();

    BkBootInfo boot_info =
    {
        .kernel_layout = BkGetKernelLayout(),
        .memory_map    = BkGetMemoryMap(),
        .acpi_root     = BkGetAcpiRoot(),
        .framebuffer   = BkGetFramebuffer(),
        .modules       = (BkBootModule*)rbp_boot.modules,
        .module_count  = rbp_boot.module_count
    };

    return boot_info;
}
