// Made by Berkay

#include "cp_private.h"

CpStatus CpKitBroadcastIpi(UInt8 vector, UInt32 flags, UInt8 include_self)
{
    UInt32 shorthand = (include_self != 0)
                       ? CP_APIC_ICR_SHORTHAND_ALL_INC_SELF
                       : CP_APIC_ICR_SHORTHAND_ALL_EX_SELF;

    return CpApicSendIpi(0, vector, flags | shorthand);
}

CpStatus CpKitGetCoreApicId(UInt8* apic_id, UInt32 core_index)
{
    if (apic_id == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    CpSmpCore* core = NULL;

    CpStatus status = CpSmpGetCore(&core, core_index);

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    *apic_id = core->apic_id;

    return CP_STATUS_SUCCESS;
}

CpStatus CpKitSetCoreThreadContext(UInt32 core_index, void* thread_context)
{
    CpSmpCore* core = NULL;

    CpStatus status = CpSmpGetCore(&core, core_index);

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    core->thread_context = thread_context;

    return CP_STATUS_SUCCESS;
}

CpStatus CpKitGetCoreThreadContext(void** thread_context, UInt32 core_index)
{
    if (thread_context == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    CpSmpCore* core = NULL;

    CpStatus status = CpSmpGetCore(&core, core_index);

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    *thread_context = core->thread_context;

    return CP_STATUS_SUCCESS;
}

CpStatus CpKitCopyFpuState(void* destination_buffer, void* source_buffer)
{
    if (destination_buffer == NULL || source_buffer == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    if ((((UInt64)destination_buffer) & 0x0F) != 0 || (((UInt64)source_buffer) & 0x0F) != 0)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    volatile UInt8* destination = (volatile UInt8*)destination_buffer;
    volatile UInt8* source      = (volatile UInt8*)source_buffer;

    for (UInt64 i = 0; i < CP_FPU_STATE_SIZE; i++)
    {
        destination[i] = source[i];
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpKitZeroFpuState(void* fpu_buffer)
{
    if (fpu_buffer == NULL || (((UInt64)fpu_buffer) & 0x0F) != 0)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    volatile UInt8* destination = (volatile UInt8*)fpu_buffer;

    for (UInt64 i = 0; i < CP_FPU_STATE_SIZE; i++)
    {
        destination[i] = 0;
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpKitGetFpuStateSize(UInt64* size, UInt64* alignment)
{
    return CpFpuGetStateSize(size, alignment);
}

CpStatus CpKitCopyContext(CpContext* destination_context, CpContext* source_context)
{
    if (destination_context == NULL || source_context == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    volatile UInt8* destination = (volatile UInt8*)destination_context;
    volatile UInt8* source      = (volatile UInt8*)source_context;

    for (UInt64 i = 0; i < sizeof(CpContext); i++)
    {
        destination[i] = source[i];
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpKitZeroContext(CpContext* context)
{
    if (context == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    volatile UInt8* destination = (volatile UInt8*)context;

    for (UInt64 i = 0; i < sizeof(CpContext); i++)
    {
        destination[i] = 0;
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpKitCompareContext(UInt8* is_equal, CpContext* first_context, CpContext* second_context)
{
    if (is_equal == NULL || first_context == NULL || second_context == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    volatile UInt8* first  = (volatile UInt8*)first_context;
    volatile UInt8* second = (volatile UInt8*)second_context;

    for (UInt64 i = 0; i < sizeof(CpContext); i++)
    {
        if (first[i] != second[i])
        {
            *is_equal = 0;
            return CP_STATUS_SUCCESS;
        }
    }

    *is_equal = 1;

    return CP_STATUS_SUCCESS;
}
