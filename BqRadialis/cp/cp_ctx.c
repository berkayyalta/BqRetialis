// Made by Berkay

#include "cp_private.h"

static CpCtxLedger s_ctx_ledger;

CpStatus CpCtxBuild(CpContext* context, CpContextMode mode, UInt64 entry_address, UInt64 stack_top_address, UInt64 page_table_address, UInt64 argument)
{
    if (context == NULL || entry_address == 0 || stack_top_address == 0)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    UInt8* raw_context = (UInt8*)context;

    for (UInt64 i = 0; i < sizeof(CpContext); i++)
    {
        raw_context[i] = 0;
    }

    if (page_table_address == 0)
    {
        __asm__ volatile ("mov %%cr3, %0" : "=r" (page_table_address));
    }

    if (mode == CP_CONTEXT_MODE_KERNEL)
    {
        context->cs               = CP_GDT_SELECTOR_KERNEL_CODE;
        context->ss               = CP_GDT_SELECTOR_KERNEL_DATA;
        context->kernel_stack_top = stack_top_address;
    }
    else if (mode == CP_CONTEXT_MODE_USER)
    {
        context->cs               = CP_GDT_SELECTOR_USER_CODE | 0x03;
        context->ss               = CP_GDT_SELECTOR_USER_DATA | 0x03;
        context->kernel_stack_top = 0;
    }
    else
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    context->cr3    = page_table_address;
    context->rdi    = argument;
    context->rip    = entry_address;
    context->rflags = CP_CTX_DEFAULT_RFLAGS;
    context->rsp    = stack_top_address;

    *((UInt16*)&context->fpu_state[0x00]) = 0x037F;
    *((UInt32*)&context->fpu_state[0x18]) = 0x1F80;

    return CP_STATUS_SUCCESS;
}

