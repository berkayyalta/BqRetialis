// Made by Berkay

#ifndef BK_FRAMEBUFFER_H
#define BK_FRAMEBUFFER_H

#include "bk_types.h"

struct BkFramebuffer
{
    UInt64 physical_base;
    UInt64 region_size;
    UInt32 width;
    UInt32 height;
    UInt32 pitch;
    UInt32 bpp;
}
__attribute__((packed));

#endif
