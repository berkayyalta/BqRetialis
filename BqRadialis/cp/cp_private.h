// Made by Berkay

#ifndef CP_PRIVATE_H
#define CP_PRIVATE_H

#include "cp_public.h"

#include "cp_madt.h"
#include "cp_gdt.h"
#include "cp_tss.h"
#include "cp_idt.h"
#include "cp_apic.h"
#include "cp_smp.h"
#include "cp_fpu.h"
#include "cp_ctx.h"

#include "../bk/bk_public.h"

typedef enum   CpMadtRecordType CpMadtRecordType;
typedef struct CpMadtSdtHeader CpMadtSdtHeader;
typedef struct CpMadtRecordHeader CpMadtRecordHeader;
typedef struct CpMadtLocalApic CpMadtLocalApic;
typedef struct CpMadtLocalApicOverride CpMadtLocalApicOverride;
typedef struct CpMadt CpMadt;

typedef enum   CpGdtSelector CpGdtSelector;
typedef enum   CpGdtAccess CpGdtAccess;
typedef enum   CpGdtGranularity CpGdtGranularity;
typedef struct CpGdtEntry CpGdtEntry;
typedef struct CpGdtRegister CpGdtRegister;
typedef struct CpGdtLedger CpGdtLedger;

typedef struct CpTssEntry CpTssEntry;
typedef struct CpTssRegister CpTssRegister;
typedef struct CpTssLedger CpTssLedger;

typedef enum   CpIdtGateType CpIdtGateType;
typedef struct CpIdtEntry CpIdtEntry;
typedef struct CpIdtRegister CpIdtRegister;
typedef struct CpIdtLedger CpIdtLedger;

typedef enum   CpApicRegisterOffset CpApicRegisterOffset;
typedef enum   CpApicSpuriousFlag CpApicSpuriousFlag;
typedef enum   CpApicIcrFlag CpApicIcrFlag;
typedef enum   CpApicTimerFlag CpApicTimerFlag;
typedef struct CpApicLedger CpApicLedger;

typedef enum   CpSmpCoreState CpSmpCoreState;
typedef struct CpSmpMailbox CpSmpMailbox;
typedef struct CpSmpCore CpSmpCore;
typedef struct CpSmpLedger CpSmpLedger;

typedef enum   CpFpuCr0Flag CpFpuCr0Flag;
typedef enum   CpFpuCr4Flag CpFpuCr4Flag;
typedef struct CpFpuState CpFpuState;
typedef struct CpFpuLedger CpFpuLedger;

typedef enum   CpCtxMode CpCtxMode;
typedef struct CpCtxContext CpCtxContext;
typedef struct CpCtxLedger CpCtxLedger;

void CpGdtSetEntry(UInt32 core_index, UInt32 entry_index, UInt8 access_rights, UInt8 granularity);
void CpGdtSetTssEntry(UInt32 core_index, UInt32 entry_index, UInt64 base_address, UInt32 limit, UInt8 access_rights, UInt8 granularity);
void CpGdtLoad(UInt32 core_index);

void   CpTssSetKernelStack(UInt32 core_index, UInt64 stack_top_address);
void   CpTssSetEntry(UInt32 core_index, UInt32 entry_index, UInt64 stack_top_address);
UInt64 CpTssGetDefaultKernelStack(UInt32 core_index);
void   CpTssLoad(UInt32 core_index);

void CpIdtSetEntry(UInt32 entry_index, UInt64 handler_address, UInt16 code_segment_selector, UInt8 type_attributes, UInt8 interrupt_stack_table_index);
void CpIdtLoad(UInt32 core_index);

CpStatus CpApicRead(UInt32* value, CpApicRegisterOffset register_offset);
CpStatus CpApicWrite(CpApicRegisterOffset register_offset, UInt32 value);
CpStatus CpApicGetId(UInt8* apic_id);
CpStatus CpApicSendEoi(void);
CpStatus CpApicSendIpi(UInt8 destination_apic_id, UInt8 vector, UInt32 flags);
CpStatus CpApicIn8(UInt8* value, UInt16 port);
CpStatus CpApicOut8(UInt16 port, UInt8 value);
CpStatus CpApicCalibrateTimer(void);
CpStatus CpApicDelay(UInt32 milliseconds);
CpStatus CpApicStartTimer(UInt8 vector, UInt32 frequency);
CpStatus CpApicStopTimer(void);
CpStatus CpApicEnable(void);
CpStatus CpApicInit(void);

CpStatus CpSmpGetCoreCount(UInt32* core_count);
CpStatus CpSmpGetOnlineCount(UInt32* online_count);
CpStatus CpSmpGetCurrentCoreIndex(UInt32* core_index);
CpStatus CpSmpGetCore(CpSmpCore** core, UInt32 core_index);
CpStatus CpSmpBindCoreState(UInt32 core_index);
CpStatus CpSmpApEntry(UInt32 core_index);
CpStatus CpSmpBootCore(UInt32 core_index, UInt64 trampoline_address, UInt32 page_table_address);
void     CpSmpSetKernelStack(UInt32 core_index, UInt64 stack_top_address);
CpStatus CpSmpInit(void);

CpStatus CpFpuSave(void* fpu_buffer);
CpStatus CpFpuRestore(void* fpu_buffer);
CpStatus CpFpuGetStateSize(UInt64* size, UInt64* alignment);
CpStatus CpFpuEnable(UInt32 core_index);
CpStatus CpFpuInit(void);

CpStatus CpCtxBuild(CpContext* context, CpContextMode mode, UInt64 entry_address, UInt64 stack_top_address, UInt64 page_table_address, UInt64 argument);
CpStatus CpCtxGetActive(CpContext* context, UInt32 core_index);
CpStatus CpCtxSetNext(UInt32 core_index, CpContext* context);
CpStatus CpCtxSwitch(CpContext* context);
CpStatus CpCtxSetPageTable(UInt64 page_table_address);
CpStatus CpCtxShootdownTlb(UInt64 virtual_address, UInt64 page_count);
CpStatus CpCtxDispatch(CpCtxContext** next_context, CpCtxContext* current_context);
CpStatus CpCtxRegisterInterruptHandler(UInt8 vector, CpInterruptHandler handler);
CpStatus CpCtxUnregisterInterruptHandler(UInt8 vector);
CpStatus CpCtxInitSyscall(CpSyscallHandler handler);
CpStatus CpSyscallDispatch(CpContext* context);
extern void CpSyscallStub(void);
CpStatus CpCtxInit(void);

CpStatus CpKitBroadcastIpi(UInt8 vector, UInt32 flags, UInt8 include_self);
CpStatus CpKitGetCoreApicId(UInt8* apic_id, UInt32 core_index);
CpStatus CpKitSetCoreThreadContext(UInt32 core_index, void* thread_context);
CpStatus CpKitGetCoreThreadContext(void** thread_context, UInt32 core_index);

CpStatus CpKitCopyFpuState(void* destination_buffer, void* source_buffer);
CpStatus CpKitZeroFpuState(void* fpu_buffer);
CpStatus CpKitGetFpuStateSize(UInt64* size, UInt64* alignment);

CpStatus CpKitCopyContext(CpContext* destination_context, CpContext* source_context);
CpStatus CpKitZeroContext(CpContext* context);
CpStatus CpKitCompareContext(UInt8* is_equal, CpContext* first_context, CpContext* second_context);

#endif
