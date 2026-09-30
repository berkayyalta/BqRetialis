// Made by Berkay

#ifndef ABI_SC_MM_H
#define ABI_SC_MM_H

#include "abi_sc_status.h"
#include "../mm/mm_public.h"

struct AbiScAllocateVirtualRegionForm
{
    UInt64 virtual_address;
    UInt64 page_count;
    UInt64 flags;
};
typedef struct AbiScAllocateVirtualRegionForm AbiScAllocateVirtualRegionForm;
AbiScStatus AbiScAllocateVirtualRegion(AbiScAllocateVirtualRegionForm* form);

struct AbiScReleaseVirtualRegionForm
{
    UInt64 virtual_address;
    UInt64 page_count;
};
typedef struct AbiScReleaseVirtualRegionForm AbiScReleaseVirtualRegionForm;
AbiScStatus AbiScReleaseVirtualRegion(AbiScReleaseVirtualRegionForm* form);

struct AbiScMapVirtualRegionForm
{
    UInt64 virtual_address;
    UInt64 physical_address;
    UInt64 page_count;
    UInt64 flags;
};
typedef struct AbiScMapVirtualRegionForm AbiScMapVirtualRegionForm;
AbiScStatus AbiScMapVirtualRegion(AbiScMapVirtualRegionForm* form);

struct AbiScUnmapVirtualRegionForm
{
    UInt64 virtual_address;
    UInt64 page_count;
};
typedef struct AbiScUnmapVirtualRegionForm AbiScUnmapVirtualRegionForm;
AbiScStatus AbiScUnmapVirtualRegion(AbiScUnmapVirtualRegionForm* form);

struct AbiScGetPhysicalAddressForm
{
    UInt64 physical_address;
    UInt64 virtual_address;
};
typedef struct AbiScGetPhysicalAddressForm AbiScGetPhysicalAddressForm;
AbiScStatus AbiScGetPhysicalAddress(AbiScGetPhysicalAddressForm* form);

struct AbiScAllocateHeapBlockForm
{
    void*  heap_address;
    UInt64 byte_count;
};
typedef struct AbiScAllocateHeapBlockForm AbiScAllocateHeapBlockForm;
AbiScStatus AbiScAllocateHeapBlock(AbiScAllocateHeapBlockForm* form);

struct AbiScReallocateHeapBlockForm
{
    void*  new_heap_address;
    void*  old_heap_address;
    UInt64 byte_count;
};
typedef struct AbiScReallocateHeapBlockForm AbiScReallocateHeapBlockForm;
AbiScStatus AbiScReallocateHeapBlock(AbiScReallocateHeapBlockForm* form);

struct AbiScReleaseHeapBlockForm
{
    void* heap_address;
};
typedef struct AbiScReleaseHeapBlockForm AbiScReleaseHeapBlockForm;
AbiScStatus AbiScReleaseHeapBlock(AbiScReleaseHeapBlockForm* form);

struct AbiScCopyPhysicalMemoryForm
{
    void*  destination_address;
    void*  source_address;
    UInt64 byte_count;
};
typedef struct AbiScCopyPhysicalMemoryForm AbiScCopyPhysicalMemoryForm;
AbiScStatus AbiScCopyPhysicalMemory(AbiScCopyPhysicalMemoryForm* form);

struct AbiScFillPhysicalMemoryForm
{
    void*  destination_address;
    UInt8  byte_value;
    UInt64 byte_count;
};
typedef struct AbiScFillPhysicalMemoryForm AbiScFillPhysicalMemoryForm;
AbiScStatus AbiScFillPhysicalMemory(AbiScFillPhysicalMemoryForm* form);

struct AbiScZeroPhysicalMemoryForm
{
    void*  destination_address;
    UInt64 byte_count;
};
typedef struct AbiScZeroPhysicalMemoryForm AbiScZeroPhysicalMemoryForm;
AbiScStatus AbiScZeroPhysicalMemory(AbiScZeroPhysicalMemoryForm* form);

struct AbiScComparePhysicalMemoryForm
{
    UInt8  is_equal;
    void*  first_address;
    void*  second_address;
    UInt64 byte_count;
};
typedef struct AbiScComparePhysicalMemoryForm AbiScComparePhysicalMemoryForm;
AbiScStatus AbiScComparePhysicalMemory(AbiScComparePhysicalMemoryForm* form);

