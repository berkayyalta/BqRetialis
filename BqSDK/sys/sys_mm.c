// Made by Berkay

#include "sys_mm.h"

BqStatus BqAllocateVirtualRegion(UInt64 virtual_address, UInt64 page_count, UInt64 flags)
{
    AbiScAllocateVirtualRegionForm form;
    form.virtual_address = virtual_address;
    form.page_count      = page_count;
    form.flags           = flags;
    return BqSyscall(ABI_SC_MM_ALLOCATE_VIRTUAL_REGION, &form);
}

BqStatus BqAllocateVirtualMemory(UInt64 virtual_address, UInt64 page_count, UInt64 flags)
{
    return BqAllocateVirtualRegion(virtual_address, page_count, flags);
}

BqStatus BqReleaseVirtualRegion(UInt64 virtual_address, UInt64 page_count)
{
    AbiScReleaseVirtualRegionForm form;
    form.virtual_address = virtual_address;
    form.page_count      = page_count;
    return BqSyscall(ABI_SC_MM_RELEASE_VIRTUAL_REGION, &form);
}

BqStatus BqMapVirtualRegion(UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags)
{
    AbiScMapVirtualRegionForm form;
    form.virtual_address  = virtual_address;
    form.physical_address = physical_address;
    form.page_count       = page_count;
    form.flags            = flags;
    return BqSyscall(ABI_SC_MM_MAP_VIRTUAL_REGION, &form);
}

BqStatus BqUnmapVirtualRegion(UInt64 virtual_address, UInt64 page_count)
{
    AbiScUnmapVirtualRegionForm form;
    form.virtual_address = virtual_address;
    form.page_count      = page_count;
    return BqSyscall(ABI_SC_MM_UNMAP_VIRTUAL_REGION, &form);
}

