// Made by Berkay

#ifndef EX_SYSCALL_H
#define EX_SYSCALL_H

#include "../bk/bk_types.h"
#include "../ps/ps_public.h"
#include "../abi/abi_sc_status.h"

typedef AbiScStatus (*ExSyscallHandler)(void* form, PsProcessPrivilege caller_privilege);

struct ExSyscallDescriptor
{
    UInt32             syscall_id;
    PsProcessPrivilege min_privilege;
    UInt32             form_size;
    ExSyscallHandler   handler;
    const char*        name;
};

#endif
