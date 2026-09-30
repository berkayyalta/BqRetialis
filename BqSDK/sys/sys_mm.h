// Made by Berkay

#ifndef SYS_MM_H
#define SYS_MM_H

#include "sys_core.h"

BqStatus BqAllocateVirtualRegion(UInt64 virtual_address, UInt64 page_count, UInt64 flags);
BqStatus BqAllocateVirtualMemory(UInt64 virtual_address, UInt64 page_count, UInt64 flags);
BqStatus BqReleaseVirtualRegion(UInt64 virtual_address, UInt64 page_count);
BqStatus BqMapVirtualRegion(UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags);
BqStatus BqUnmapVirtualRegion(UInt64 virtual_address, UInt64 page_count);
BqStatus BqGetPhysicalAddress(UInt64 virtual_address, UInt64* physical_address);
BqStatus BqAllocateHeapBlock(UInt64 byte_count, void** heap_address);
BqStatus BqReallocateHeapBlock(void* old_heap_address, UInt64 byte_count, void** new_heap_address);
BqStatus BqReleaseHeapBlock(void* heap_address);
BqStatus BqCopyPhysicalMemory(void* destination_address, const void* source_address, UInt64 byte_count);
BqStatus BqFillPhysicalMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count);
BqStatus BqZeroPhysicalMemory(void* destination_address, UInt64 byte_count);
BqStatus BqComparePhysicalMemory(const void* first_address, const void* second_address, UInt64 byte_count, UInt8* is_equal);
BqStatus BqCopyVirtualMemory(void* destination_address, const void* source_address, UInt64 byte_count);
BqStatus BqFillVirtualMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count);
BqStatus BqZeroVirtualMemory(void* destination_address, UInt64 byte_count);
BqStatus BqCompareVirtualMemory(const void* first_address, const void* second_address, UInt64 byte_count, UInt8* is_equal);
BqStatus BqCopyHeapMemory(void* destination_address, const void* source_address, UInt64 byte_count);
BqStatus BqFillHeapMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count);
BqStatus BqZeroHeapMemory(void* destination_address, UInt64 byte_count);
BqStatus BqCompareHeapMemory(const void* first_address, const void* second_address, UInt64 byte_count, UInt8* is_equal);
BqStatus BqCreateAddressSpace(UInt64* page_table_address);
BqStatus BqDestroyAddressSpace(UInt64 page_table_address);
BqStatus BqMapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags);
BqStatus BqUnmapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 page_count);
BqStatus BqGetPhysicalAddressInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64* physical_address);

#endif