CpStatus CpCtxGetActive(CpContext* context, UInt32 core_index)
{
    if (context == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    if (core_index >= CP_CTX_MAX_CORES)
    {
        return CP_STATUS_OUT_OF_BOUNDS;
    }

    volatile UInt8* destination = (volatile UInt8*)context;
    volatile UInt8* source      = (volatile UInt8*)&s_ctx_ledger.active_contexts[core_index];

    for (UInt64 i = 0; i < sizeof(CpContext); i++)
    {
        destination[i] = source[i];
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpCtxSetNext(UInt32 core_index, CpContext* context)
{
    if (context == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    if (core_index >= CP_CTX_MAX_CORES)
    {
        return CP_STATUS_OUT_OF_BOUNDS;
    }

    s_ctx_ledger.next_contexts[core_index] = (CpCtxContext*)context;

    return CP_STATUS_SUCCESS;
}

CpStatus CpCtxSwitch(CpContext* context)
{
    if (context == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    UInt32 core_index = 0;

    CpStatus status = CpSmpGetCurrentCoreIndex(&core_index);

    if (status == CP_STATUS_SUCCESS && core_index < CP_CTX_MAX_CORES)
    {
        if (s_ctx_ledger.current_contexts[core_index] != NULL &&
            s_ctx_ledger.current_contexts[core_index] != (CpCtxContext*)context)
        {
            CpFpuSave(s_ctx_ledger.current_contexts[core_index]->fpu_state);
        }

        volatile UInt8* destination = (volatile UInt8*)&s_ctx_ledger.active_contexts[core_index];
        volatile UInt8* source      = (volatile UInt8*)context;

        for (UInt64 i = 0; i < sizeof(CpCtxContext); i++)
        {
            destination[i] = source[i];
        }

        s_ctx_ledger.current_contexts[core_index] = (CpCtxContext*)context;
        s_ctx_ledger.next_contexts[core_index]    = NULL;

        UInt64 kernel_stack = context->kernel_stack_top;

        if (kernel_stack == 0)
        {
            kernel_stack = CpTssGetDefaultKernelStack(core_index);
        }

        CpTssSetKernelStack(core_index, kernel_stack);
        CpSmpSetKernelStack(core_index, kernel_stack);
    }

    CpFpuRestore(context->fpu_state);

    CpCtxLoad((CpCtxContext*)context);

    return CP_STATUS_SUCCESS;
}

CpStatus CpCtxSetPageTable(UInt64 page_table_address)
{
    if (page_table_address == 0)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    __asm__ volatile ("mov %0, %%cr3" : : "r"(page_table_address) : "memory");

    return CP_STATUS_SUCCESS;
}

CpStatus CpCtxShootdownTlb(UInt64 virtual_address, UInt64 page_count)
{
    if (page_count == 0)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    for (UInt64 i = 0; i < page_count; i++)
    {
        UInt64 addr = virtual_address + (i * 4096ULL);
        __asm__ volatile ("invlpg (%0)" : : "r"(addr) : "memory");
    }

    UInt32 online_count = 0;
    if (CpSmpGetOnlineCount(&online_count) != CP_STATUS_SUCCESS || online_count <= 1)
    {
        return CP_STATUS_SUCCESS;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_ctx_ledger.tlb_shootdown_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    s_ctx_ledger.tlb_shootdown_address = virtual_address;
    s_ctx_ledger.tlb_shootdown_pages   = page_count;
    s_ctx_ledger.tlb_shootdown_acks    = 0;

    UInt32 target_acks = online_count - 1U;

    CpStatus status = CpKitBroadcastIpi(CP_CTX_TLB_SHOOTDOWN_VECTOR, CP_APIC_ICR_DELIVERY_FIXED, 0);

    if (status == CP_STATUS_SUCCESS)
    {
        for (UInt32 spin = 0; spin < 1000000U; spin++)
        {
            if (s_ctx_ledger.tlb_shootdown_acks >= target_acks)
            {
                break;
            }
            __asm__ volatile ("pause");
        }
    }

    __sync_lock_release(&s_ctx_ledger.tlb_shootdown_lock);

    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return status;
}

CpStatus CpCtxDispatch(CpCtxContext** next_context, CpCtxContext* current_context)
{
    if (next_context == NULL || current_context == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    *next_context = current_context;

    if (current_context->vector == CP_CTX_TLB_SHOOTDOWN_VECTOR)
    {
        UInt64 base_addr  = s_ctx_ledger.tlb_shootdown_address;
        UInt64 page_count = s_ctx_ledger.tlb_shootdown_pages;

        for (UInt64 i = 0; i < page_count; i++)
        {
            UInt64 addr = base_addr + (i * 4096ULL);
            __asm__ volatile ("invlpg (%0)" : : "r"(addr) : "memory");
        }

        __asm__ volatile ("lock incl %0" : "+m"(s_ctx_ledger.tlb_shootdown_acks) : : "memory");
        CpApicSendEoi();
        return CP_STATUS_SUCCESS;
    }

    if (current_context->vector == CP_CTX_PAGE_TABLE_SYNC_VECTOR)
    {
        UInt64 new_pml4 = s_ctx_ledger.page_table_sync_address;

        current_context->cr3 = new_pml4;
        __asm__ volatile ("mov %0, %%cr3" : : "r"(new_pml4) : "memory");
        __asm__ volatile ("lock incl %0" : "+m"(s_ctx_ledger.page_table_sync_acks) : : "memory");
        CpApicSendEoi();
        return CP_STATUS_SUCCESS;
    }

    if (current_context->vector >= 32 && current_context->vector != CP_APIC_SPURIOUS_VECTOR)
    {
        CpApicSendEoi();
    }

    UInt32 core_index = 0;

    CpStatus status = CpSmpGetCurrentCoreIndex(&core_index);

    if (status != CP_STATUS_SUCCESS || core_index >= CP_CTX_MAX_CORES)
    {
        if (current_context->vector < 32)
        {
            BkPanicReport report =
            {
                .message  = "Unhandled CPU exception on unindexed core",
                .file     = __FILE__,
                .function = __func__,
                .line     = (UInt32)current_context->rip
            };
            BkPanic(&report);
        }

        return CP_STATUS_SUCCESS;
    }

    volatile UInt8* active_bytes  = (volatile UInt8*)&s_ctx_ledger.active_contexts[core_index];
    volatile UInt8* current_bytes = (volatile UInt8*)current_context;

    for (UInt64 i = 0; i < CP_CTX_HW_FRAME_SIZE; i++)
    {
        active_bytes[i] = current_bytes[i];
    }

    if (current_context->vector < 32)
    {
        UInt64 cr2_val = 0;
        __asm__ volatile ("mov %%cr2, %0" : "=r"(cr2_val));
        s_ctx_ledger.fault_cr2[core_index] = cr2_val;

        BkPanicReport report =
        {
            .message  = (current_context->vector == 14) ? "CPU Page Fault (#PF)" :
                        (current_context->vector == 13) ? "CPU General Protection Fault (#GP)" :
                        (current_context->vector == 8)  ? "CPU Double Fault (#DF)" :
                        (current_context->vector == 6)  ? "CPU Invalid Opcode (#UD)" :
                                                          "Unhandled CPU Exception",
            .file     = __FILE__,
            .function = __func__,
            .line     = (UInt32)current_context->rip
        };
        BkPanic(&report);
    }

    if (s_ctx_ledger.current_contexts[core_index] != NULL)
    {
        volatile UInt8* saved_bytes = (volatile UInt8*)s_ctx_ledger.current_contexts[core_index];

        for (UInt64 i = 0; i < CP_CTX_HW_FRAME_SIZE; i++)
        {
            saved_bytes[i] = current_bytes[i];
        }
    }

    CpContext* handler_next = (CpContext*)s_ctx_ledger.next_contexts[core_index];

    if (handler_next == NULL)
    {
        handler_next = (CpContext*)current_context;
    }

    if (s_ctx_ledger.interrupt_handlers[current_context->vector] != NULL)
    {
        s_ctx_ledger.interrupt_handlers[current_context->vector](&handler_next, (CpContext*)current_context);
    }

    if (handler_next != (CpContext*)current_context && handler_next != NULL)
    {
        s_ctx_ledger.next_contexts[core_index] = (CpCtxContext*)handler_next;
    }

    if (s_ctx_ledger.next_contexts[core_index] != NULL)
    {
        CpCtxContext* incoming_context = s_ctx_ledger.next_contexts[core_index];

        volatile UInt8* next_bytes = (volatile UInt8*)incoming_context;

        for (UInt64 i = 0; i < sizeof(CpCtxContext); i++)
        {
            active_bytes[i] = next_bytes[i];
        }

        s_ctx_ledger.current_contexts[core_index] = incoming_context;
        s_ctx_ledger.next_contexts[core_index]    = NULL;

        UInt64 kernel_stack = incoming_context->kernel_stack_top;

        if (kernel_stack == 0)
        {
            kernel_stack = CpTssGetDefaultKernelStack(core_index);
        }

        CpTssSetKernelStack(core_index, kernel_stack);
        CpSmpSetKernelStack(core_index, kernel_stack);
        CpFpuRestore(incoming_context->fpu_state);

        *next_context = &s_ctx_ledger.active_contexts[core_index];
    }

    return CP_STATUS_SUCCESS;
}

CpStatus CpCtxRegisterInterruptHandler(UInt8 vector, CpInterruptHandler handler)
{
    if (handler == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    s_ctx_ledger.interrupt_handlers[vector] = handler;

    return CP_STATUS_SUCCESS;
}

CpStatus CpCtxUnregisterInterruptHandler(UInt8 vector)
{
    s_ctx_ledger.interrupt_handlers[vector] = NULL;

    return CP_STATUS_SUCCESS;
}

CpStatus CpCtxInitSyscall(CpSyscallHandler handler)
{
    if (handler == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    s_ctx_ledger.syscall_handler = handler;

    return CP_STATUS_SUCCESS;
}

CpStatus CpSyscallDispatch(CpContext* context)
{
    if (context == NULL)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    if (s_ctx_ledger.syscall_handler == NULL)
    {
        return CP_STATUS_NOT_INITIALIZED;
    }

    return s_ctx_ledger.syscall_handler(context);
}

CpStatus CpCtxInit(void)
{
    UInt8* raw_ledger = (UInt8*)&s_ctx_ledger;

    for (UInt64 i = 0; i < sizeof(CpCtxLedger); i++)
    {
        raw_ledger[i] = 0;
    }

    return CP_STATUS_SUCCESS;
}
