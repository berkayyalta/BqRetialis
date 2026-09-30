// Made by Berkay

#include "mm_private.h"

void MmLoad(void)
{
    MmPmLedgerLoad();
}

MmStatus MmInit(void)
{
    MmStatus status = MmVmLedgerLoad();

    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    return MmHpLedgerLoad();
}

UInt64 MmAllocatePhysicalRegion(UInt64 highest_acceptable_physical_address, UInt64 page_count)
{
    return MmPmAllocatePhysicalRegion(highest_acceptable_physical_address, page_count);
}

void MmReleasePhysicalRegion(UInt64 physical_address, UInt64 page_count)
{
    MmPmReleasePhysicalRegion(physical_address, page_count);
}

MmStatus MmAllocateVirtualRegion(UInt64* virtual_address, UInt64 page_count, UInt64 flags)
{
    return MmVmAllocateVirtualRegion(virtual_address, page_count, flags);
}

MmStatus MmReleaseVirtualRegion(UInt64 virtual_address, UInt64 page_count)
{
    return MmVmReleaseVirtualRegion(virtual_address, page_count);
}

MmStatus MmMapVirtualRegion(UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags)
{
    return MmVmMapVirtualRegion(virtual_address, physical_address, page_count, flags);
}

MmStatus MmUnmapVirtualRegion(UInt64 virtual_address, UInt64 page_count)
{
    return MmVmUnmapVirtualRegion(virtual_address, page_count);
}

MmStatus MmGetPhysicalAddress(UInt64* physical_address, UInt64 virtual_address)
{
    return MmVmGetPhysicalAddress(physical_address, virtual_address);
}

MmStatus MmAllocateHeapBlock(void** heap_address, UInt64 byte_count)
{
    return MmHpAllocateBlock(heap_address, byte_count);
}

MmStatus MmReallocateHeapBlock(void** new_heap_address, void* old_heap_address, UInt64 byte_count)
{
    return MmHpReallocateBlock(new_heap_address, old_heap_address, byte_count);
}

MmStatus MmReleaseHeapBlock(void* heap_address)
{
    return MmHpReleaseBlock(heap_address);
}

MmStatus MmCopyPhysicalMemory(void* destination_address, void* source_address, UInt64 byte_count)
{
    return MmKitCopyPhysicalMemory(destination_address, source_address, byte_count);
}

MmStatus MmFillPhysicalMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count)
{
    return MmKitFillPhysicalMemory(destination_address, byte_value, byte_count);
}

MmStatus MmZeroPhysicalMemory(void* destination_address, UInt64 byte_count)
{
    return MmKitZeroPhysicalMemory(destination_address, byte_count);
}

MmStatus MmComparePhysicalMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count)
{
    return MmKitComparePhysicalMemory(is_equal, first_address, second_address, byte_count);
}

MmStatus MmCopyVirtualMemory(void* destination_address, void* source_address, UInt64 byte_count)
{
    return MmKitCopyVirtualMemory(destination_address, source_address, byte_count);
}

MmStatus MmFillVirtualMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count)
{
    return MmKitFillVirtualMemory(destination_address, byte_value, byte_count);
}

MmStatus MmZeroVirtualMemory(void* destination_address, UInt64 byte_count)
{
    return MmKitZeroVirtualMemory(destination_address, byte_count);
}

MmStatus MmCompareVirtualMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count)
{
    return MmKitCompareVirtualMemory(is_equal, first_address, second_address, byte_count);
}

MmStatus MmCopyHeapMemory(void* destination_address, void* source_address, UInt64 byte_count)
{
    return MmKitCopyHeapMemory(destination_address, source_address, byte_count);
}

MmStatus MmFillHeapMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count)
{
    return MmKitFillHeapMemory(destination_address, byte_value, byte_count);
}

MmStatus MmZeroHeapMemory(void* destination_address, UInt64 byte_count)
{
    return MmKitZeroHeapMemory(destination_address, byte_count);
}

MmStatus MmCompareHeapMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count)
{
    return MmKitCompareHeapMemory(is_equal, first_address, second_address, byte_count);
}

MmStatus MmCreateAddressSpace(UInt64* page_table_address)
{
    return MmVmCreateAddressSpace(page_table_address);
}

MmStatus MmDestroyAddressSpace(UInt64 page_table_address)
{
    return MmVmDestroyAddressSpace(page_table_address);
}

MmStatus MmMapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags)
{
    return MmVmMapVirtualRegionInSpace(page_table_address, virtual_address, physical_address, page_count, flags);
}

MmStatus MmUnmapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 page_count)
{
    return MmVmUnmapVirtualRegionInSpace(page_table_address, virtual_address, page_count);
}

MmStatus MmGetPhysicalAddressInSpace(UInt64* physical_address, UInt64 page_table_address, UInt64 virtual_address)
{
    return MmVmGetPhysicalAddressInSpace(physical_address, page_table_address, virtual_address);
}
