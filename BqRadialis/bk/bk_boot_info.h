// Made by Berkay

#ifndef BK_BOOT_INFO_H
#define BK_BOOT_INFO_H

#include "bk_types.h"

#include "bk_kernel_layout.h"
#include "bk_memory_map.h"
#include "bk_acpi.h"
#include "bk_framebuffer.h"

enum BkBootLimit : UInt32
{
    BK_BOOT_MAX_MODULES = 16
};

struct BkBootModule
{
    UInt64 physical_base;
    UInt64 size;
    char   path[128];
};

struct BkBootInfo
{
    struct BkKernelLayout   kernel_layout;
    struct BkMemoryMap      memory_map;
    struct BkAcpiRoot       acpi_root;
    struct BkFramebuffer    framebuffer;
    struct BkBootModule*    modules;
    UInt32                  module_count;
};

#endif
