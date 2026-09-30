// Made by Berkay

#ifndef MM_FRAMEBUFFER_H
#define MM_FRAMEBUFFER_H

#include "../bk/bk_types.h"

struct MmFramebuffer
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
