// Made by Berkay

#include "ex_private.h"

AbiScStatus ExKitFromBkStatus(BkStatus status)
{
    switch (status)
    {
        case BK_STATUS_SUCCESS:          return ABI_SC_STATUS_SUCCESS;
        case BK_STATUS_CP_FAILURE:       return ABI_SC_STATUS_HARDWARE_FAILURE;
        case BK_STATUS_HW_FAILURE:       return ABI_SC_STATUS_HARDWARE_FAILURE;
        case BK_STATUS_MM_FAILURE:       return ABI_SC_STATUS_OUT_OF_MEMORY;
        case BK_STATUS_ACPI_FAILURE:     return ABI_SC_STATUS_HARDWARE_FAILURE;
        case BK_STATUS_INVALID_ARGUMENT: return ABI_SC_STATUS_INVALID_ARGUMENT;
        case BK_STATUS_DEVICE_NOT_FOUND: return ABI_SC_STATUS_NOT_FOUND;
        case BK_STATUS_IO_FAILURE:       return ABI_SC_STATUS_IO_ERROR;
        default:                         return ABI_SC_STATUS_INTERNAL_ERROR;
    }
}

AbiScStatus ExKitFromCpStatus(CpStatus status)
{
    switch (status)
    {
        case CP_STATUS_SUCCESS:             return ABI_SC_STATUS_SUCCESS;
        case CP_STATUS_INVALID_ARGUMENT:    return ABI_SC_STATUS_INVALID_ARGUMENT;
        case CP_STATUS_OUT_OF_BOUNDS:       return ABI_SC_STATUS_OUT_OF_BOUNDS;
        case CP_STATUS_SLOT_OCCUPIED:       return ABI_SC_STATUS_BUSY;
        case CP_STATUS_UNSUPPORTED_FEATURE: return ABI_SC_STATUS_NOT_SUPPORTED;
        case CP_STATUS_NOT_INITIALIZED:     return ABI_SC_STATUS_NOT_READY;
        case CP_STATUS_HARDWARE_FAILURE:    return ABI_SC_STATUS_HARDWARE_FAILURE;
        default:                            return ABI_SC_STATUS_INTERNAL_ERROR;
    }
}

AbiScStatus ExKitFromMmStatus(MmStatus status)
{
    switch (status)
    {
        case MM_STATUS_SUCCESS:           return ABI_SC_STATUS_SUCCESS;
        case MM_STATUS_INVALID_ARGUMENT:  return ABI_SC_STATUS_INVALID_ARGUMENT;
        case MM_STATUS_OUT_OF_BOUNDS:     return ABI_SC_STATUS_OUT_OF_BOUNDS;
        case MM_STATUS_UNALIGNED_ADDRESS: return ABI_SC_STATUS_INVALID_ARGUMENT;
        case MM_STATUS_OUT_OF_MEMORY:     return ABI_SC_STATUS_OUT_OF_MEMORY;
        case MM_STATUS_ALREADY_FREE:      return ABI_SC_STATUS_INVALID_ARGUMENT;
        case MM_STATUS_ALREADY_MAPPED:    return ABI_SC_STATUS_ALREADY_EXISTS;
        case MM_STATUS_NOT_MAPPED:        return ABI_SC_STATUS_NOT_FOUND;
        default:                          return ABI_SC_STATUS_INTERNAL_ERROR;
    }
}

AbiScStatus ExKitFromHwStatus(HwStatus status)
{
    switch (status)
    {
        case HW_STATUS_SUCCESS:          return ABI_SC_STATUS_SUCCESS;
        case HW_STATUS_INVALID_ARGUMENT: return ABI_SC_STATUS_INVALID_ARGUMENT;
        case HW_STATUS_DEVICE_NOT_FOUND: return ABI_SC_STATUS_NOT_FOUND;
        case HW_STATUS_DEVICE_BUSY:      return ABI_SC_STATUS_BUSY;
        case HW_STATUS_TIMEOUT:          return ABI_SC_STATUS_BUSY;
        case HW_STATUS_IO_FAILURE:       return ABI_SC_STATUS_IO_ERROR;
        default:                         return ABI_SC_STATUS_INTERNAL_ERROR;
    }
}

AbiScStatus ExKitFromPsStatus(PsStatus status)
{
    switch (status)
    {
        case PS_STATUS_SUCCESS:           return ABI_SC_STATUS_SUCCESS;
        case PS_STATUS_INVALID_PARAMETER: return ABI_SC_STATUS_INVALID_ARGUMENT;
        case PS_STATUS_OUT_OF_MEMORY:     return ABI_SC_STATUS_OUT_OF_MEMORY;
        case PS_STATUS_OUT_OF_RESOURCES:  return ABI_SC_STATUS_OUT_OF_MEMORY;
        case PS_STATUS_NOT_FOUND:         return ABI_SC_STATUS_NOT_FOUND;
        case PS_STATUS_BUSY:              return ABI_SC_STATUS_BUSY;
        case PS_STATUS_INVALID_STATE:     return ABI_SC_STATUS_NOT_READY;
        case PS_STATUS_INTERNAL_ERROR:    return ABI_SC_STATUS_INTERNAL_ERROR;
        default:                          return ABI_SC_STATUS_INTERNAL_ERROR;
    }
}

