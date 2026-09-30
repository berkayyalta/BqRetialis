// Made by Berkay

#ifndef MM_KERNEL_LAYOUT_H
#define MM_KERNEL_LAYOUT_H

#include "../bk/bk_types.h"

struct MmKernelLayout
{
    UInt64 physical_base;
    UInt64 virtual_base;
    UInt64 region_size;
}
__attribute__((packed));

#endif