struct AbiScCopyVirtualMemoryForm
{
    void*  destination_address;
    void*  source_address;
    UInt64 byte_count;
};
typedef struct AbiScCopyVirtualMemoryForm AbiScCopyVirtualMemoryForm;
AbiScStatus AbiScCopyVirtualMemory(AbiScCopyVirtualMemoryForm* form);

struct AbiScFillVirtualMemoryForm
{
    void*  destination_address;
    UInt8  byte_value;
    UInt64 byte_count;
};
typedef struct AbiScFillVirtualMemoryForm AbiScFillVirtualMemoryForm;
AbiScStatus AbiScFillVirtualMemory(AbiScFillVirtualMemoryForm* form);

struct AbiScZeroVirtualMemoryForm
{
    void*  destination_address;
    UInt64 byte_count;
};
typedef struct AbiScZeroVirtualMemoryForm AbiScZeroVirtualMemoryForm;
AbiScStatus AbiScZeroVirtualMemory(AbiScZeroVirtualMemoryForm* form);

struct AbiScCompareVirtualMemoryForm
{
    UInt8  is_equal;
    void*  first_address;
    void*  second_address;
    UInt64 byte_count;
};
typedef struct AbiScCompareVirtualMemoryForm AbiScCompareVirtualMemoryForm;
AbiScStatus AbiScCompareVirtualMemory(AbiScCompareVirtualMemoryForm* form);

struct AbiScCopyHeapMemoryForm
{
    void*  destination_address;
    void*  source_address;
    UInt64 byte_count;
};
typedef struct AbiScCopyHeapMemoryForm AbiScCopyHeapMemoryForm;
AbiScStatus AbiScCopyHeapMemory(AbiScCopyHeapMemoryForm* form);

struct AbiScFillHeapMemoryForm
{
    void*  destination_address;
    UInt8  byte_value;
    UInt64 byte_count;
};
typedef struct AbiScFillHeapMemoryForm AbiScFillHeapMemoryForm;
AbiScStatus AbiScFillHeapMemory(AbiScFillHeapMemoryForm* form);

struct AbiScZeroHeapMemoryForm
{
    void*  destination_address;
    UInt64 byte_count;
};
typedef struct AbiScZeroHeapMemoryForm AbiScZeroHeapMemoryForm;
AbiScStatus AbiScZeroHeapMemory(AbiScZeroHeapMemoryForm* form);

struct AbiScCompareHeapMemoryForm
{
    UInt8  is_equal;
    void*  first_address;
    void*  second_address;
    UInt64 byte_count;
};
typedef struct AbiScCompareHeapMemoryForm AbiScCompareHeapMemoryForm;
AbiScStatus AbiScCompareHeapMemory(AbiScCompareHeapMemoryForm* form);

struct AbiScCreateAddressSpaceForm
{
    UInt64 page_table_address;
};
typedef struct AbiScCreateAddressSpaceForm AbiScCreateAddressSpaceForm;
AbiScStatus AbiScCreateAddressSpace(AbiScCreateAddressSpaceForm* form);

struct AbiScDestroyAddressSpaceForm
{
    UInt64 page_table_address;
};
typedef struct AbiScDestroyAddressSpaceForm AbiScDestroyAddressSpaceForm;
AbiScStatus AbiScDestroyAddressSpace(AbiScDestroyAddressSpaceForm* form);

struct AbiScMapVirtualRegionInSpaceForm
{
    UInt64 page_table_address;
    UInt64 virtual_address;
    UInt64 physical_address;
    UInt64 page_count;
    UInt64 flags;
};
typedef struct AbiScMapVirtualRegionInSpaceForm AbiScMapVirtualRegionInSpaceForm;
AbiScStatus AbiScMapVirtualRegionInSpace(AbiScMapVirtualRegionInSpaceForm* form);

struct AbiScUnmapVirtualRegionInSpaceForm
{
    UInt64 page_table_address;
    UInt64 virtual_address;
    UInt64 page_count;
};
typedef struct AbiScUnmapVirtualRegionInSpaceForm AbiScUnmapVirtualRegionInSpaceForm;
AbiScStatus AbiScUnmapVirtualRegionInSpace(AbiScUnmapVirtualRegionInSpaceForm* form);

struct AbiScGetPhysicalAddressInSpaceForm
{
    UInt64 physical_address;
    UInt64 page_table_address;
    UInt64 virtual_address;
};
typedef struct AbiScGetPhysicalAddressInSpaceForm AbiScGetPhysicalAddressInSpaceForm;
AbiScStatus AbiScGetPhysicalAddressInSpace(AbiScGetPhysicalAddressInSpaceForm* form);

#endif
