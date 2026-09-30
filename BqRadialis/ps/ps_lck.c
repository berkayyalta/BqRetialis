// Made by Berkay

#include "ps_private.h"

static PsLckLedger s_lck_ledger;

PsStatus PsLckInit(void)
{
    UInt8* raw = (UInt8*)&s_lck_ledger;
    for (UInt64 i = 0; i < sizeof(PsLckLedger); i++)
    {
        raw[i] = 0;
    }

    s_lck_ledger.is_initialized = 1;

    return PS_STATUS_SUCCESS;
}

PsStatus PsLckSpinlockCreate(UInt32* lock_id)
{
    if (lock_id == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsKitSpinlockAcquire(&s_lck_ledger.ledger_lock);

    for (UInt32 i = 0; i < PS_LCK_MAX_SPINLOCKS; i++)
    {
        if (s_lck_ledger.spinlocks[i].is_allocated == 0)
        {
            s_lck_ledger.spinlocks[i].id              = i;
            s_lck_ledger.spinlocks[i].lock            = 0;
            s_lck_ledger.spinlocks[i].owner_core      = 0xFFFFFFFF;
            s_lck_ledger.spinlocks[i].interrupt_state = 0;
            s_lck_ledger.spinlocks[i].recursion_count = 0;
            s_lck_ledger.spinlocks[i].is_allocated    = 1;

            *lock_id = i;

            PsKitSpinlockRelease(&s_lck_ledger.ledger_lock);
            return PS_STATUS_SUCCESS;
        }
    }

    PsKitSpinlockRelease(&s_lck_ledger.ledger_lock);
    return PS_STATUS_OUT_OF_RESOURCES;
}

PsStatus PsLckSpinlockAcquire(UInt32 lock_id)
{
    if (lock_id >= PS_LCK_MAX_SPINLOCKS)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsLckSpinlock* spinlock = &s_lck_ledger.spinlocks[lock_id];
    if (spinlock->is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    UInt32 core_index = 0;
    CpGetCurrentCoreIndex(&core_index);

    if (spinlock->owner_core == core_index)
    {
        spinlock->recursion_count++;
        return PS_STATUS_SUCCESS;
    }

    UInt64 flags = 0;
    CpSaveAndDisableInterrupts(&flags);

    while (__sync_lock_test_and_set(&spinlock->lock, 1U) != 0U)
    {
        CpPause();
    }

    spinlock->owner_core      = core_index;
    spinlock->interrupt_state = flags;
    spinlock->recursion_count = 1;

    return PS_STATUS_SUCCESS;
}

PsStatus PsLckSpinlockRelease(UInt32 lock_id)
{
    if (lock_id >= PS_LCK_MAX_SPINLOCKS)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsLckSpinlock* spinlock = &s_lck_ledger.spinlocks[lock_id];
    if (spinlock->is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    UInt32 core_index = 0;
    CpGetCurrentCoreIndex(&core_index);

    if (spinlock->owner_core != core_index)
    {
        return PS_STATUS_INVALID_STATE;
    }

    if (spinlock->recursion_count > 1)
    {
        spinlock->recursion_count--;
        return PS_STATUS_SUCCESS;
    }

    UInt64 flags = spinlock->interrupt_state;

    spinlock->owner_core      = 0xFFFFFFFF;
    spinlock->recursion_count = 0;

    __sync_lock_release(&spinlock->lock);

    CpRestoreInterrupts(flags);

    return PS_STATUS_SUCCESS;
}

PsStatus PsLckSpinlockDestroy(UInt32 lock_id)
{
    if (lock_id >= PS_LCK_MAX_SPINLOCKS)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsKitSpinlockAcquire(&s_lck_ledger.ledger_lock);

    PsLckSpinlock* spinlock = &s_lck_ledger.spinlocks[lock_id];
    if (spinlock->is_allocated == 0)
    {
        PsKitSpinlockRelease(&s_lck_ledger.ledger_lock);
        return PS_STATUS_NOT_FOUND;
    }

    spinlock->lock            = 0;
    spinlock->owner_core      = 0xFFFFFFFF;
    spinlock->interrupt_state = 0;
    spinlock->recursion_count = 0;
    spinlock->is_allocated    = 0;

    PsKitSpinlockRelease(&s_lck_ledger.ledger_lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsLckMutexCreate(UInt32* mutex_id)
{
    if (mutex_id == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsKitSpinlockAcquire(&s_lck_ledger.ledger_lock);

    for (UInt32 i = 0; i < PS_LCK_MAX_MUTEXES; i++)
    {
        if (s_lck_ledger.mutexes[i].is_allocated == 0)
        {
            s_lck_ledger.mutexes[i].id              = i;
            s_lck_ledger.mutexes[i].lock            = 0;
            s_lck_ledger.mutexes[i].owner_thread_id = 0xFFFFFFFF;
            s_lck_ledger.mutexes[i].recursion_count = 0;
            s_lck_ledger.mutexes[i].is_allocated    = 1;

            *mutex_id = i;

            PsKitSpinlockRelease(&s_lck_ledger.ledger_lock);
            return PS_STATUS_SUCCESS;
        }
    }

    PsKitSpinlockRelease(&s_lck_ledger.ledger_lock);
    return PS_STATUS_OUT_OF_RESOURCES;
}

PsStatus PsLckMutexAcquire(UInt32 mutex_id)
{
    if (mutex_id >= PS_LCK_MAX_MUTEXES)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsLckMutex* mutex = &s_lck_ledger.mutexes[mutex_id];
    if (mutex->is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    UInt32 thread_id = 0;
    PsThrGetCurrentId(&thread_id);

    if (mutex->owner_thread_id == thread_id && mutex->lock != 0)
    {
        mutex->recursion_count++;
        return PS_STATUS_SUCCESS;
    }

    while (__sync_lock_test_and_set(&mutex->lock, 1U) != 0U)
    {
        PsSchYield();
    }

    mutex->owner_thread_id = thread_id;
    mutex->recursion_count = 1;

    return PS_STATUS_SUCCESS;
}

PsStatus PsLckMutexRelease(UInt32 mutex_id)
{
    if (mutex_id >= PS_LCK_MAX_MUTEXES)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsLckMutex* mutex = &s_lck_ledger.mutexes[mutex_id];
    if (mutex->is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    UInt32 thread_id = 0;
    PsThrGetCurrentId(&thread_id);

    if (mutex->owner_thread_id != thread_id)
    {
        return PS_STATUS_INVALID_STATE;
    }

    if (mutex->recursion_count > 1)
    {
        mutex->recursion_count--;
        return PS_STATUS_SUCCESS;
    }

    mutex->owner_thread_id = 0xFFFFFFFF;
    mutex->recursion_count = 0;

    __sync_lock_release(&mutex->lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsLckMutexDestroy(UInt32 mutex_id)
{
    if (mutex_id >= PS_LCK_MAX_MUTEXES)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsKitSpinlockAcquire(&s_lck_ledger.ledger_lock);

    PsLckMutex* mutex = &s_lck_ledger.mutexes[mutex_id];
    if (mutex->is_allocated == 0)
    {
        PsKitSpinlockRelease(&s_lck_ledger.ledger_lock);
        return PS_STATUS_NOT_FOUND;
    }

    mutex->lock            = 0;
    mutex->owner_thread_id = 0xFFFFFFFF;
    mutex->recursion_count = 0;
    mutex->is_allocated    = 0;

    PsKitSpinlockRelease(&s_lck_ledger.ledger_lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsLckGet(PsLock* lock, UInt32 lock_id)
{
    if (lock == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (lock_id < PS_LCK_MAX_SPINLOCKS && s_lck_ledger.spinlocks[lock_id].is_allocated != 0)
    {
        return PsKitLockToPublic(lock, &s_lck_ledger.spinlocks[lock_id], NULL);
    }

    if (lock_id < PS_LCK_MAX_MUTEXES && s_lck_ledger.mutexes[lock_id].is_allocated != 0)
    {
        return PsKitLockToPublic(lock, NULL, &s_lck_ledger.mutexes[lock_id]);
    }

    return PS_STATUS_NOT_FOUND;
}
