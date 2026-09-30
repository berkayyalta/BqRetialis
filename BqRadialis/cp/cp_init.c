// Made by Berkay

#include "cp_private.h"

void CpLoad(void)
{
    CpIdtLoad(0);

    CpFpuEnable(0);

    CpGdtLoad(0);

    CpTssLoad(0);
}

CpStatus CpInit(void)
{
    CpStatus status = CpApicInit();

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    status = CpSmpInit();

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    status = CpFpuInit();

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    status = CpCtxInit();

    if (status != CP_STATUS_SUCCESS)
    {
        return status;
    }

    return CP_STATUS_SUCCESS;
}

void CpSetKernelStack(UInt32 core_index, UInt64 stack_top_address)
{
    CpTssSetKernelStack(core_index, stack_top_address);
    CpSmpSetKernelStack(core_index, stack_top_address);
}

void CpSetTssEntry(UInt32 core_index, UInt32 entry_index, UInt64 stack_top_address)
{
    CpTssSetEntry(core_index, entry_index, stack_top_address);
}

void CpSetIdtEntry(UInt32 entry_index, UInt64 handler_address, UInt16 code_segment_selector, UInt8 type_attributes, UInt8 interrupt_stack_table_index)
{
    CpIdtSetEntry(entry_index, handler_address, code_segment_selector, type_attributes, interrupt_stack_table_index);
}

void CpEnableInterrupts(void)
{
    __asm__ volatile ("sti");
}

void CpDisableInterrupts(void)
{
    __asm__ volatile ("cli");
}

void CpSaveAndDisableInterrupts(UInt64* interrupt_state)
{
    if (interrupt_state != 0)
    {
        __asm__ volatile ("pushfq; popq %0; cli" : "=r"(*interrupt_state) : : "memory");
    }
}

void CpRestoreInterrupts(UInt64 interrupt_state)
{
    if ((interrupt_state & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }
}

void CpHalt(void)
{
    __asm__ volatile ("hlt");
}

void CpPause(void)
{
    __asm__ volatile ("pause");
}

CpStatus CpRegisterInterruptHandler(UInt8 vector, CpInterruptHandler handler)
{
    return CpCtxRegisterInterruptHandler(vector, handler);
}

CpStatus CpUnregisterInterruptHandler(UInt8 vector)
{
    return CpCtxUnregisterInterruptHandler(vector);
}

CpStatus CpTriggerYield(void)
{
    __asm__ volatile ("int $0xFC");
    return CP_STATUS_SUCCESS;
}

CpStatus CpGetFaultAddress(UInt64* fault_address)
{
    if (fault_address != 0)
    {
        __asm__ volatile ("mov %%cr2, %0" : "=r"(*fault_address));
    }
    return CP_STATUS_SUCCESS;
}

CpStatus CpInitSyscall(CpSyscallHandler handler)
{
    return CpCtxInitSyscall(handler);
}

CpStatus CpGetApicId(UInt8* apic_id)
{
    return CpApicGetId(apic_id);
}

CpStatus CpSendApicEoi(void)
{
    return CpApicSendEoi();
}

CpStatus CpSendIpi(UInt8 destination_apic_id, UInt8 vector, UInt32 flags)
{
    return CpApicSendIpi(destination_apic_id, vector, flags);
}

CpStatus CpDelayApicTimer(UInt32 milliseconds)
{
    return CpApicDelay(milliseconds);
}

CpStatus CpStartApicTimer(UInt8 vector, UInt32 frequency)
{
    return CpApicStartTimer(vector, frequency);
}

CpStatus CpStopApicTimer(void)
{
    return CpApicStopTimer();
}

CpStatus CpGetCoreCount(UInt32* core_count)
{
    return CpSmpGetCoreCount(core_count);
}

CpStatus CpGetOnlineCoreCount(UInt32* online_count)
{
    return CpSmpGetOnlineCount(online_count);
}

CpStatus CpGetCurrentCoreIndex(UInt32* core_index)
{
    return CpSmpGetCurrentCoreIndex(core_index);
}

CpStatus CpSaveFpuState(void* fpu_buffer)
{
    return CpFpuSave(fpu_buffer);
}

CpStatus CpRestoreFpuState(void* fpu_buffer)
{
    return CpFpuRestore(fpu_buffer);
}

CpStatus CpGetFpuStateSize(UInt64* size, UInt64* alignment)
{
    return CpKitGetFpuStateSize(size, alignment);
}

CpStatus CpBuildContext(CpContext* context, CpContextMode mode, UInt64 entry_address, UInt64 stack_top_address, UInt64 page_table_address, UInt64 argument)
{
    return CpCtxBuild(context, mode, entry_address, stack_top_address, page_table_address, argument);
}

CpStatus CpGetActiveContext(CpContext* context, UInt32 core_index)
{
    return CpCtxGetActive(context, core_index);
}

CpStatus CpSetNextContext(UInt32 core_index, CpContext* context)
{
    return CpCtxSetNext(core_index, context);
}

CpStatus CpSwitchContext(CpContext* context)
{
    return CpCtxSwitch(context);
}

CpStatus CpSetPageTable(UInt64 page_table_address)
{
    return CpCtxSetPageTable(page_table_address);
}

CpStatus CpShootdownTlb(UInt64 virtual_address, UInt64 page_count)
{
    return CpCtxShootdownTlb(virtual_address, page_count);
}

CpStatus CpBroadcastIpi(UInt8 vector, UInt32 flags, UInt8 include_self)
{
    return CpKitBroadcastIpi(vector, flags, include_self);
}

CpStatus CpGetCoreApicId(UInt8* apic_id, UInt32 core_index)
{
    return CpKitGetCoreApicId(apic_id, core_index);
}

CpStatus CpSetCoreThreadContext(UInt32 core_index, void* thread_context)
{
    return CpKitSetCoreThreadContext(core_index, thread_context);
}

CpStatus CpGetCoreThreadContext(void** thread_context, UInt32 core_index)
{
    return CpKitGetCoreThreadContext(thread_context, core_index);
}

CpStatus CpCopyFpuState(void* destination_buffer, void* source_buffer)
{
    return CpKitCopyFpuState(destination_buffer, source_buffer);
}

CpStatus CpZeroFpuState(void* fpu_buffer)
{
    return CpKitZeroFpuState(fpu_buffer);
}

CpStatus CpCopyContext(CpContext* destination_context, CpContext* source_context)
{
    return CpKitCopyContext(destination_context, source_context);
}

CpStatus CpZeroContext(CpContext* context)
{
    return CpKitZeroContext(context);
}

CpStatus CpCompareContext(UInt8* is_equal, CpContext* first_context, CpContext* second_context)
{
    return CpKitCompareContext(is_equal, first_context, second_context);
}
