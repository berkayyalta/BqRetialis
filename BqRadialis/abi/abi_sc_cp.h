// Made by Berkay

#ifndef ABI_SC_CP_H
#define ABI_SC_CP_H

#include "abi_sc_status.h"
#include "../cp/cp_public.h"

struct AbiScEnableInterruptsForm
{
    UInt32 reserved;
};
typedef struct AbiScEnableInterruptsForm AbiScEnableInterruptsForm;
AbiScStatus AbiScEnableInterrupts(AbiScEnableInterruptsForm* form);

struct AbiScDisableInterruptsForm
{
    UInt32 reserved;
};
typedef struct AbiScDisableInterruptsForm AbiScDisableInterruptsForm;
AbiScStatus AbiScDisableInterrupts(AbiScDisableInterruptsForm* form);

struct AbiScSaveAndDisableInterruptsForm
{
    UInt64 interrupt_state;
};
typedef struct AbiScSaveAndDisableInterruptsForm AbiScSaveAndDisableInterruptsForm;
AbiScStatus AbiScSaveAndDisableInterrupts(AbiScSaveAndDisableInterruptsForm* form);

struct AbiScRestoreInterruptsForm
{
    UInt64 interrupt_state;
};
typedef struct AbiScRestoreInterruptsForm AbiScRestoreInterruptsForm;
AbiScStatus AbiScRestoreInterrupts(AbiScRestoreInterruptsForm* form);

struct AbiScHaltForm
{
    UInt32 reserved;
};
typedef struct AbiScHaltForm AbiScHaltForm;
AbiScStatus AbiScHalt(AbiScHaltForm* form);

struct AbiScPauseForm
{
    UInt32 reserved;
};
typedef struct AbiScPauseForm AbiScPauseForm;
AbiScStatus AbiScPause(AbiScPauseForm* form);

struct AbiScRegisterInterruptHandlerForm
{
    UInt8              vector;
    CpInterruptHandler handler;
};
typedef struct AbiScRegisterInterruptHandlerForm AbiScRegisterInterruptHandlerForm;
AbiScStatus AbiScRegisterInterruptHandler(AbiScRegisterInterruptHandlerForm* form);

struct AbiScUnregisterInterruptHandlerForm
{
    UInt8 vector;
};
typedef struct AbiScUnregisterInterruptHandlerForm AbiScUnregisterInterruptHandlerForm;
AbiScStatus AbiScUnregisterInterruptHandler(AbiScUnregisterInterruptHandlerForm* form);

struct AbiScTriggerYieldForm
{
    UInt32 reserved;
};
typedef struct AbiScTriggerYieldForm AbiScTriggerYieldForm;
AbiScStatus AbiScTriggerYield(AbiScTriggerYieldForm* form);

struct AbiScGetFaultAddressForm
{
    UInt64 fault_address;
};
typedef struct AbiScGetFaultAddressForm AbiScGetFaultAddressForm;
AbiScStatus AbiScGetFaultAddress(AbiScGetFaultAddressForm* form);

struct AbiScSetUserTlsBaseForm
{
    UInt64 fs_base_address;
};
typedef struct AbiScSetUserTlsBaseForm AbiScSetUserTlsBaseForm;
AbiScStatus AbiScSetUserTlsBase(AbiScSetUserTlsBaseForm* form);

struct AbiScGetUserTlsBaseForm
{
    UInt64 fs_base_address;
};
typedef struct AbiScGetUserTlsBaseForm AbiScGetUserTlsBaseForm;
AbiScStatus AbiScGetUserTlsBase(AbiScGetUserTlsBaseForm* form);

struct AbiScGetApicIdForm
{
    UInt8 apic_id;
};
typedef struct AbiScGetApicIdForm AbiScGetApicIdForm;
AbiScStatus AbiScGetApicId(AbiScGetApicIdForm* form);

struct AbiScSendApicEoiForm
{
    UInt32 reserved;
};
typedef struct AbiScSendApicEoiForm AbiScSendApicEoiForm;
AbiScStatus AbiScSendApicEoi(AbiScSendApicEoiForm* form);

struct AbiScSendIpiForm
{
    UInt8  destination_apic_id;
    UInt8  vector;
    UInt32 flags;
};
typedef struct AbiScSendIpiForm AbiScSendIpiForm;
AbiScStatus AbiScSendIpi(AbiScSendIpiForm* form);

struct AbiScDelayApicTimerForm
{
    UInt32 milliseconds;
};
typedef struct AbiScDelayApicTimerForm AbiScDelayApicTimerForm;
AbiScStatus AbiScDelayApicTimer(AbiScDelayApicTimerForm* form);

