// Made by Berkay

#include "ps_private.h"

static PsPrcLedger s_prc_ledger;

PsStatus PsPrcInit(void)
{
    UInt8* raw = (UInt8*)&s_prc_ledger;
    for (UInt64 i = 0; i < sizeof(PsPrcLedger); i++)
    {
        raw[i] = 0;
    }

    UInt64 cr3 = 0;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(cr3));

    s_prc_ledger.processes[0].process_id         = 0;
    s_prc_ledger.processes[0].parent_process_id  = 0;
    s_prc_ledger.processes[0].state              = PS_PRC_STATE_RUNNING;
    s_prc_ledger.processes[0].priority           = PS_PRC_PRIORITY_NORMAL;
    s_prc_ledger.processes[0].privilege          = PS_PRC_PRIVILEGE_KERNEL;
    s_prc_ledger.processes[0].page_table_address = cr3;
    s_prc_ledger.processes[0].thread_count       = 0;
    s_prc_ledger.processes[0].is_allocated       = 1;

    s_prc_ledger.process_count  = 1;
    s_prc_ledger.is_initialized = 1;

    return PS_STATUS_SUCCESS;
}

PsStatus PsPrcCreate(UInt32* process_id, PsPrcPrivilege privilege, PsPrcPriority priority)
{
    if (process_id == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsKitSpinlockAcquire(&s_prc_ledger.ledger_lock);

    UInt32 slot = 0;
    for (UInt32 i = 1; i < PS_PRC_MAX_PROCESSES; i++)
    {
        if (s_prc_ledger.processes[i].is_allocated == 0)
        {
            slot = i;
            break;
        }
    }

    if (slot == 0)
    {
        PsKitSpinlockRelease(&s_prc_ledger.ledger_lock);
        return PS_STATUS_OUT_OF_RESOURCES;
    }

    UInt64 page_table = 0;
    if (privilege == PS_PRC_PRIVILEGE_USER)
    {
        MmStatus mm_status = MmCreateAddressSpace(&page_table);
        if (mm_status != MM_STATUS_SUCCESS)
        {
            PsKitSpinlockRelease(&s_prc_ledger.ledger_lock);
            return PS_STATUS_OUT_OF_MEMORY;
        }
    }
    else
    {
        page_table = s_prc_ledger.processes[0].page_table_address;
    }

    UInt32 parent_id = 0;
    PsPrcGetCurrentId(&parent_id);

    s_prc_ledger.processes[slot].process_id         = slot;
    s_prc_ledger.processes[slot].parent_process_id  = parent_id;
    s_prc_ledger.processes[slot].state              = PS_PRC_STATE_READY;
    s_prc_ledger.processes[slot].priority           = priority;
    s_prc_ledger.processes[slot].privilege          = privilege;
    s_prc_ledger.processes[slot].page_table_address = page_table;
    s_prc_ledger.processes[slot].thread_count       = 0;
    s_prc_ledger.processes[slot].is_allocated       = 1;

    s_prc_ledger.process_count++;

    *process_id = slot;

    PsKitSpinlockRelease(&s_prc_ledger.ledger_lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsPrcDestroy(UInt32 process_id)
{
    if (process_id == 0 || process_id >= PS_PRC_MAX_PROCESSES)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    PsKitSpinlockAcquire(&s_prc_ledger.ledger_lock);

    PsPrcProcess* process = &s_prc_ledger.processes[process_id];
    if (process->is_allocated == 0)
    {
        PsKitSpinlockRelease(&s_prc_ledger.ledger_lock);
        return PS_STATUS_NOT_FOUND;
    }

    if (process->privilege == PS_PRC_PRIVILEGE_USER && process->page_table_address != 0)
    {
        MmDestroyAddressSpace(process->page_table_address);
    }

    process->state              = PS_PRC_STATE_TERMINATED;
    process->page_table_address = 0;
    process->is_allocated       = 0;

    s_prc_ledger.process_count--;

    PsKitSpinlockRelease(&s_prc_ledger.ledger_lock);

    return PS_STATUS_SUCCESS;
}

PsStatus PsPrcGet(PsPrcProcess** process, UInt32 process_id)
{
    if (process == NULL || process_id >= PS_PRC_MAX_PROCESSES)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (s_prc_ledger.processes[process_id].is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    *process = &s_prc_ledger.processes[process_id];

    return PS_STATUS_SUCCESS;
}

PsStatus PsPrcGetPublic(PsProcess* process, UInt32 process_id)
{
    if (process == NULL || process_id >= PS_PRC_MAX_PROCESSES)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (s_prc_ledger.processes[process_id].is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    return PsKitProcessToPublic(process, &s_prc_ledger.processes[process_id]);
}

PsStatus PsPrcGetCurrentId(UInt32* process_id)
{
    if (process_id == NULL)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    UInt32 thread_id = 0;
    PsStatus status = PsThrGetCurrentId(&thread_id);

    PsThrThread* thread = NULL;
    if (status == PS_STATUS_SUCCESS && PsThrGet(&thread, thread_id) == PS_STATUS_SUCCESS && thread != NULL)
    {
        *process_id = thread->process_id;
    }
    else
    {
        *process_id = 0;
    }

    return PS_STATUS_SUCCESS;
}

PsStatus PsPrcIncrementThreadCount(UInt32 process_id)
{
    if (process_id >= PS_PRC_MAX_PROCESSES)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (s_prc_ledger.processes[process_id].is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    s_prc_ledger.processes[process_id].thread_count++;

    return PS_STATUS_SUCCESS;
}

PsStatus PsPrcDecrementThreadCount(UInt32 process_id)
{
    if (process_id >= PS_PRC_MAX_PROCESSES)
    {
        return PS_STATUS_INVALID_PARAMETER;
    }

    if (s_prc_ledger.processes[process_id].is_allocated == 0)
    {
        return PS_STATUS_NOT_FOUND;
    }

    if (s_prc_ledger.processes[process_id].thread_count > 0)
    {
        s_prc_ledger.processes[process_id].thread_count--;
    }

    return PS_STATUS_SUCCESS;
}
