// Made by Berkay

#ifndef ABI_SC_EX_H
#define ABI_SC_EX_H

#include "abi_sc_status.h"
#include "../ps/ps_public.h"

struct AbiScGetSyscallCountForm
{
    UInt32 count;
};
typedef struct AbiScGetSyscallCountForm AbiScGetSyscallCountForm;
AbiScStatus AbiScGetSyscallCount(AbiScGetSyscallCountForm* form);

struct AbiScGetSyscallDescriptorForm
{
    UInt32             syscall_id;
    PsProcessPrivilege min_privilege;
    UInt32             form_size;
    char               name[32];
};
typedef struct AbiScGetSyscallDescriptorForm AbiScGetSyscallDescriptorForm;
AbiScStatus AbiScGetSyscallDescriptor(AbiScGetSyscallDescriptorForm* form);

#endif
