// Made by Berkay

#ifndef SYS_CP_H
#define SYS_CP_H

#include "sys_core.h"

BqStatus BqEnableInterrupts(void);
BqStatus BqDisableInterrupts(void);
BqStatus BqSaveAndDisableInterrupts(UInt64* interrupt_state);
BqStatus BqRestoreInterrupts(UInt64 interrupt_state);
BqStatus BqHalt(void);
BqStatus BqPause(void);
BqStatus BqRegisterInterruptHandler(UInt8 vector, CpInterruptHandler handler);
BqStatus BqUnregisterInterruptHandler(UInt8 vector);
BqStatus BqTriggerYield(void);
BqStatus BqGetFaultAddress(UInt64* fault_address);
BqStatus BqSetUserTlsBase(UInt64 fs_base_address);
BqStatus BqGetUserTlsBase(UInt64* fs_base_address);
BqStatus BqGetApicId(UInt8* apic_id);
BqStatus BqSendApicEoi(void);
BqStatus BqSendIpi(UInt8 destination_apic_id, UInt8 vector, UInt32 flags);
BqStatus BqDelayApicTimer(UInt32 milliseconds);
BqStatus BqStartApicTimer(UInt8 vector, UInt32 frequency);
BqStatus BqStopApicTimer(void);
BqStatus BqGetCoreCount(UInt32* core_count);
BqStatus BqGetOnlineCoreCount(UInt32* online_count);
BqStatus BqGetCurrentCoreIndex(UInt32* core_index);
BqStatus BqSaveFpuState(void* fpu_buffer);
BqStatus BqRestoreFpuState(void* fpu_buffer);
BqStatus BqGetFpuStateSize(UInt64* size, UInt64* alignment);
BqStatus BqShootdownTlb(UInt64 virtual_address, UInt64 page_count);
BqStatus BqBroadcastIpi(UInt8 vector, UInt32 flags, UInt8 include_self);
BqStatus BqGetCoreApicId(UInt32 core_index, UInt8* apic_id);
BqStatus BqCopyFpuState(void* destination_buffer, const void* source_buffer);
BqStatus BqZeroFpuState(void* fpu_buffer);
BqStatus BqCopyContext(CpContext* destination_context, const CpContext* source_context);
BqStatus BqZeroContext(CpContext* context);
BqStatus BqCompareContext(const CpContext* first_context, const CpContext* second_context, UInt8* is_equal);

#endif
