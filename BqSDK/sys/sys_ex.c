// Made by Berkay

#include "sys_ex.h"

BqStatus BqGetSyscallCount(UInt32* count)
{
    if (count == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetSyscallCountForm form;
    form.count = 0;
    BqStatus status = BqSyscall(ABI_SC_EX_GET_SYSCALL_COUNT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *count = form.count;
    }

    return status;
}

BqStatus BqGetSyscallDescriptor(UInt32 syscall_id, PsProcessPrivilege* min_privilege, UInt32* form_size, char* name_buffer, UInt32 name_buffer_size)
{
    AbiScGetSyscallDescriptorForm form;
    form.syscall_id = syscall_id;
    for (UInt64 i = 0; i < sizeof(form.name); i++)
    {
        form.name[i] = '\0';
    }

    BqStatus status = BqSyscall(ABI_SC_EX_GET_SYSCALL_DESCRIPTOR, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        if (min_privilege != NULL)
        {
            *min_privilege = form.min_privilege;
        }

        if (form_size != NULL)
        {
            *form_size = form.form_size;
        }

        if (name_buffer != NULL && name_buffer_size > 0)
        {
            UInt32 i = 0;
            while (i < name_buffer_size - 1 && form.name[i] != '\0')
            {
                name_buffer[i] = form.name[i];
                i++;
            }
            name_buffer[i] = '\0';
        }
    }

    return status;
}

BqStatus BqGetSyscallDescriptorRaw(AbiScGetSyscallDescriptorForm* form)
{
    if (form == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    return BqSyscall(ABI_SC_EX_GET_SYSCALL_DESCRIPTOR, form);
}
