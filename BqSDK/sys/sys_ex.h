// Made by Berkay

#ifndef SYS_EX_H
#define SYS_EX_H

#include "sys_core.h"

BqStatus BqGetSyscallCount(UInt32* count);
BqStatus BqGetSyscallDescriptor(UInt32 syscall_id, PsProcessPrivilege* min_privilege, UInt32* form_size, char* name_buffer, UInt32 name_buffer_size);
BqStatus BqGetSyscallDescriptorRaw(AbiScGetSyscallDescriptorForm* form);

#endif
