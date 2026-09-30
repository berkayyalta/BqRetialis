// Made by Berkay

#ifndef EX_PUBLIC_H
#define EX_PUBLIC_H

#include "ex_status.h"
#include "ex_syscall.h"
#include "../cp/cp_public.h"

typedef enum   ExStatus ExStatus;
typedef struct ExSyscallDescriptor ExSyscallDescriptor;

void     ExLoad(void);
ExStatus ExInit(void);

ExStatus ExDispatchSyscall(CpContext* context);
ExStatus ExGetSyscallCount(UInt32* count);
ExStatus ExGetSyscallDescriptor(ExSyscallDescriptor* descriptor, UInt32 syscall_id);

#endif
