// Made by Berkay

#include "ps_private.h"

void PsLoad(void)
{
    PsSchLoad();
}

PsStatus PsInit(void)
{
    PsStatus status = PsLckInit();
    if (status != PS_STATUS_SUCCESS)
    {
        return status;
    }

    status = PsPrcInit();
    if (status != PS_STATUS_SUCCESS)
    {
        return status;
    }

    status = PsThrInit();
    if (status != PS_STATUS_SUCCESS)
    {
        return status;
    }

    status = PsSchInit();
    if (status != PS_STATUS_SUCCESS)
    {
        return status;
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsStartScheduling(void)
{
    return PsSchStart();
}

PsStatus PsSpinlockCreate(UInt32* lock_id)
{
    return PsLckSpinlockCreate(lock_id);
}

PsStatus PsSpinlockAcquire(UInt32 lock_id)
{
    return PsLckSpinlockAcquire(lock_id);
}

PsStatus PsSpinlockRelease(UInt32 lock_id)
{
    return PsLckSpinlockRelease(lock_id);
}

PsStatus PsSpinlockDestroy(UInt32 lock_id)
{
    return PsLckSpinlockDestroy(lock_id);
}

PsStatus PsMutexCreate(UInt32* mutex_id)
{
    return PsLckMutexCreate(mutex_id);
}

PsStatus PsMutexAcquire(UInt32 mutex_id)
{
    return PsLckMutexAcquire(mutex_id);
}

PsStatus PsMutexRelease(UInt32 mutex_id)
{
    return PsLckMutexRelease(mutex_id);
}

PsStatus PsMutexDestroy(UInt32 mutex_id)
{
    return PsLckMutexDestroy(mutex_id);
}

PsStatus PsGetLock(PsLock* lock, UInt32 lock_id)
{
    return PsLckGet(lock, lock_id);
}

PsStatus PsProcessCreate(UInt32* process_id, PsProcessPrivilege privilege, PsProcessPriority priority)
{
    return PsPrcCreate(process_id, (PsPrcPrivilege)privilege, (PsPrcPriority)priority);
}

PsStatus PsProcessDestroy(UInt32 process_id)
{
    return PsPrcDestroy(process_id);
}

PsStatus PsProcessGet(PsProcess* process, UInt32 process_id)
{
    return PsPrcGetPublic(process, process_id);
}

PsStatus PsProcessGetCurrentId(UInt32* process_id)
{
    return PsPrcGetCurrentId(process_id);
}

PsStatus PsThreadCreate(UInt32* thread_id, UInt32 process_id, PsThreadEntry entry, void* argument, PsThreadPriority priority)
{
    return PsThrCreate(thread_id, process_id, entry, argument, (PsThrPriority)priority);
}

PsStatus PsThreadDestroy(UInt32 thread_id)
{
    return PsThrDestroy(thread_id);
}

PsStatus PsThreadGet(PsThread* thread, UInt32 thread_id)
{
    return PsThrGetPublic(thread, thread_id);
}

PsStatus PsThreadGetCurrentId(UInt32* thread_id)
{
    return PsThrGetCurrentId(thread_id);
}

PsStatus PsThreadGetContext(PsContext* context, UInt32 thread_id)
{
    return PsThrGetContext(context, thread_id);
}

PsStatus PsThreadSetContext(UInt32 thread_id, PsContext* context)
{
    return PsThrSetContext(thread_id, context);
}

PsStatus PsTick(void)
{
    return PsSchTick();
}

PsStatus PsYield(void)
{
    return PsSchYield();
}

PsStatus PsSleep(UInt64 ticks)
{
    return PsSchSleep(ticks);
}

PsStatus PsBlock(UInt32 thread_id)
{
    return PsSchBlock(thread_id);
}

PsStatus PsUnblock(UInt32 thread_id)
{
    return PsSchUnblock(thread_id);
}
