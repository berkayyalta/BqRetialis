// Made by Berkay

#ifndef BK_KERNEL_LAYOUT_H
#define BK_KERNEL_LAYOUT_H

#include "bk_types.h"

struct BkKernelLayout
{
    UInt64 physical_base;
    UInt64 virtual_base;
    UInt64 region_size;
}
__attribute__((packed));

#endif
