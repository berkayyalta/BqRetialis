// Made by Berkay

#include "mm_private.h"

MmStatus MmKitCopyPhysicalMemory(void* destination_address, void* source_address, UInt64 byte_count)
{
    if (destination_address == 0 || source_address == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt8* dst = 0;
    UInt8* src = 0;

    MmVmPhysicalToVirtual((void**)&dst, (UInt64)destination_address);
    MmVmPhysicalToVirtual((void**)&src, (UInt64)source_address);

    if (dst == src)
    {
        return MM_STATUS_SUCCESS;
    }

    if (dst < src)
    {
        for (UInt64 i = 0; i < byte_count; i++)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        for (UInt64 i = byte_count; i > 0; i--)
        {
            dst[i - 1] = src[i - 1];
        }
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmKitFillPhysicalMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count)
{
    if (destination_address == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt8* dst = 0;
    MmVmPhysicalToVirtual((void**)&dst, (UInt64)destination_address);

    for (UInt64 i = 0; i < byte_count; i++)
    {
        dst[i] = byte_value;
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmKitZeroPhysicalMemory(void* destination_address, UInt64 byte_count)
{
    return MmKitFillPhysicalMemory(destination_address, 0, byte_count);
}

MmStatus MmKitComparePhysicalMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count)
{
    if (is_equal == 0 || first_address == 0 || second_address == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt8* first  = 0;
    UInt8* second = 0;

    MmVmPhysicalToVirtual((void**)&first, (UInt64)first_address);
    MmVmPhysicalToVirtual((void**)&second, (UInt64)second_address);

    for (UInt64 i = 0; i < byte_count; i++)
    {
        if (first[i] != second[i])
        {
            *is_equal = 0;
            return MM_STATUS_SUCCESS;
        }
    }

    *is_equal = 1;

    return MM_STATUS_SUCCESS;
}

MmStatus MmKitCopyVirtualMemory(void* destination_address, void* source_address, UInt64 byte_count)
{
    if (destination_address == 0 || source_address == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if (destination_address == source_address)
    {
        return MM_STATUS_SUCCESS;
    }

    UInt8* dst = (UInt8*)destination_address;
    UInt8* src = (UInt8*)source_address;

    if (dst < src)
    {
        for (UInt64 i = 0; i < byte_count; i++)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        for (UInt64 i = byte_count; i > 0; i--)
        {
            dst[i - 1] = src[i - 1];
        }
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmKitFillVirtualMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count)
{
    if (destination_address == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt8* dst = (UInt8*)destination_address;

    for (UInt64 i = 0; i < byte_count; i++)
    {
        dst[i] = byte_value;
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmKitZeroVirtualMemory(void* destination_address, UInt64 byte_count)
{
    return MmKitFillVirtualMemory(destination_address, 0, byte_count);
}

MmStatus MmKitCompareVirtualMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count)
{
    if (is_equal == 0 || first_address == 0 || second_address == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt8* first  = (UInt8*)first_address;
    UInt8* second = (UInt8*)second_address;

    for (UInt64 i = 0; i < byte_count; i++)
    {
        if (first[i] != second[i])
        {
            *is_equal = 0;
            return MM_STATUS_SUCCESS;
        }
    }

    *is_equal = 1;

    return MM_STATUS_SUCCESS;
}

MmStatus MmKitCopyHeapMemory(void* destination_address, void* source_address, UInt64 byte_count)
{
    if (destination_address == 0 || source_address == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if (destination_address == source_address)
    {
        return MM_STATUS_SUCCESS;
    }

    UInt8* dst = (UInt8*)destination_address;
    UInt8* src = (UInt8*)source_address;

    if (dst < src)
    {
        for (UInt64 i = 0; i < byte_count; i++)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        for (UInt64 i = byte_count; i > 0; i--)
        {
            dst[i - 1] = src[i - 1];
        }
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmKitFillHeapMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count)
{
    if (destination_address == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt8* dst = (UInt8*)destination_address;

    for (UInt64 i = 0; i < byte_count; i++)
    {
        dst[i] = byte_value;
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmKitZeroHeapMemory(void* destination_address, UInt64 byte_count)
{
    return MmKitFillHeapMemory(destination_address, 0, byte_count);
}

MmStatus MmKitCompareHeapMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count)
{
    if (is_equal == 0 || first_address == 0 || second_address == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt8* first  = (UInt8*)first_address;
    UInt8* second = (UInt8*)second_address;

    for (UInt64 i = 0; i < byte_count; i++)
    {
        if (first[i] != second[i])
        {
            *is_equal = 0;
            return MM_STATUS_SUCCESS;
        }
    }

    *is_equal = 1;

    return MM_STATUS_SUCCESS;
}
