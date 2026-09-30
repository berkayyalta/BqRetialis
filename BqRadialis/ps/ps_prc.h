// Made by Berkay

#ifndef PS_PRC_H
#define PS_PRC_H

#include "../bk/bk_types.h"

enum PsPrcLimit: UInt32
{
    PS_PRC_MAX_PROCESSES = 256
};

enum PsPrcState: UInt32
{
    PS_PRC_STATE_FREE       = 0,
    PS_PRC_STATE_READY      = 1,
    PS_PRC_STATE_RUNNING    = 2,
    PS_PRC_STATE_TERMINATED = 3
};

enum PsPrcPriority: UInt32
{
    PS_PRC_PRIORITY_IDLE     = 0,
    PS_PRC_PRIORITY_LOW      = 1,
    PS_PRC_PRIORITY_NORMAL   = 2,
    PS_PRC_PRIORITY_HIGH     = 3,
    PS_PRC_PRIORITY_REALTIME = 4
};

enum PsPrcPrivilege: UInt32
{
    PS_PRC_PRIVILEGE_KERNEL = 0,
    PS_PRC_PRIVILEGE_USER   = 1
};

struct PsPrcProcess
{
    UInt32                  process_id;
    UInt32                  parent_process_id;
    enum PsPrcState         state;
    enum PsPrcPriority      priority;
    enum PsPrcPrivilege     privilege;
    UInt64                  page_table_address;
    UInt32                  thread_count;
    UInt8                   is_allocated;
};

struct PsPrcLedger
{
    struct PsPrcProcess     processes[PS_PRC_MAX_PROCESSES];
    UInt32                  process_count;
    volatile UInt32         ledger_lock;
    UInt8                   is_initialized;
};

#endif
