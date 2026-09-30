// Made by Berkay

#ifndef MM_VM_H
#define MM_VM_H

#include "../bk/bk_types.h"

#define MM_VM_PAGE_SIZE        4096ULL
#define MM_VM_HHDM_BASE        0xFFFF800000000000ULL
#define MM_VM_MAX_FREE_REGIONS 64U

enum MmVmFlags : UInt64
{
    MM_VM_FLAG_NONE          = 0ULL,
    MM_VM_FLAG_PRESENT       = (1ULL << 0),
    MM_VM_FLAG_WRITABLE      = (1ULL << 1),
    MM_VM_FLAG_USER          = (1ULL << 2),
    MM_VM_FLAG_WRITE_THROUGH = (1ULL << 3),
    MM_VM_FLAG_CACHE_DISABLE = (1ULL << 4),
    MM_VM_FLAG_ACCESSED      = (1ULL << 5),
    MM_VM_FLAG_DIRTY         = (1ULL << 6),
    MM_VM_FLAG_HUGE_PAGE     = (1ULL << 7),
    MM_VM_FLAG_GLOBAL        = (1ULL << 8),
    MM_VM_FLAG_NO_EXECUTE    = (1ULL << 63)
};

enum MmVmMasks : UInt64
{
    MM_VM_MASK_FRAME = 0x000FFFFFFFFFF000ULL,
    MM_VM_MASK_FLAGS = 0xFFF0000000000FFFULL
};

struct MmVmPageTable
{
    UInt64 entries[512];
}
__attribute__((aligned(4096)));

struct MmVmFreeRegion
{
    UInt64 virtual_base;
    UInt64 page_count;
};

struct MmVmLedger
{
    UInt64                pml4_physical_address;
    UInt64                virtual_base;
    UInt64                virtual_limit;
    UInt64                next_free_virtual;
    UInt64                total_mapped_pages;
    UInt64                total_allocated_tables;
    struct MmVmFreeRegion free_regions[MM_VM_MAX_FREE_REGIONS];
    UInt32                free_region_count;
    UInt8                 is_paging_active;
    volatile UInt32       spin_lock;
};

#endif
