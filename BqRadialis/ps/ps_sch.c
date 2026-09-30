// Made by Berkay

#include "ps_private.h"

static PsSchLedger s_sch_ledger;
static CpContext   s_sch_next_contexts[PS_SCH_MAX_CORES] __attribute__((aligned(16)));

void PsSchLoad(void)
{
    UInt8* raw = (UInt8*)&s_sch_ledger;
    for (UInt64 i = 0; i < sizeof(PsSchLedger); i++)
    {
        raw[i] = 0;
    }

    for (UInt64 c = 0; c < (UInt64)PS_SCH_MAX_CORES; c++)
    {
        raw = (UInt8*)&s_sch_next_contexts[c];
        for (UInt64 i = 0; i < sizeof(CpContext); i++)
        {
            raw[i] = 0;
        }
    }
}

PsStatus PsSchInit(void)
{
    UInt32 core_count = 1;
    CpGetCoreCount(&core_count);

    if (core_count == 0)
    {
        core_count = 1;
    }

    if (core_count > PS_SCH_MAX_CORES)
    {
        core_count = PS_SCH_MAX_CORES;
    }

    s_sch_ledger.core_count = core_count;

    for (UInt32 i = 0; i < core_count; i++)
    {
        PsSchCore* core = &s_sch_ledger.cores[i];
        core->core_index      = i;
        core->remaining_ticks = PS_SCH_DEFAULT_TIME_SLICE;
        core->is_active       = 1;

        for (UInt32 p = 0; p < PS_SCH_PRIORITY_LEVELS; p++)
        {
            core->ready_queues[p].head  = 0;
            core->ready_queues[p].tail  = 0;
            core->ready_queues[p].count = 0;
        }

        UInt32 idle_id = 0;
        PsThrCreateIdle(&idle_id, i);

        core->idle_thread_id    = idle_id;
        core->current_thread_id = idle_id;

        CpSetCoreThreadContext(i, (void*)(UInt64)idle_id);
    }

    CpRegisterInterruptHandler(PS_SCH_TIMER_VECTOR, (CpInterruptHandler)PsSchSwitchContext);
    CpRegisterInterruptHandler(PS_SCH_YIELD_VECTOR, (CpInterruptHandler)PsSchSwitchContext);

    s_sch_ledger.is_initialized = 1;

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchStart(void)
{
    CpStartApicTimer(PS_SCH_TIMER_VECTOR, PS_SCH_TIMER_FREQUENCY);
    CpEnableInterrupts();
    CpTriggerYield();

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchEnqueue(UInt32 core_index, UInt32 thread_id, PsThrPriority priority)
{
    if (core_index >= s_sch_ledger.core_count)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    UInt32 prio = (UInt32)priority;
    if (prio >= PS_SCH_PRIORITY_LEVELS)
    {
        prio = PS_SCH_PRIORITY_LEVELS - 1;
    }

    PsKitSpinlockAcquire(&s_sch_ledger.ledger_lock);

    PsSchCore* core   = &s_sch_ledger.cores[core_index];
    PsSchQueue* queue = &core->ready_queues[prio];

    if (queue->count >= PS_SCH_QUEUE_CAPACITY)
    {
        PsKitSpinlockRelease(&s_sch_ledger.ledger_lock);
        return PS_STATUS_OUT_OF_RESOURCES;
    }

    queue->thread_ids[queue->tail] = thread_id;
    queue->tail                    = (queue->tail + 1) % PS_SCH_QUEUE_CAPACITY;
    queue->count++;

    PsKitSpinlockRelease(&s_sch_ledger.ledger_lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchDequeue(UInt32* thread_id, UInt32 core_index)
{
    if (thread_id == NULL || core_index >= s_sch_ledger.core_count)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsKitSpinlockAcquire(&s_sch_ledger.ledger_lock);

    PsSchCore* core = &s_sch_ledger.cores[core_index];

    for (Int32 p = PS_SCH_PRIORITY_LEVELS - 1; p >= 0; p--)
    {
        PsSchQueue* queue = &core->ready_queues[p];
        if (queue->count > 0)
        {
            *thread_id  = queue->thread_ids[queue->head];
            queue->head = (queue->head + 1) % PS_SCH_QUEUE_CAPACITY;
            queue->count--;

            PsKitSpinlockRelease(&s_sch_ledger.ledger_lock);
            return PS_STATUS_SUCCESS;
        }
    }

    PsKitSpinlockRelease(&s_sch_ledger.ledger_lock);

    return PS_STATUS_NOT_FOUND;
}

PsStatus PsSchPickNext(UInt32* next_thread_id, UInt32 core_index)
{
    if (next_thread_id == NULL || core_index >= s_sch_ledger.core_count)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsStatus status = PsSchDequeue(next_thread_id, core_index);
    if (status == PS_STATUS_SUCCESS)
    {
        return PS_STATUS_SUCCESS;
    }

    *next_thread_id = s_sch_ledger.cores[core_index].idle_thread_id;

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchSwitchContext(CpContext** next_cp_context, CpContext* current_cp_context)
{
    if (next_cp_context == NULL || current_cp_context == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    *next_cp_context = current_cp_context;

    UInt32 core_index = 0;
    CpGetCurrentCoreIndex(&core_index);

    if (core_index >= s_sch_ledger.core_count)
    {
        return PS_STATUS_SUCCESS;
    }

    PsSchCore* core = &s_sch_ledger.cores[core_index];

    if (current_cp_context->vector == PS_SCH_TIMER_VECTOR)
    {
        s_sch_ledger.total_ticks++;

        for (UInt32 i = 0; i < PS_THR_MAX_THREADS; i++)
        {
            PsThrThread* thr = NULL;
            if (PsThrGet(&thr, i) == PS_STATUS_SUCCESS && thr != NULL)
            {
                if (thr->state == PS_THR_STATE_SLEEPING)
                {
                    if (thr->sleep_ticks > 0)
                    {
                        thr->sleep_ticks--;
                    }

                    if (thr->sleep_ticks == 0)
                    {
                        thr->state = PS_THR_STATE_READY;
                        PsSchEnqueue(thr->affinity_core, thr->thread_id, thr->priority);
                    }
                }
            }
        }

        if (core->remaining_ticks > 0 && core->current_thread_id != core->idle_thread_id)
        {
            core->remaining_ticks--;
            if (core->remaining_ticks > 0)
            {
                return PS_STATUS_SUCCESS;
            }
        }
    }

    core->remaining_ticks = PS_SCH_DEFAULT_TIME_SLICE;

    UInt32 cur_id = core->current_thread_id;
    PsThrThread* cur_thr = NULL;

    if (PsThrGet(&cur_thr, cur_id) == PS_STATUS_SUCCESS && cur_thr != NULL)
    {
        if (cur_thr->state == PS_THR_STATE_RUNNING)
        {
            PsKitContextFromCp(&cur_thr->context, current_cp_context);
            CpSaveFpuState(cur_thr->context.fpu_state);

            if (cur_id != core->idle_thread_id)
            {
                cur_thr->state = PS_THR_STATE_READY;
                PsSchEnqueue(core_index, cur_id, cur_thr->priority);
            }
        }
    }

    UInt32 next_id = 0;
    PsSchPickNext(&next_id, core_index);

    if (next_id == cur_id && cur_thr != NULL && cur_thr->state == PS_THR_STATE_RUNNING)
    {
        return PS_STATUS_SUCCESS;
    }

    PsThrThread* next_thr = NULL;
    if (PsThrGet(&next_thr, next_id) == PS_STATUS_SUCCESS && next_thr != NULL)
    {
        next_thr->state         = PS_THR_STATE_RUNNING;
        core->current_thread_id = next_id;

        CpSetCoreThreadContext(core_index, (void*)(UInt64)next_id);

        PsPrcProcess* prc = NULL;
        if (PsPrcGet(&prc, next_thr->process_id) == PS_STATUS_SUCCESS && prc != NULL)
        {
            if (prc->page_table_address != 0)
            {
                next_thr->context.cr3 = prc->page_table_address;
            }
        }

        CpSetKernelStack(core_index, next_thr->kernel_stack_base + next_thr->kernel_stack_size);
        next_thr->context.kernel_stack_top = next_thr->kernel_stack_base + next_thr->kernel_stack_size;

        CpContext* next_cp = &s_sch_next_contexts[core_index];
        PsKitContextToCp(next_cp, &next_thr->context);
        *next_cp_context = next_cp;
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchTick(void)
{
    UInt32 core_index = 0;
    CpGetCurrentCoreIndex(&core_index);

    if (core_index < s_sch_ledger.core_count)
    {
        PsSchCore* core = &s_sch_ledger.cores[core_index];
        if (core->remaining_ticks > 0)
        {
            core->remaining_ticks--;
        }

        if (core->remaining_ticks == 0)
        {
            CpTriggerYield();
        }
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchYield(void)
{
    UInt32 core_index = 0;
    CpGetCurrentCoreIndex(&core_index);

    if (core_index < s_sch_ledger.core_count)
    {
        s_sch_ledger.cores[core_index].remaining_ticks = 0;
    }

    CpTriggerYield();

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchSleep(UInt64 ticks)
{
    UInt32 thread_id = 0;
    PsThrGetCurrentId(&thread_id);

    PsThrThread* thread = NULL;
    if (PsThrGet(&thread, thread_id) == PS_STATUS_SUCCESS && thread != NULL)
    {
        thread->sleep_ticks = ticks;
        thread->state       = PS_THR_STATE_SLEEPING;
        PsSchYield();
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchBlock(UInt32 thread_id)
{
    PsThrThread* thread = NULL;
    PsStatus status = PsThrGet(&thread, thread_id);
    if (status != PS_STATUS_SUCCESS || thread == NULL)
    {
        return status;
    }

    thread->state = PS_THR_STATE_BLOCKED;

    UInt32 current_id = 0;
    PsThrGetCurrentId(&current_id);

    if (current_id == thread_id)
    {
        PsSchYield();
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchUnblock(UInt32 thread_id)
{
    PsThrThread* thread = NULL;
    PsStatus status = PsThrGet(&thread, thread_id);
    if (status != PS_STATUS_SUCCESS || thread == NULL)
    {
        return status;
    }

    if (thread->state == PS_THR_STATE_BLOCKED)
    {
        thread->state = PS_THR_STATE_READY;
        PsSchEnqueue(thread->affinity_core, thread_id, thread->priority);
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsSchIdleEntry(void* argument)
{
    (void)argument;

    for (;;)
    {
        __asm__ volatile ("hlt");
    }

    return PS_STATUS_SUCCESS;
}
