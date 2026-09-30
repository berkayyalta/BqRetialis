// Made by Berkay

#include "sys_cp.h"

BqStatus BqEnableInterrupts(void)
{
    AbiScEnableInterruptsForm form;
    form.reserved = 0;
    return BqSyscall(ABI_SC_CP_ENABLE_INTERRUPTS, &form);
}

BqStatus BqDisableInterrupts(void)
{
    AbiScDisableInterruptsForm form;
    form.reserved = 0;
    return BqSyscall(ABI_SC_CP_DISABLE_INTERRUPTS, &form);
}

BqStatus BqSaveAndDisableInterrupts(UInt64* interrupt_state)
{
    if (interrupt_state == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScSaveAndDisableInterruptsForm form;
    form.interrupt_state = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_SAVE_AND_DISABLE_INTERRUPTS, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *interrupt_state = form.interrupt_state;
    }

    return status;
}

BqStatus BqRestoreInterrupts(UInt64 interrupt_state)
{
    AbiScRestoreInterruptsForm form;
    form.interrupt_state = interrupt_state;
    return BqSyscall(ABI_SC_CP_RESTORE_INTERRUPTS, &form);
}

BqStatus BqHalt(void)
{
    AbiScHaltForm form;
    form.reserved = 0;
    return BqSyscall(ABI_SC_CP_HALT, &form);
}

BqStatus BqPause(void)
{
    AbiScPauseForm form;
    form.reserved = 0;
    return BqSyscall(ABI_SC_CP_PAUSE, &form);
}