BqStatus BqGetPhysicalAddress(UInt64 virtual_address, UInt64* physical_address)
{
    if (physical_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetPhysicalAddressForm form;
    form.virtual_address  = virtual_address;
    form.physical_address = 0;
    BqStatus status = BqSyscall(ABI_SC_MM_GET_PHYSICAL_ADDRESS, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *physical_address = form.physical_address;
    }

    return status;
}

BqStatus BqAllocateHeapBlock(UInt64 byte_count, void** heap_address)
{
    if (heap_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScAllocateHeapBlockForm form;
    form.byte_count   = byte_count;
    form.heap_address = NULL;
    BqStatus status = BqSyscall(ABI_SC_MM_ALLOCATE_HEAP_BLOCK, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *heap_address = form.heap_address;
    }

    return status;
}

BqStatus BqReallocateHeapBlock(void* old_heap_address, UInt64 byte_count, void** new_heap_address)
{
    if (new_heap_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScReallocateHeapBlockForm form;
    form.old_heap_address = old_heap_address;
    form.byte_count       = byte_count;
    form.new_heap_address = NULL;
    BqStatus status = BqSyscall(ABI_SC_MM_REALLOCATE_HEAP_BLOCK, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *new_heap_address = form.new_heap_address;
    }

    return status;
}

BqStatus BqReleaseHeapBlock(void* heap_address)
{
    if (heap_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScReleaseHeapBlockForm form;
    form.heap_address = heap_address;
    return BqSyscall(ABI_SC_MM_RELEASE_HEAP_BLOCK, &form);
}

BqStatus BqCopyPhysicalMemory(void* destination_address, const void* source_address, UInt64 byte_count)
{
    if (destination_address == NULL || source_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyPhysicalMemoryForm form;
    form.destination_address = destination_address;
    form.source_address      = (void*)source_address;
    form.byte_count          = byte_count;
    return BqSyscall(ABI_SC_MM_COPY_PHYSICAL_MEMORY, &form);
}

BqStatus BqFillPhysicalMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count)
{
    if (destination_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFillPhysicalMemoryForm form;
    form.destination_address = destination_address;
    form.byte_value          = byte_value;
    form.byte_count          = byte_count;
    return BqSyscall(ABI_SC_MM_FILL_PHYSICAL_MEMORY, &form);
}

BqStatus BqZeroPhysicalMemory(void* destination_address, UInt64 byte_count)
{
    if (destination_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroPhysicalMemoryForm form;
    form.destination_address = destination_address;
    form.byte_count          = byte_count;
    return BqSyscall(ABI_SC_MM_ZERO_PHYSICAL_MEMORY, &form);
}

BqStatus BqComparePhysicalMemory(const void* first_address, const void* second_address, UInt64 byte_count, UInt8* is_equal)
{
    if (first_address == NULL || second_address == NULL || is_equal == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScComparePhysicalMemoryForm form;
    form.first_address  = (void*)first_address;
    form.second_address = (void*)second_address;
    form.byte_count     = byte_count;
    form.is_equal       = 0;
    BqStatus status = BqSyscall(ABI_SC_MM_COMPARE_PHYSICAL_MEMORY, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *is_equal = form.is_equal;
    }

    return status;
}

BqStatus BqCopyVirtualMemory(void* destination_address, const void* source_address, UInt64 byte_count)
{
    if (destination_address == NULL || source_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyVirtualMemoryForm form;
    form.destination_address = destination_address;
    form.source_address      = (void*)source_address;
    form.byte_count          = byte_count;
    return BqSyscall(ABI_SC_MM_COPY_VIRTUAL_MEMORY, &form);
}

BqStatus BqFillVirtualMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count)
{
    if (destination_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFillVirtualMemoryForm form;
    form.destination_address = destination_address;
    form.byte_value          = byte_value;
    form.byte_count          = byte_count;
    return BqSyscall(ABI_SC_MM_FILL_VIRTUAL_MEMORY, &form);
}

BqStatus BqZeroVirtualMemory(void* destination_address, UInt64 byte_count)
{
    if (destination_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroVirtualMemoryForm form;
    form.destination_address = destination_address;
    form.byte_count          = byte_count;
    return BqSyscall(ABI_SC_MM_ZERO_VIRTUAL_MEMORY, &form);
}

BqStatus BqCompareVirtualMemory(const void* first_address, const void* second_address, UInt64 byte_count, UInt8* is_equal)
{
    if (first_address == NULL || second_address == NULL || is_equal == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCompareVirtualMemoryForm form;
    form.first_address  = (void*)first_address;
    form.second_address = (void*)second_address;
    form.byte_count     = byte_count;
    form.is_equal       = 0;
    BqStatus status = BqSyscall(ABI_SC_MM_COMPARE_VIRTUAL_MEMORY, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *is_equal = form.is_equal;
    }

    return status;
}

BqStatus BqCopyHeapMemory(void* destination_address, const void* source_address, UInt64 byte_count)
{
    if (destination_address == NULL || source_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyHeapMemoryForm form;
    form.destination_address = destination_address;
    form.source_address      = (void*)source_address;
    form.byte_count          = byte_count;
    return BqSyscall(ABI_SC_MM_COPY_HEAP_MEMORY, &form);
}

BqStatus BqFillHeapMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count)
{
    if (destination_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFillHeapMemoryForm form;
    form.destination_address = destination_address;
    form.byte_value          = byte_value;
    form.byte_count          = byte_count;
    return BqSyscall(ABI_SC_MM_FILL_HEAP_MEMORY, &form);
}

BqStatus BqZeroHeapMemory(void* destination_address, UInt64 byte_count)
{
    if (destination_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroHeapMemoryForm form;
    form.destination_address = destination_address;
    form.byte_count          = byte_count;
    return BqSyscall(ABI_SC_MM_ZERO_HEAP_MEMORY, &form);
}

BqStatus BqCompareHeapMemory(const void* first_address, const void* second_address, UInt64 byte_count, UInt8* is_equal)
{
    if (first_address == NULL || second_address == NULL || is_equal == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCompareHeapMemoryForm form;
    form.first_address  = (void*)first_address;
    form.second_address = (void*)second_address;
    form.byte_count     = byte_count;
    form.is_equal       = 0;
    BqStatus status = BqSyscall(ABI_SC_MM_COMPARE_HEAP_MEMORY, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *is_equal = form.is_equal;
    }

    return status;
}

BqStatus BqCreateAddressSpace(UInt64* page_table_address)
{
    if (page_table_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCreateAddressSpaceForm form;
    form.page_table_address = 0;
    BqStatus status = BqSyscall(ABI_SC_MM_CREATE_ADDRESS_SPACE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *page_table_address = form.page_table_address;
    }

    return status;
}

BqStatus BqDestroyAddressSpace(UInt64 page_table_address)
{
    AbiScDestroyAddressSpaceForm form;
    form.page_table_address = page_table_address;
    return BqSyscall(ABI_SC_MM_DESTROY_ADDRESS_SPACE, &form);
}

BqStatus BqMapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags)
{
    AbiScMapVirtualRegionInSpaceForm form;
    form.page_table_address = page_table_address;
    form.virtual_address    = virtual_address;
    form.physical_address   = physical_address;
    form.page_count         = page_count;
    form.flags              = flags;
    return BqSyscall(ABI_SC_MM_MAP_VIRTUAL_REGION_IN_SPACE, &form);
}

BqStatus BqUnmapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 page_count)
{
    AbiScUnmapVirtualRegionInSpaceForm form;
    form.page_table_address = page_table_address;
    form.virtual_address    = virtual_address;
    form.page_count         = page_count;
    return BqSyscall(ABI_SC_MM_UNMAP_VIRTUAL_REGION_IN_SPACE, &form);
}

BqStatus BqGetPhysicalAddressInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64* physical_address)
{
    if (physical_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetPhysicalAddressInSpaceForm form;
    form.page_table_address = page_table_address;
    form.virtual_address    = virtual_address;
    form.physical_address   = 0;
    BqStatus status = BqSyscall(ABI_SC_MM_GET_PHYSICAL_ADDRESS_IN_SPACE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *physical_address = form.physical_address;
    }

    return status;
}
