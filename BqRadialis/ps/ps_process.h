// Made by Berkay

#ifndef PS_PROCESS_H
#define PS_PROCESS_H

#include "../bk/bk_types.h"

enum PsProcessState: UInt32
{
    PS_PROCESS_STATE_FREE       = 0,
    PS_PROCESS_STATE_READY      = 1,
    PS_PROCESS_STATE_RUNNING    = 2,
    PS_PROCESS_STATE_TERMINATED = 3
};

enum PsProcessPriority: UInt32
{
    PS_PROCESS_PRIORITY_IDLE     = 0,
    PS_PROCESS_PRIORITY_LOW      = 1,
    PS_PROCESS_PRIORITY_NORMAL   = 2,
    PS_PROCESS_PRIORITY_HIGH     = 3,
    PS_PROCESS_PRIORITY_REALTIME = 4
};

enum PsProcessPrivilege: UInt32
{
    PS_PROCESS_PRIVILEGE_KERNEL = 0,
    PS_PROCESS_PRIVILEGE_USER   = 1
};

struct PsProcess
{
    UInt32                  process_id;
    UInt32                  parent_process_id;
    enum PsProcessState     state;
    enum PsProcessPriority  priority;
    enum PsProcessPrivilege privilege;
    UInt64                  page_table_address;
    UInt32                  thread_count;
};

#endif
