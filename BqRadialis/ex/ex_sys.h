// Made by Berkay

#ifndef EX_SYS_H
#define EX_SYS_H

#include "../bk/bk_types.h"
#include "ex_status.h"
#include "ex_syscall.h"
#include "../abi/abi_sc_id.h"

struct ExSysLedger
{
    struct ExSyscallDescriptor dispatch_table[ABI_SC_COUNT];
    UInt32                     is_initialized;
    UInt64                     total_dispatches;
};

#endif
