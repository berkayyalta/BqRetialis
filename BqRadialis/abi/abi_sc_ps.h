// Made by Berkay

#ifndef ABI_SC_PS_H
#define ABI_SC_PS_H

#include "abi_sc_status.h"
#include "../ps/ps_public.h"

struct AbiScSpinlockCreateForm
{
    UInt32 lock_id;
};
typedef struct AbiScSpinlockCreateForm AbiScSpinlockCreateForm;
AbiScStatus AbiScSpinlockCreate(AbiScSpinlockCreateForm* form);

struct AbiScSpinlockAcquireForm
{
    UInt32 lock_id;
};
typedef struct AbiScSpinlockAcquireForm AbiScSpinlockAcquireForm;
AbiScStatus AbiScSpinlockAcquire(AbiScSpinlockAcquireForm* form);

struct AbiScSpinlockReleaseForm
{
    UInt32 lock_id;
};
typedef struct AbiScSpinlockReleaseForm AbiScSpinlockReleaseForm;
AbiScStatus AbiScSpinlockRelease(AbiScSpinlockReleaseForm* form);

struct AbiScSpinlockDestroyForm
{
    UInt32 lock_id;
};
typedef struct AbiScSpinlockDestroyForm AbiScSpinlockDestroyForm;
AbiScStatus AbiScSpinlockDestroy(AbiScSpinlockDestroyForm* form);

struct AbiScMutexCreateForm
{
    UInt32 mutex_id;
};
typedef struct AbiScMutexCreateForm AbiScMutexCreateForm;
AbiScStatus AbiScMutexCreate(AbiScMutexCreateForm* form);

struct AbiScMutexAcquireForm
{
    UInt32 mutex_id;
};
typedef struct AbiScMutexAcquireForm AbiScMutexAcquireForm;
AbiScStatus AbiScMutexAcquire(AbiScMutexAcquireForm* form);

struct AbiScMutexReleaseForm
{
    UInt32 mutex_id;
};
typedef struct AbiScMutexReleaseForm AbiScMutexReleaseForm;
AbiScStatus AbiScMutexRelease(AbiScMutexReleaseForm* form);

struct AbiScMutexDestroyForm
{
    UInt32 mutex_id;
};
typedef struct AbiScMutexDestroyForm AbiScMutexDestroyForm;
AbiScStatus AbiScMutexDestroy(AbiScMutexDestroyForm* form);

struct AbiScGetLockForm
{
    PsLock lock;
    UInt32 lock_id;
};
typedef struct AbiScGetLockForm AbiScGetLockForm;
AbiScStatus AbiScGetLock(AbiScGetLockForm* form);

struct AbiScProcessCreateForm
{
    UInt32             process_id;
    PsProcessPrivilege privilege;
    PsProcessPriority  priority;
};
typedef struct AbiScProcessCreateForm AbiScProcessCreateForm;
AbiScStatus AbiScProcessCreate(AbiScProcessCreateForm* form);

struct AbiScProcessDestroyForm
{
    UInt32 process_id;
};
typedef struct AbiScProcessDestroyForm AbiScProcessDestroyForm;
AbiScStatus AbiScProcessDestroy(AbiScProcessDestroyForm* form);

struct AbiScProcessGetForm
{
    PsProcess process;
    UInt32    process_id;
};
typedef struct AbiScProcessGetForm AbiScProcessGetForm;
AbiScStatus AbiScProcessGet(AbiScProcessGetForm* form);

struct AbiScProcessGetCurrentIdForm
{
    UInt32 process_id;
};
typedef struct AbiScProcessGetCurrentIdForm AbiScProcessGetCurrentIdForm;
AbiScStatus AbiScProcessGetCurrentId(AbiScProcessGetCurrentIdForm* form);

struct AbiScThreadCreateForm
{
    UInt32           thread_id;
    UInt32           process_id;
    PsThreadEntry    entry;
    void*            argument;
    PsThreadPriority priority;
};
typedef struct AbiScThreadCreateForm AbiScThreadCreateForm;
AbiScStatus AbiScThreadCreate(AbiScThreadCreateForm* form);

struct AbiScThreadDestroyForm
{
    UInt32 thread_id;
};
typedef struct AbiScThreadDestroyForm AbiScThreadDestroyForm;
AbiScStatus AbiScThreadDestroy(AbiScThreadDestroyForm* form);

struct AbiScThreadGetForm
{
    PsThread thread;
    UInt32   thread_id;
};
typedef struct AbiScThreadGetForm AbiScThreadGetForm;
AbiScStatus AbiScThreadGet(AbiScThreadGetForm* form);

struct AbiScThreadGetCurrentIdForm
{
    UInt32 thread_id;
};
typedef struct AbiScThreadGetCurrentIdForm AbiScThreadGetCurrentIdForm;
AbiScStatus AbiScThreadGetCurrentId(AbiScThreadGetCurrentIdForm* form);

struct AbiScThreadGetContextForm
{
    PsContext context;
    UInt32    thread_id;
};
typedef struct AbiScThreadGetContextForm AbiScThreadGetContextForm;
AbiScStatus AbiScThreadGetContext(AbiScThreadGetContextForm* form);

struct AbiScThreadSetContextForm
{
    UInt32    thread_id;
    PsContext context;
};
typedef struct AbiScThreadSetContextForm AbiScThreadSetContextForm;
AbiScStatus AbiScThreadSetContext(AbiScThreadSetContextForm* form);

struct AbiScYieldForm
{
    UInt32 reserved;
};
typedef struct AbiScYieldForm AbiScYieldForm;
AbiScStatus AbiScYield(AbiScYieldForm* form);

struct AbiScSleepForm
{
    UInt64 ticks;
};
typedef struct AbiScSleepForm AbiScSleepForm;
AbiScStatus AbiScSleep(AbiScSleepForm* form);

struct AbiScBlockForm
{
    UInt32 thread_id;
};
typedef struct AbiScBlockForm AbiScBlockForm;
AbiScStatus AbiScBlock(AbiScBlockForm* form);

struct AbiScUnblockForm
{
    UInt32 thread_id;
};
typedef struct AbiScUnblockForm AbiScUnblockForm;
AbiScStatus AbiScUnblock(AbiScUnblockForm* form);

#endif