struct AbiScStartApicTimerForm
{
    UInt8  vector;
    UInt32 frequency;
};
typedef struct AbiScStartApicTimerForm AbiScStartApicTimerForm;
AbiScStatus AbiScStartApicTimer(AbiScStartApicTimerForm* form);

struct AbiScStopApicTimerForm
{
    UInt32 reserved;
};
typedef struct AbiScStopApicTimerForm AbiScStopApicTimerForm;
AbiScStatus AbiScStopApicTimer(AbiScStopApicTimerForm* form);

struct AbiScGetCoreCountForm
{
    UInt32 core_count;
};
typedef struct AbiScGetCoreCountForm AbiScGetCoreCountForm;
AbiScStatus AbiScGetCoreCount(AbiScGetCoreCountForm* form);

struct AbiScGetOnlineCoreCountForm
{
    UInt32 online_count;
};
typedef struct AbiScGetOnlineCoreCountForm AbiScGetOnlineCoreCountForm;
AbiScStatus AbiScGetOnlineCoreCount(AbiScGetOnlineCoreCountForm* form);

struct AbiScGetCurrentCoreIndexForm
{
    UInt32 core_index;
};
typedef struct AbiScGetCurrentCoreIndexForm AbiScGetCurrentCoreIndexForm;
AbiScStatus AbiScGetCurrentCoreIndex(AbiScGetCurrentCoreIndexForm* form);

struct AbiScSaveFpuStateForm
{
    void* fpu_buffer;
};
typedef struct AbiScSaveFpuStateForm AbiScSaveFpuStateForm;
AbiScStatus AbiScSaveFpuState(AbiScSaveFpuStateForm* form);

struct AbiScRestoreFpuStateForm
{
    void* fpu_buffer;
};
typedef struct AbiScRestoreFpuStateForm AbiScRestoreFpuStateForm;
AbiScStatus AbiScRestoreFpuState(AbiScRestoreFpuStateForm* form);

struct AbiScGetFpuStateSizeForm
{
    UInt64 size;
    UInt64 alignment;
};
typedef struct AbiScGetFpuStateSizeForm AbiScGetFpuStateSizeForm;
AbiScStatus AbiScGetFpuStateSize(AbiScGetFpuStateSizeForm* form);

struct AbiScShootdownTlbForm
{
    UInt64 virtual_address;
    UInt64 page_count;
};
typedef struct AbiScShootdownTlbForm AbiScShootdownTlbForm;
AbiScStatus AbiScShootdownTlb(AbiScShootdownTlbForm* form);

struct AbiScBroadcastIpiForm
{
    UInt8  vector;
    UInt32 flags;
    UInt8  include_self;
};
typedef struct AbiScBroadcastIpiForm AbiScBroadcastIpiForm;
AbiScStatus AbiScBroadcastIpi(AbiScBroadcastIpiForm* form);

struct AbiScGetCoreApicIdForm
{
    UInt8  apic_id;
    UInt32 core_index;
};
typedef struct AbiScGetCoreApicIdForm AbiScGetCoreApicIdForm;
AbiScStatus AbiScGetCoreApicId(AbiScGetCoreApicIdForm* form);

struct AbiScCopyFpuStateForm
{
    void* destination_buffer;
    void* source_buffer;
};
typedef struct AbiScCopyFpuStateForm AbiScCopyFpuStateForm;
AbiScStatus AbiScCopyFpuState(AbiScCopyFpuStateForm* form);

struct AbiScZeroFpuStateForm
{
    void* fpu_buffer;
};
typedef struct AbiScZeroFpuStateForm AbiScZeroFpuStateForm;
AbiScStatus AbiScZeroFpuState(AbiScZeroFpuStateForm* form);

struct AbiScCopyContextForm
{
    CpContext* destination_context;
    CpContext* source_context;
};
typedef struct AbiScCopyContextForm AbiScCopyContextForm;
AbiScStatus AbiScCopyContext(AbiScCopyContextForm* form);

struct AbiScZeroContextForm
{
    CpContext* context;
};
typedef struct AbiScZeroContextForm AbiScZeroContextForm;
AbiScStatus AbiScZeroContext(AbiScZeroContextForm* form);

struct AbiScCompareContextForm
{
    UInt8      is_equal;
    CpContext* first_context;
    CpContext* second_context;
};
typedef struct AbiScCompareContextForm AbiScCompareContextForm;
AbiScStatus AbiScCompareContext(AbiScCompareContextForm* form);

#endif
