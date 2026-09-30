// Made by Berkay

#ifndef MM_PUBLIC_H
#define MM_PUBLIC_H

#include "mm_status.h"
#include "mm_virtual_memory.h"

typedef enum MmStatus MmStatus;

typedef enum MmVirtualMemoryFlags MmVirtualMemoryFlags;

void MmLoad(void);

MmStatus MmInit(void);

UInt64 MmAllocatePhysicalRegion(UInt64 highest_acceptable_physical_address, UInt64 page_count);
void MmReleasePhysicalRegion(UInt64 physical_address, UInt64 page_count);

MmStatus MmAllocateVirtualRegion(UInt64* virtual_address, UInt64 page_count, UInt64 flags);
MmStatus MmReleaseVirtualRegion(UInt64 virtual_address, UInt64 page_count);

MmStatus MmMapVirtualRegion(UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags);
MmStatus MmUnmapVirtualRegion(UInt64 virtual_address, UInt64 page_count);
MmStatus MmGetPhysicalAddress(UInt64* physical_address, UInt64 virtual_address);

MmStatus MmAllocateHeapBlock(void** heap_address, UInt64 byte_count);
MmStatus MmReallocateHeapBlock(void** new_heap_address, void* old_heap_address, UInt64 byte_count);
MmStatus MmReleaseHeapBlock(void* heap_address);

MmStatus MmCopyPhysicalMemory(void* destination_address, void* source_address, UInt64 byte_count);
MmStatus MmFillPhysicalMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count);
MmStatus MmZeroPhysicalMemory(void* destination_address, UInt64 byte_count);
MmStatus MmComparePhysicalMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count);

MmStatus MmCopyVirtualMemory(void* destination_address, void* source_address, UInt64 byte_count);
MmStatus MmFillVirtualMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count);
MmStatus MmZeroVirtualMemory(void* destination_address, UInt64 byte_count);
MmStatus MmCompareVirtualMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count);

MmStatus MmCopyHeapMemory(void* destination_address, void* source_address, UInt64 byte_count);
MmStatus MmFillHeapMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count);
MmStatus MmZeroHeapMemory(void* destination_address, UInt64 byte_count);
MmStatus MmCompareHeapMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count);

MmStatus MmCreateAddressSpace(UInt64* page_table_address);
MmStatus MmDestroyAddressSpace(UInt64 page_table_address);
MmStatus MmMapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags);
MmStatus MmUnmapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 page_count);
MmStatus MmGetPhysicalAddressInSpace(UInt64* physical_address, UInt64 page_table_address, UInt64 virtual_address);

#endif