BqStatus BqRegisterInterruptHandler(UInt8 vector, CpInterruptHandler handler)
{
    if (handler == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScRegisterInterruptHandlerForm form;
    form.vector  = vector;
    form.handler = handler;
    return BqSyscall(ABI_SC_CP_REGISTER_INTERRUPT_HANDLER, &form);
}

BqStatus BqUnregisterInterruptHandler(UInt8 vector)
{
    AbiScUnregisterInterruptHandlerForm form;
    form.vector = vector;
    return BqSyscall(ABI_SC_CP_UNREGISTER_INTERRUPT_HANDLER, &form);
}

BqStatus BqTriggerYield(void)
{
    AbiScTriggerYieldForm form;
    form.reserved = 0;
    return BqSyscall(ABI_SC_CP_TRIGGER_YIELD, &form);
}

BqStatus BqGetFaultAddress(UInt64* fault_address)
{
    if (fault_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetFaultAddressForm form;
    form.fault_address = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_GET_FAULT_ADDRESS, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *fault_address = form.fault_address;
    }

    return status;
}

BqStatus BqSetUserTlsBase(UInt64 fs_base_address)
{
    AbiScSetUserTlsBaseForm form;
    form.fs_base_address = fs_base_address;
    return BqSyscall(ABI_SC_CP_SET_USER_TLS_BASE, &form);
}

BqStatus BqGetUserTlsBase(UInt64* fs_base_address)
{
    if (fs_base_address == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetUserTlsBaseForm form;
    form.fs_base_address = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_GET_USER_TLS_BASE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *fs_base_address = form.fs_base_address;
    }

    return status;
}

BqStatus BqGetApicId(UInt8* apic_id)
{
    if (apic_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetApicIdForm form;
    form.apic_id = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_GET_APIC_ID, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *apic_id = form.apic_id;
    }

    return status;
}

BqStatus BqSendApicEoi(void)
{
    AbiScSendApicEoiForm form;
    form.reserved = 0;
    return BqSyscall(ABI_SC_CP_SEND_APIC_EOI, &form);
}

BqStatus BqSendIpi(UInt8 destination_apic_id, UInt8 vector, UInt32 flags)
{
    AbiScSendIpiForm form;
    form.destination_apic_id = destination_apic_id;
    form.vector              = vector;
    form.flags               = flags;
    return BqSyscall(ABI_SC_CP_SEND_IPI, &form);
}

BqStatus BqDelayApicTimer(UInt32 milliseconds)
{
    AbiScDelayApicTimerForm form;
    form.milliseconds = milliseconds;
    return BqSyscall(ABI_SC_CP_DELAY_APIC_TIMER, &form);
}

BqStatus BqStartApicTimer(UInt8 vector, UInt32 frequency)
{
    AbiScStartApicTimerForm form;
    form.vector    = vector;
    form.frequency = frequency;
    return BqSyscall(ABI_SC_CP_START_APIC_TIMER, &form);
}

BqStatus BqStopApicTimer(void)
{
    AbiScStopApicTimerForm form;
    form.reserved = 0;
    return BqSyscall(ABI_SC_CP_STOP_APIC_TIMER, &form);
}

BqStatus BqGetCoreCount(UInt32* core_count)
{
    if (core_count == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetCoreCountForm form;
    form.core_count = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_GET_CORE_COUNT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *core_count = form.core_count;
    }

    return status;
}

BqStatus BqGetOnlineCoreCount(UInt32* online_count)
{
    if (online_count == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetOnlineCoreCountForm form;
    form.online_count = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_GET_ONLINE_CORE_COUNT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *online_count = form.online_count;
    }

    return status;
}

BqStatus BqGetCurrentCoreIndex(UInt32* core_index)
{
    if (core_index == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetCurrentCoreIndexForm form;
    form.core_index = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_GET_CURRENT_CORE_INDEX, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *core_index = form.core_index;
    }

    return status;
}

BqStatus BqSaveFpuState(void* fpu_buffer)
{
    if (fpu_buffer == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScSaveFpuStateForm form;
    form.fpu_buffer = fpu_buffer;
    return BqSyscall(ABI_SC_CP_SAVE_FPU_STATE, &form);
}

BqStatus BqRestoreFpuState(void* fpu_buffer)
{
    if (fpu_buffer == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScRestoreFpuStateForm form;
    form.fpu_buffer = fpu_buffer;
    return BqSyscall(ABI_SC_CP_RESTORE_FPU_STATE, &form);
}

BqStatus BqGetFpuStateSize(UInt64* size, UInt64* alignment)
{
    if (size == NULL || alignment == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetFpuStateSizeForm form;
    form.size      = 0;
    form.alignment = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_GET_FPU_STATE_SIZE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *size      = form.size;
        *alignment = form.alignment;
    }

    return status;
}

BqStatus BqShootdownTlb(UInt64 virtual_address, UInt64 page_count)
{
    AbiScShootdownTlbForm form;
    form.virtual_address = virtual_address;
    form.page_count      = page_count;
    return BqSyscall(ABI_SC_CP_SHOOTDOWN_TLB, &form);
}

BqStatus BqBroadcastIpi(UInt8 vector, UInt32 flags, UInt8 include_self)
{
    AbiScBroadcastIpiForm form;
    form.vector       = vector;
    form.flags        = flags;
    form.include_self = include_self;
    return BqSyscall(ABI_SC_CP_BROADCAST_IPI, &form);
}

BqStatus BqGetCoreApicId(UInt32 core_index, UInt8* apic_id)
{
    if (apic_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetCoreApicIdForm form;
    form.core_index = core_index;
    form.apic_id    = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_GET_CORE_APIC_ID, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *apic_id = form.apic_id;
    }

    return status;
}

BqStatus BqCopyFpuState(void* destination_buffer, const void* source_buffer)
{
    if (destination_buffer == NULL || source_buffer == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyFpuStateForm form;
    form.destination_buffer = destination_buffer;
    form.source_buffer      = (void*)source_buffer;
    return BqSyscall(ABI_SC_CP_COPY_FPU_STATE, &form);
}

BqStatus BqZeroFpuState(void* fpu_buffer)
{
    if (fpu_buffer == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroFpuStateForm form;
    form.fpu_buffer = fpu_buffer;
    return BqSyscall(ABI_SC_CP_ZERO_FPU_STATE, &form);
}

BqStatus BqCopyContext(CpContext* destination_context, const CpContext* source_context)
{
    if (destination_context == NULL || source_context == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyContextForm form;
    form.destination_context = destination_context;
    form.source_context      = (CpContext*)source_context;
    return BqSyscall(ABI_SC_CP_COPY_CONTEXT, &form);
}

BqStatus BqZeroContext(CpContext* context)
{
    if (context == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroContextForm form;
    form.context = context;
    return BqSyscall(ABI_SC_CP_ZERO_CONTEXT, &form);
}

BqStatus BqCompareContext(const CpContext* first_context, const CpContext* second_context, UInt8* is_equal)
{
    if (first_context == NULL || second_context == NULL || is_equal == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCompareContextForm form;
    form.first_context  = (CpContext*)first_context;
    form.second_context = (CpContext*)second_context;
    form.is_equal       = 0;
    BqStatus status = BqSyscall(ABI_SC_CP_COMPARE_CONTEXT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *is_equal = form.is_equal;
    }

    return status;
}
