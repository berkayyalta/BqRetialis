// Made by Berkay

#ifndef CP_PUBLIC_H
#define CP_PUBLIC_H

#include "cp_status.h"
#include "cp_context.h"

typedef enum   CpStatus CpStatus;

typedef enum   CpContextMode CpContextMode;
typedef struct CpContext CpContext;

typedef CpStatus (*CpInterruptHandler)(CpContext** next_context, CpContext* current_context);
typedef CpStatus (*CpSyscallHandler)(CpContext* context);

void CpLoad(void);

CpStatus CpInit(void);

void CpSetKernelStack(UInt32 core_index, UInt64 stack_top_address);
void CpSetTssEntry(UInt32 core_index, UInt32 entry_index, UInt64 stack_top_address);

void CpSetIdtEntry(UInt32 entry_index, UInt64 handler_address, UInt16 code_segment_selector, UInt8 type_attributes, UInt8 interrupt_stack_table_index);

void CpEnableInterrupts(void);
void CpDisableInterrupts(void);

void CpSaveAndDisableInterrupts(UInt64* interrupt_state);
void CpRestoreInterrupts(UInt64 interrupt_state);
void CpHalt(void);
void CpPause(void);

CpStatus CpRegisterInterruptHandler(UInt8 vector, CpInterruptHandler handler);
CpStatus CpUnregisterInterruptHandler(UInt8 vector);

CpStatus CpTriggerYield(void);
CpStatus CpGetFaultAddress(UInt64* fault_address);

CpStatus CpInitSyscall(CpSyscallHandler handler);
CpStatus CpSetUserTlsBase(UInt64 fs_base_address);
CpStatus CpGetUserTlsBase(UInt64* fs_base_address);

CpStatus CpGetApicId(UInt8* apic_id);
CpStatus CpSendApicEoi(void);
CpStatus CpSendIpi(UInt8 destination_apic_id, UInt8 vector, UInt32 flags);
CpStatus CpDelayApicTimer(UInt32 milliseconds);
CpStatus CpStartApicTimer(UInt8 vector, UInt32 frequency);
CpStatus CpStopApicTimer(void);

CpStatus CpGetCoreCount(UInt32* core_count);
CpStatus CpGetOnlineCoreCount(UInt32* online_count);
CpStatus CpGetCurrentCoreIndex(UInt32* core_index);

CpStatus CpSaveFpuState(void* fpu_buffer);
CpStatus CpRestoreFpuState(void* fpu_buffer);
CpStatus CpGetFpuStateSize(UInt64* size, UInt64* alignment);

CpStatus CpBuildContext(CpContext* context, CpContextMode mode, UInt64 entry_address, UInt64 stack_top_address, UInt64 page_table_address, UInt64 argument);
CpStatus CpGetActiveContext(CpContext* context, UInt32 core_index);
CpStatus CpSetNextContext(UInt32 core_index, CpContext* context);
CpStatus CpSwitchContext(CpContext* context);
CpStatus CpSetPageTable(UInt64 page_table_address);
CpStatus CpShootdownTlb(UInt64 virtual_address, UInt64 page_count);

CpStatus CpBroadcastIpi(UInt8 vector, UInt32 flags, UInt8 include_self);
CpStatus CpGetCoreApicId(UInt8* apic_id, UInt32 core_index);
CpStatus CpSetCoreThreadContext(UInt32 core_index, void* thread_context);
CpStatus CpGetCoreThreadContext(void** thread_context, UInt32 core_index);

CpStatus CpCopyFpuState(void* destination_buffer, void* source_buffer);
CpStatus CpZeroFpuState(void* fpu_buffer);

CpStatus CpCopyContext(CpContext* destination_context, CpContext* source_context);
CpStatus CpZeroContext(CpContext* context);
CpStatus CpCompareContext(UInt8* is_equal, CpContext* first_context, CpContext* second_context);

#endif
