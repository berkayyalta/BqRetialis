// Made by Berkay

#include "ps_private.h"

PsStatus PsKitSpinlockAcquire(volatile UInt32* lock)
{
    if (lock == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    while (__sync_lock_test_and_set(lock, 1U) != 0U)
    {
        CpPause();
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsKitSpinlockRelease(volatile UInt32* lock)
{
    if (lock == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    __sync_lock_release(lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsKitSelectCore(UInt32* core_index)
{
    if (core_index == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    UInt32 current = 0;
    CpGetCurrentCoreIndex(&current);

    *core_index = current;

    return PS_STATUS_SUCCESS;
}

PsStatus PsKitContextToCp(CpContext* cp_context, PsThrContext* thr_context)
{
    if (cp_context == NULL || thr_context == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    cp_context->cr3              = thr_context->cr3;
    cp_context->r15              = thr_context->r15;
    cp_context->r14              = thr_context->r14;
    cp_context->r13              = thr_context->r13;
    cp_context->r12              = thr_context->r12;
    cp_context->r11              = thr_context->r11;
    cp_context->r10              = thr_context->r10;
    cp_context->r9               = thr_context->r9;
    cp_context->r8               = thr_context->r8;
    cp_context->rbp              = thr_context->rbp;
    cp_context->rdi              = thr_context->rdi;
    cp_context->rsi              = thr_context->rsi;
    cp_context->rdx              = thr_context->rdx;
    cp_context->rcx              = thr_context->rcx;
    cp_context->rbx              = thr_context->rbx;
    cp_context->rax              = thr_context->rax;
    cp_context->vector           = thr_context->vector;
    cp_context->error_code       = thr_context->error_code;
    cp_context->rip              = thr_context->rip;
    cp_context->cs               = thr_context->cs;
    cp_context->rflags           = thr_context->rflags;
    cp_context->rsp              = thr_context->rsp;
    cp_context->ss               = thr_context->ss;
    cp_context->kernel_stack_top = thr_context->kernel_stack_top;

    for (UInt64 i = 0; i < 512; i++)
    {
        cp_context->fpu_state[i] = thr_context->fpu_state[i];
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsKitContextFromCp(PsThrContext* thr_context, CpContext* cp_context)
{
    if (thr_context == NULL || cp_context == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    thr_context->cr3              = cp_context->cr3;
    thr_context->r15              = cp_context->r15;
    thr_context->r14              = cp_context->r14;
    thr_context->r13              = cp_context->r13;
    thr_context->r12              = cp_context->r12;
    thr_context->r11              = cp_context->r11;
    thr_context->r10              = cp_context->r10;
    thr_context->r9               = cp_context->r9;
    thr_context->r8               = cp_context->r8;
    thr_context->rbp              = cp_context->rbp;
    thr_context->rdi              = cp_context->rdi;
    thr_context->rsi              = cp_context->rsi;
    thr_context->rdx              = cp_context->rdx;
    thr_context->rcx              = cp_context->rcx;
    thr_context->rbx              = cp_context->rbx;
    thr_context->rax              = cp_context->rax;
    thr_context->vector           = cp_context->vector;
    thr_context->error_code       = cp_context->error_code;
    thr_context->rip              = cp_context->rip;
    thr_context->cs               = cp_context->cs;
    thr_context->rflags           = cp_context->rflags;
    thr_context->rsp              = cp_context->rsp;
    thr_context->ss               = cp_context->ss;

    return PS_STATUS_SUCCESS;
}

PsStatus PsKitContextToPublic(PsContext* public_context, PsThrContext* thr_context)
{
    if (public_context == NULL || thr_context == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    public_context->r15    = thr_context->r15;
    public_context->r14    = thr_context->r14;
    public_context->r13    = thr_context->r13;
    public_context->r12    = thr_context->r12;
    public_context->r11    = thr_context->r11;
    public_context->r10    = thr_context->r10;
    public_context->r9     = thr_context->r9;
    public_context->r8     = thr_context->r8;
    public_context->rbp    = thr_context->rbp;
    public_context->rdi    = thr_context->rdi;
    public_context->rsi    = thr_context->rsi;
    public_context->rdx    = thr_context->rdx;
    public_context->rcx    = thr_context->rcx;
    public_context->rbx    = thr_context->rbx;
    public_context->rax    = thr_context->rax;
    public_context->rip    = thr_context->rip;
    public_context->cs     = thr_context->cs;
    public_context->rflags = thr_context->rflags;
    public_context->rsp    = thr_context->rsp;
    public_context->ss     = thr_context->ss;
    public_context->cr3    = thr_context->cr3;

    for (UInt64 i = 0; i < 512; i++)
    {
        public_context->fpu_state[i] = thr_context->fpu_state[i];
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsKitContextFromPublic(PsThrContext* thr_context, PsContext* public_context)
{
    if (thr_context == NULL || public_context == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    thr_context->r15    = public_context->r15;
    thr_context->r14    = public_context->r14;
    thr_context->r13    = public_context->r13;
    thr_context->r12    = public_context->r12;
    thr_context->r11    = public_context->r11;
    thr_context->r10    = public_context->r10;
    thr_context->r9     = public_context->r9;
    thr_context->r8     = public_context->r8;
    thr_context->rbp    = public_context->rbp;
    thr_context->rdi    = public_context->rdi;
    thr_context->rsi    = public_context->rsi;
    thr_context->rdx    = public_context->rdx;
    thr_context->rcx    = public_context->rcx;
    thr_context->rbx    = public_context->rbx;
    thr_context->rax    = public_context->rax;
    thr_context->rip    = public_context->rip;
    thr_context->cs     = public_context->cs;
    thr_context->rflags = public_context->rflags;
    thr_context->rsp    = public_context->rsp;
    thr_context->ss     = public_context->ss;
    thr_context->cr3    = public_context->cr3;

    for (UInt64 i = 0; i < 512; i++)
    {
        thr_context->fpu_state[i] = public_context->fpu_state[i];
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsKitLockToPublic(PsLock* public_lock, PsLckSpinlock* spinlock, PsLckMutex* mutex)
{
    if (public_lock == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (spinlock != NULL)
    {
        public_lock->id              = spinlock->id;
        public_lock->type            = PS_LOCK_TYPE_SPINLOCK;
        public_lock->state           = (spinlock->lock != 0) ? PS_LOCK_STATE_LOCKED : PS_LOCK_STATE_UNLOCKED;
        public_lock->owner_thread_id = spinlock->owner_core;
        public_lock->recursion_count = spinlock->recursion_count;
        return PS_STATUS_SUCCESS;
    }

    if (mutex != NULL)
    {
        public_lock->id              = mutex->id;
        public_lock->type            = PS_LOCK_TYPE_MUTEX;
        public_lock->state           = (mutex->lock != 0) ? PS_LOCK_STATE_LOCKED : PS_LOCK_STATE_UNLOCKED;
        public_lock->owner_thread_id = mutex->owner_thread_id;
        public_lock->recursion_count = mutex->recursion_count;
        return PS_STATUS_SUCCESS;
    }

    return PS_STATUS_INVALID_PARAMETER;
}

PsStatus PsKitProcessToPublic(PsProcess* public_process, PsPrcProcess* prc_process)
{
    if (public_process == NULL || prc_process == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    public_process->process_id         = prc_process->process_id;
    public_process->parent_process_id  = prc_process->parent_process_id;
    public_process->state              = (PsProcessState)prc_process->state;
    public_process->priority           = (PsProcessPriority)prc_process->priority;
    public_process->privilege          = (PsProcessPrivilege)prc_process->privilege;
    public_process->page_table_address = prc_process->page_table_address;
    public_process->thread_count       = prc_process->thread_count;

    return PS_STATUS_SUCCESS;
}

PsStatus PsKitThreadToPublic(PsThread* public_thread, PsThrThread* thr_thread)
{
    if (public_thread == NULL || thr_thread == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    public_thread->thread_id         = thr_thread->thread_id;
    public_thread->process_id        = thr_thread->process_id;
    public_thread->state             = (PsThreadState)thr_thread->state;
    public_thread->priority          = (PsThreadPriority)thr_thread->priority;
    public_thread->affinity_core     = thr_thread->affinity_core;
    public_thread->kernel_stack_base = thr_thread->kernel_stack_base;
    public_thread->kernel_stack_size = thr_thread->kernel_stack_size;
    public_thread->user_stack_base   = thr_thread->user_stack_base;
    public_thread->user_stack_size   = thr_thread->user_stack_size;
    public_thread->sleep_ticks       = thr_thread->sleep_ticks;

    return PsKitContextToPublic(&public_thread->context, &thr_thread->context);
}
