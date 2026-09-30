// Made by Berkay

#include "sys_ps.h"

BqStatus BqSpinlockCreate(UInt32* lock_id)
{
    if (lock_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScSpinlockCreateForm form;
    form.lock_id = 0;
    BqStatus status = BqSyscall(ABI_SC_PS_SPINLOCK_CREATE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *lock_id = form.lock_id;
    }

    return status;
}

BqStatus BqSpinlockAcquire(UInt32 lock_id)
{
    AbiScSpinlockAcquireForm form;
    form.lock_id = lock_id;
    return BqSyscall(ABI_SC_PS_SPINLOCK_ACQUIRE, &form);
}

BqStatus BqSpinlockRelease(UInt32 lock_id)
{
    AbiScSpinlockReleaseForm form;
    form.lock_id = lock_id;
    return BqSyscall(ABI_SC_PS_SPINLOCK_RELEASE, &form);
}

BqStatus BqSpinlockDestroy(UInt32 lock_id)
{
    AbiScSpinlockDestroyForm form;
    form.lock_id = lock_id;
    return BqSyscall(ABI_SC_PS_SPINLOCK_DESTROY, &form);
}

BqStatus BqMutexCreate(UInt32* mutex_id)
{
    if (mutex_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScMutexCreateForm form;
    form.mutex_id = 0;
    BqStatus status = BqSyscall(ABI_SC_PS_MUTEX_CREATE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *mutex_id = form.mutex_id;
    }

    return status;
}

BqStatus BqMutexAcquire(UInt32 mutex_id)
{
    AbiScMutexAcquireForm form;
    form.mutex_id = mutex_id;
    return BqSyscall(ABI_SC_PS_MUTEX_ACQUIRE, &form);
}

BqStatus BqMutexRelease(UInt32 mutex_id)
{
    AbiScMutexReleaseForm form;
    form.mutex_id = mutex_id;
    return BqSyscall(ABI_SC_PS_MUTEX_RELEASE, &form);
}

BqStatus BqMutexDestroy(UInt32 mutex_id)
{
    AbiScMutexDestroyForm form;
    form.mutex_id = mutex_id;
    return BqSyscall(ABI_SC_PS_MUTEX_DESTROY, &form);
}

BqStatus BqGetLock(UInt32 lock_id, PsLock* lock)
{
    if (lock == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetLockForm form;
    form.lock_id = lock_id;
    BqStatus status = BqSyscall(ABI_SC_PS_GET_LOCK, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *lock = form.lock;
    }

    return status;
}

BqStatus BqProcessCreate(PsProcessPrivilege privilege, PsProcessPriority priority, UInt32* process_id)
{
    if (process_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScProcessCreateForm form;
    form.privilege  = privilege;
    form.priority   = priority;
    form.process_id = 0;
    BqStatus status = BqSyscall(ABI_SC_PS_PROCESS_CREATE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *process_id = form.process_id;
    }

    return status;
}

BqStatus BqProcessDestroy(UInt32 process_id)
{
    AbiScProcessDestroyForm form;
    form.process_id = process_id;
    return BqSyscall(ABI_SC_PS_PROCESS_DESTROY, &form);
}

BqStatus BqProcessGet(UInt32 process_id, PsProcess* process)
{
    if (process == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScProcessGetForm form;
    form.process_id = process_id;
    BqStatus status = BqSyscall(ABI_SC_PS_PROCESS_GET, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *process = form.process;
    }

    return status;
}

BqStatus BqProcessGetCurrentId(UInt32* process_id)
{
    if (process_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScProcessGetCurrentIdForm form;
    form.process_id = 0;
    BqStatus status = BqSyscall(ABI_SC_PS_PROCESS_GET_CURRENT_ID, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *process_id = form.process_id;
    }

    return status;
}

BqStatus BqThreadCreate(UInt32 process_id, PsThreadEntry entry, void* argument, PsThreadPriority priority, UInt32* thread_id)
{
    if (entry == NULL || thread_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadCreateForm form;
    form.process_id = process_id;
    form.entry      = entry;
    form.argument   = argument;
    form.priority   = priority;
    form.thread_id  = 0;
    BqStatus status = BqSyscall(ABI_SC_PS_THREAD_CREATE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *thread_id = form.thread_id;
    }

    return status;
}

BqStatus BqThreadDestroy(UInt32 thread_id)
{
    AbiScThreadDestroyForm form;
    form.thread_id = thread_id;
    return BqSyscall(ABI_SC_PS_THREAD_DESTROY, &form);
}

BqStatus BqThreadGet(UInt32 thread_id, PsThread* thread)
{
    if (thread == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadGetForm form;
    form.thread_id = thread_id;
    BqStatus status = BqSyscall(ABI_SC_PS_THREAD_GET, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *thread = form.thread;
    }

    return status;
}

BqStatus BqThreadGetCurrentId(UInt32* thread_id)
{
    if (thread_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadGetCurrentIdForm form;
    form.thread_id = 0;
    BqStatus status = BqSyscall(ABI_SC_PS_THREAD_GET_CURRENT_ID, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *thread_id = form.thread_id;
    }

    return status;
}

BqStatus BqThreadGetContext(UInt32 thread_id, PsContext* context)
{
    if (context == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadGetContextForm form;
    form.thread_id = thread_id;
    BqStatus status = BqSyscall(ABI_SC_PS_THREAD_GET_CONTEXT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *context = form.context;
    }

    return status;
}

BqStatus BqThreadSetContext(UInt32 thread_id, const PsContext* context)
{
    if (context == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadSetContextForm form;
    form.thread_id = thread_id;
    form.context   = *context;
    return BqSyscall(ABI_SC_PS_THREAD_SET_CONTEXT, &form);
}

BqStatus BqYield(void)
{
    AbiScYieldForm form;
    form.reserved = 0;
    return BqSyscall(ABI_SC_PS_YIELD, &form);
}

BqStatus BqSleep(UInt64 ticks)
{
    AbiScSleepForm form;
    form.ticks = ticks;
    return BqSyscall(ABI_SC_PS_SLEEP, &form);
}

BqStatus BqBlock(UInt32 thread_id)
{
    AbiScBlockForm form;
    form.thread_id = thread_id;
    return BqSyscall(ABI_SC_PS_BLOCK, &form);
}

BqStatus BqUnblock(UInt32 thread_id)
{
    AbiScUnblockForm form;
    form.thread_id = thread_id;
    return BqSyscall(ABI_SC_PS_UNBLOCK, &form);
}