AbiScStatus ExKitFromFsStatus(FsStatus status)
{
    switch (status)
    {
        case FS_STATUS_SUCCESS:           return ABI_SC_STATUS_SUCCESS;
        case FS_STATUS_INVALID_PARAMETER: return ABI_SC_STATUS_INVALID_ARGUMENT;
        case FS_STATUS_OUT_OF_MEMORY:     return ABI_SC_STATUS_OUT_OF_MEMORY;
        case FS_STATUS_OUT_OF_RESOURCES:  return ABI_SC_STATUS_OUT_OF_MEMORY;
        case FS_STATUS_NOT_FOUND:         return ABI_SC_STATUS_NOT_FOUND;
        case FS_STATUS_ALREADY_EXISTS:    return ABI_SC_STATUS_ALREADY_EXISTS;
        case FS_STATUS_NOT_SUPPORTED:     return ABI_SC_STATUS_NOT_SUPPORTED;
        case FS_STATUS_BUSY:              return ABI_SC_STATUS_BUSY;
        case FS_STATUS_END_OF_FILE:       return ABI_SC_STATUS_END_OF_FILE;
        case FS_STATUS_INVALID_PATH:      return ABI_SC_STATUS_INVALID_ARGUMENT;
        case FS_STATUS_ACCESS_DENIED:     return ABI_SC_STATUS_ACCESS_DENIED;
        case FS_STATUS_IO_ERROR:          return ABI_SC_STATUS_IO_ERROR;
        case FS_STATUS_INTERNAL_ERROR:    return ABI_SC_STATUS_INTERNAL_ERROR;
        default:                          return ABI_SC_STATUS_INTERNAL_ERROR;
    }
}

ExStatus ExKitToExStatus(AbiScStatus status)
{
    switch (status)
    {
        case ABI_SC_STATUS_SUCCESS:          return EX_STATUS_SUCCESS;
        case ABI_SC_STATUS_INVALID_ARGUMENT: return EX_STATUS_INVALID_ARGUMENT;
        case ABI_SC_STATUS_ACCESS_DENIED:    return EX_STATUS_ACCESS_DENIED;
        case ABI_SC_STATUS_NOT_FOUND:        return EX_STATUS_NOT_FOUND;
        case ABI_SC_STATUS_OUT_OF_MEMORY:    return EX_STATUS_OUT_OF_MEMORY;
        case ABI_SC_STATUS_OUT_OF_BOUNDS:    return EX_STATUS_INVALID_ARGUMENT;
        case ABI_SC_STATUS_BUSY:             return EX_STATUS_BUSY;
        case ABI_SC_STATUS_NOT_READY:        return EX_STATUS_BUSY;
        case ABI_SC_STATUS_NOT_SUPPORTED:    return EX_STATUS_NOT_SUPPORTED;
        case ABI_SC_STATUS_ALREADY_EXISTS:   return EX_STATUS_INVALID_ARGUMENT;
        case ABI_SC_STATUS_IO_ERROR:         return EX_STATUS_INTERNAL_ERROR;
        case ABI_SC_STATUS_END_OF_FILE:      return EX_STATUS_SUCCESS;
        case ABI_SC_STATUS_INVALID_SYSCALL:  return EX_STATUS_INVALID_SYSCALL;
        case ABI_SC_STATUS_HARDWARE_FAILURE: return EX_STATUS_INTERNAL_ERROR;
        case ABI_SC_STATUS_INTERNAL_ERROR:   return EX_STATUS_INTERNAL_ERROR;
        default:                             return EX_STATUS_INTERNAL_ERROR;
    }
}

ExStatus ExKitValidateUserPointer(const void* pointer, UInt64 size)
{
    if (pointer == NULL)
    {
        return EX_STATUS_INVALID_ARGUMENT;
    }

    UInt64 addr = (UInt64)pointer;

    if (addr + size < addr)
    {
        return EX_STATUS_INVALID_ARGUMENT;
    }

    return EX_STATUS_SUCCESS;
}

ExStatus ExKitGetCallerPrivilege(PsProcessPrivilege* privilege, CpContext* context)
{
    if (privilege == NULL)
    {
        return EX_STATUS_INVALID_ARGUMENT;
    }

    if (context != NULL && (context->cs & 0x03ULL) == 0ULL)
    {
        *privilege = PS_PROCESS_PRIVILEGE_KERNEL;
        return EX_STATUS_SUCCESS;
    }

    UInt32 process_id = 0;
    PsStatus status = PsProcessGetCurrentId(&process_id);
    if (status != PS_STATUS_SUCCESS)
    {
        *privilege = PS_PROCESS_PRIVILEGE_KERNEL;
        return EX_STATUS_SUCCESS;
    }

    if (ExLdrIsDriverProcess(process_id) != 0)
    {
        *privilege = PS_PROCESS_PRIVILEGE_KERNEL;
        return EX_STATUS_SUCCESS;
    }

    PsProcess process;
    status = PsProcessGet(&process, process_id);
    if (status != PS_STATUS_SUCCESS)
    {
        *privilege = PS_PROCESS_PRIVILEGE_USER;
        return EX_STATUS_SUCCESS;
    }

    *privilege = process.privilege;
    return EX_STATUS_SUCCESS;
}
