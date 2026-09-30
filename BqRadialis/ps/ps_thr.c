// Made by Berkay

#include "ps_private.h"

static PsThrLedger s_thr_ledger;

PsStatus PsThrInit(void)
{
    UInt8* raw = (UInt8*)&s_thr_ledger;
    for (UInt64 i = 0; i < sizeof(PsThrLedger); i++)
    {
        raw[i] = 0;
    }

    s_thr_ledger.is_initialized = 1;

    return PS_STATUS_SUCCESS;
}

PsStatus PsThrCreate(UInt32* thread_id, UInt32 process_id, PsThreadEntry entry, void* argument, PsThrPriority priority)
{
    if (thread_id == NULL || entry == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsPrcProcess* process = NULL;
    PsStatus prc_status = PsPrcGet(&process, process_id);
    if (prc_status != PS_STATUS_SUCCESS)
    {
        return prc_status;
    }

    PsKitSpinlockAcquire(&s_thr_ledger.ledger_lock);

    UInt32 slot = 0;
    for (UInt32 i = 1; i < PS_THR_MAX_THREADS; i++)
    {
        if (s_thr_ledger.threads[i].is_allocated == 0)
        {
            slot = i;
            break;
        }
    }

    if (slot == 0)
    {
        PsKitSpinlockRelease(&s_thr_ledger.ledger_lock);
        return PS_STATUS_OUT_OF_RESOURCES;
    }

    UInt64 kernel_stack_size = PS_THR_DEFAULT_STACK_SIZE;
    void* kernel_stack = NULL;
    MmStatus mm_status = MmAllocateHeapBlock(&kernel_stack, kernel_stack_size);
    if (mm_status != MM_STATUS_SUCCESS)
    {
        PsKitSpinlockRelease(&s_thr_ledger.ledger_lock);
        return PS_STATUS_OUT_OF_MEMORY;
    }

    UInt64 kernel_stack_top = ((UInt64)kernel_stack + kernel_stack_size) & ~0xFULL;

    UInt64 user_stack_base = 0;
    UInt64 user_stack_size = 0;
    UInt64 user_stack_top  = 0;

    if (process->privilege == PS_PRC_PRIVILEGE_USER)
    {
        void* user_stack = NULL;
        mm_status = MmAllocateHeapBlock(&user_stack, kernel_stack_size);
        if (mm_status != MM_STATUS_SUCCESS)
        {
            MmReleaseHeapBlock(kernel_stack);
            PsKitSpinlockRelease(&s_thr_ledger.ledger_lock);
            return PS_STATUS_OUT_OF_MEMORY;
        }

        user_stack_base = (UInt64)user_stack;
        user_stack_size = kernel_stack_size;
        user_stack_top  = ((user_stack_base + user_stack_size) & ~0xFULL) - 8ULL;
    }

    UInt32 core_index = 0;
    PsKitSelectCore(&core_index);

    CpContext cp_context;
    CpContextMode mode = (process->privilege == PS_PRC_PRIVILEGE_USER) ? CP_CONTEXT_MODE_USER : CP_CONTEXT_MODE_KERNEL;
    UInt64 stack_top   = (mode == CP_CONTEXT_MODE_USER) ? user_stack_top : kernel_stack_top;

    CpBuildContext(&cp_context, mode, (UInt64)PsThrBootstrap, stack_top, process->page_table_address, (UInt64)slot);
    cp_context.kernel_stack_top = kernel_stack_top;

    PsKitContextFromCp(&s_thr_ledger.threads[slot].context, &cp_context);
    s_thr_ledger.threads[slot].context.kernel_stack_top = kernel_stack_top;

    for (UInt64 i = 0; i < 512; i++)
    {
        s_thr_ledger.threads[slot].context.fpu_state[i] = cp_context.fpu_state[i];
    }

    s_thr_ledger.threads[slot].thread_id         = slot;
    s_thr_ledger.threads[slot].process_id        = process_id;
    s_thr_ledger.threads[slot].state             = PS_THR_STATE_READY;
    s_thr_ledger.threads[slot].priority          = priority;
    s_thr_ledger.threads[slot].affinity_core     = core_index;
    s_thr_ledger.threads[slot].kernel_stack_base = (UInt64)kernel_stack;
    s_thr_ledger.threads[slot].kernel_stack_size = kernel_stack_size;
    s_thr_ledger.threads[slot].user_stack_base   = user_stack_base;
    s_thr_ledger.threads[slot].user_stack_size   = user_stack_size;
    s_thr_ledger.threads[slot].sleep_ticks       = 0;
    s_thr_ledger.threads[slot].entry_point       = (void*)entry;
    s_thr_ledger.threads[slot].argument          = argument;
    s_thr_ledger.threads[slot].is_allocated      = 1;

    s_thr_ledger.thread_count++;

    PsPrcIncrementThreadCount(process_id);

    PsSchEnqueue(core_index, slot, priority);

    *thread_id = slot;

    PsKitSpinlockRelease(&s_thr_ledger.ledger_lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsThrCreateIdle(UInt32* thread_id, UInt32 core_index)
{
    if (thread_id == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsKitSpinlockAcquire(&s_thr_ledger.ledger_lock);

    UInt32 slot = 0;
    for (UInt32 i = 0; i < PS_THR_MAX_THREADS; i++)
    {
        if (s_thr_ledger.threads[i].is_allocated == 0)
        {
            slot = i;
            break;
        }
    }

    UInt64 kernel_stack_size = PS_THR_DEFAULT_STACK_SIZE;
    void* kernel_stack = NULL;
    MmStatus mm_status = MmAllocateHeapBlock(&kernel_stack, kernel_stack_size);
    if (mm_status != MM_STATUS_SUCCESS)
    {
        PsKitSpinlockRelease(&s_thr_ledger.ledger_lock);
        return PS_STATUS_OUT_OF_MEMORY;
    }

    UInt64 kernel_stack_top = ((UInt64)kernel_stack + kernel_stack_size) & ~0xFULL;

    CpContext cp_context;
    CpBuildContext(&cp_context, CP_CONTEXT_MODE_KERNEL, (UInt64)PsSchIdleEntry, kernel_stack_top, 0, 0);
    cp_context.kernel_stack_top = kernel_stack_top;

    PsKitContextFromCp(&s_thr_ledger.threads[slot].context, &cp_context);
    s_thr_ledger.threads[slot].context.kernel_stack_top = kernel_stack_top;

    for (UInt64 i = 0; i < 512; i++)
    {
        s_thr_ledger.threads[slot].context.fpu_state[i] = cp_context.fpu_state[i];
    }

    s_thr_ledger.threads[slot].thread_id         = slot;
    s_thr_ledger.threads[slot].process_id        = 0;
    s_thr_ledger.threads[slot].state             = PS_THR_STATE_READY;
    s_thr_ledger.threads[slot].priority          = PS_THR_PRIORITY_IDLE;
    s_thr_ledger.threads[slot].affinity_core     = core_index;
    s_thr_ledger.threads[slot].kernel_stack_base = (UInt64)kernel_stack;
    s_thr_ledger.threads[slot].kernel_stack_size = kernel_stack_size;
    s_thr_ledger.threads[slot].user_stack_base   = 0;
    s_thr_ledger.threads[slot].user_stack_size   = 0;
    s_thr_ledger.threads[slot].sleep_ticks       = 0;
    s_thr_ledger.threads[slot].entry_point       = (void*)PsSchIdleEntry;
    s_thr_ledger.threads[slot].argument          = NULL;
    s_thr_ledger.threads[slot].is_allocated      = 1;

    s_thr_ledger.thread_count++;

    *thread_id = slot;

    PsKitSpinlockRelease(&s_thr_ledger.ledger_lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsThrDestroy(UInt32 thread_id)
{
    if (thread_id >= PS_THR_MAX_THREADS)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsKitSpinlockAcquire(&s_thr_ledger.ledger_lock);

    PsThrThread* thread = &s_thr_ledger.threads[thread_id];
    if (thread->is_allocated == 0)
    {
        PsKitSpinlockRelease(&s_thr_ledger.ledger_lock);
        return PS_STATUS_NOT_FOUND;
    }

    if (thread->kernel_stack_base != 0)
    {
        MmReleaseHeapBlock((void*)thread->kernel_stack_base);
    }

    if (thread->user_stack_base != 0)
    {
        MmReleaseHeapBlock((void*)thread->user_stack_base);
    }

    thread->state        = PS_THR_STATE_TERMINATED;
    thread->is_allocated = 0;

    s_thr_ledger.thread_count--;

    PsPrcDecrementThreadCount(thread->process_id);

    PsKitSpinlockRelease(&s_thr_ledger.ledger_lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsThrGet(PsThrThread** thread, UInt32 thread_id)
{
    if (thread == NULL || thread_id >= PS_THR_MAX_THREADS)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (s_thr_ledger.threads[thread_id].is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    *thread = &s_thr_ledger.threads[thread_id];

    return PS_STATUS_SUCCESS;
}

PsStatus PsThrGetPublic(PsThread* thread, UInt32 thread_id)
{
    if (thread == NULL || thread_id >= PS_THR_MAX_THREADS)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (s_thr_ledger.threads[thread_id].is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    return PsKitThreadToPublic(thread, &s_thr_ledger.threads[thread_id]);
}

PsStatus PsThrGetCurrentId(UInt32* thread_id)
{
    if (thread_id == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    UInt32 core_index = 0;
    CpGetCurrentCoreIndex(&core_index);

    void* ctx = NULL;
    CpGetCoreThreadContext(&ctx, core_index);

    *thread_id = (UInt32)(UInt64)ctx;

    return PS_STATUS_SUCCESS;
}

PsStatus PsThrGetContext(PsContext* context, UInt32 thread_id)
{
    if (context == NULL || thread_id >= PS_THR_MAX_THREADS)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (s_thr_ledger.threads[thread_id].is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    return PsKitContextToPublic(context, &s_thr_ledger.threads[thread_id].context);
}

PsStatus PsThrSetContext(UInt32 thread_id, PsContext* context)
{
    if (context == NULL || thread_id >= PS_THR_MAX_THREADS)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (s_thr_ledger.threads[thread_id].is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    return PsKitContextFromPublic(&s_thr_ledger.threads[thread_id].context, context);
}

PsStatus PsThrBootstrap(void)
{
    UInt32 thread_id = 0;
    PsThrGetCurrentId(&thread_id);

    PsThrThread* thread = NULL;
    if (PsThrGet(&thread, thread_id) == PS_STATUS_SUCCESS && thread != NULL)
    {
        if (thread->entry_point != NULL)
        {
            PsThreadEntry entry = (PsThreadEntry)thread->entry_point;
            entry(thread->argument);
        }
    }

    PsThreadDestroy(thread_id);
    PsSchYield();

    for (;;)
    {
        CpPause();
    }

    return PS_STATUS_SUCCESS;
}
