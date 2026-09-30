// Made by Berkay

#include "ex_private.h"

void ExLoad(void)
{
}

ExStatus ExInit(void)
{
    ExStatus status = ExSysInit();
    if (status != EX_STATUS_SUCCESS)
    {
        return status;
    }

    CpStatus cp_status = CpInitSyscall(ExSysDispatchHandler);
    if (cp_status != CP_STATUS_SUCCESS)
    {
        return EX_STATUS_INTERNAL_ERROR;
    }

    status = ExLdrInit();
    if (status != EX_STATUS_SUCCESS)
    {
        return status;
    }

    status = ExLdrStartup();
    if (status != EX_STATUS_SUCCESS)
    {
        return status;
    }

    return EX_STATUS_SUCCESS;
}

ExStatus ExDispatchSyscall(CpContext* context)
{
    return ExSysDispatch(context);
}

ExStatus ExGetSyscallCount(UInt32* count)
{
    return ExSysGetCount(count);
}

ExStatus ExGetSyscallDescriptor(ExSyscallDescriptor* descriptor, UInt32 syscall_id)
{
    return ExSysGetDescriptor(descriptor, syscall_id);
}
