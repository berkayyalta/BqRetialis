// Made by Berkay

#ifndef EX_PRIVATE_H
#define EX_PRIVATE_H

#include "ex_public.h"

#include "ex_sys.h"
#include "ex_ldr.h"

#include "../bk/bk_public.h"
#include "../cp/cp_public.h"
#include "../mm/mm_public.h"
#include "../hw/hw_public.h"
#include "../ps/ps_public.h"
#include "../fs/fs_public.h"
#include "../abi/abi_sc.h"

typedef struct ExSysLedger ExSysLedger;

typedef struct ExLdrElfHeader ExLdrElfHeader;
typedef struct ExLdrElfProgramHeader ExLdrElfProgramHeader;
typedef struct ExLdrLedger ExLdrLedger;

ExStatus ExSysInit(void);
ExStatus ExSysDispatch(CpContext* context);
CpStatus ExSysDispatchHandler(CpContext* context);
ExStatus ExSysGetCount(UInt32* count);
ExStatus ExSysGetDescriptor(ExSyscallDescriptor* descriptor, UInt32 syscall_id);

AbiScStatus ExSysHdlGetKernelLayout(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetMemoryMap(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetAcpiRoot(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetAcpiMadt(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetAcpiFadt(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetAcpiHpet(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetAcpiMcfg(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetFramebuffer(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetBootInfo(void* form, PsProcessPrivilege caller_privilege);

AbiScStatus ExSysHdlEnableInterrupts(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlDisableInterrupts(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSaveAndDisableInterrupts(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlRestoreInterrupts(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlHalt(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlPause(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlRegisterInterruptHandler(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlUnregisterInterruptHandler(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlTriggerYield(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetFaultAddress(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSetUserTlsBase(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetUserTlsBase(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetApicId(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSendApicEoi(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSendIpi(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlDelayApicTimer(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlStartApicTimer(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlStopApicTimer(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetCoreCount(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetOnlineCoreCount(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetCurrentCoreIndex(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSaveFpuState(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlRestoreFpuState(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetFpuStateSize(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlShootdownTlb(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlBroadcastIpi(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetCoreApicId(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCopyFpuState(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlZeroFpuState(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCopyContext(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlZeroContext(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCompareContext(void* form, PsProcessPrivilege caller_privilege);

AbiScStatus ExSysHdlAllocateVirtualRegion(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlReleaseVirtualRegion(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlMapVirtualRegion(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlUnmapVirtualRegion(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetPhysicalAddress(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlAllocateHeapBlock(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlReallocateHeapBlock(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlReleaseHeapBlock(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCopyPhysicalMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFillPhysicalMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlZeroPhysicalMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlComparePhysicalMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCopyVirtualMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFillVirtualMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlZeroVirtualMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCompareVirtualMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCopyHeapMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFillHeapMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlZeroHeapMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCompareHeapMemory(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCreateAddressSpace(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlDestroyAddressSpace(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlMapVirtualRegionInSpace(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlUnmapVirtualRegionInSpace(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetPhysicalAddressInSpace(void* form, PsProcessPrivilege caller_privilege);

AbiScStatus ExSysHdlIn8(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlOut8(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlIn16(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlOut16(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlIn32(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlOut32(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlMaskIrq(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlUnmaskIrq(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSendEoi(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSetTimerFrequency(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetTimerFrequency(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetTimerTicks(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlReadPciConfig32(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlWritePciConfig32(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetDeviceCount(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetDevice(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlRouteAcpiIrq(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlReadPciConfig8(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlWritePciConfig8(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlReadPciConfig16(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlWritePciConfig16(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlEnablePciDevice(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFindPciCapability(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlEnablePciMsi(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFindDeviceByClass(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFindDeviceById(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFindDeviceByLocation(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFindDeviceResource(void* form, PsProcessPrivilege caller_privilege);

AbiScStatus ExSysHdlSpinlockCreate(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSpinlockAcquire(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSpinlockRelease(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSpinlockDestroy(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlMutexCreate(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlMutexAcquire(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlMutexRelease(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlMutexDestroy(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetLock(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlProcessCreate(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlProcessDestroy(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlProcessGet(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlProcessGetCurrentId(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlThreadCreate(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlThreadDestroy(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlThreadGet(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlThreadGetCurrentId(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlThreadGetContext(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlThreadSetContext(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlYield(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSleep(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlBlock(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlUnblock(void* form, PsProcessPrivilege caller_privilege);

AbiScStatus ExSysHdlRegisterDriver(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlUnregisterDriver(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetDriver(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFindDriverByType(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlDriverRead(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlDriverWrite(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlDriverControl(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlMountVolume(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlUnmountVolume(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetVolume(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlFindVolumeByPath(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCreateNode(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlDestroyNode(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetNode(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlResolvePath(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetNodeByPath(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlBindNodeBuffer(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlReadNodeBuffer(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlWriteNodeBuffer(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlOpenFile(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlOpenFileByNode(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlCloseFile(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlReadFile(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlWriteFile(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlSeekFile(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetFile(void* form, PsProcessPrivilege caller_privilege);

AbiScStatus ExSysHdlGetSyscallCount(void* form, PsProcessPrivilege caller_privilege);
AbiScStatus ExSysHdlGetSyscallDescriptor(void* form, PsProcessPrivilege caller_privilege);

ExStatus ExLdrInit(void);
ExStatus ExLdrLoadExecutable(UInt32* process_id, const char* path, UInt32 is_driver);
ExStatus ExLdrLaunchDriver(UInt32* process_id, const char* path);
ExStatus ExLdrLaunchService(UInt32* process_id, const char* path);
ExStatus ExLdrStartup(void);
UInt8    ExLdrIsDriverProcess(UInt32 process_id);

AbiScStatus ExKitFromBkStatus(BkStatus status);
AbiScStatus ExKitFromCpStatus(CpStatus status);
AbiScStatus ExKitFromMmStatus(MmStatus status);
AbiScStatus ExKitFromHwStatus(HwStatus status);
AbiScStatus ExKitFromPsStatus(PsStatus status);
AbiScStatus ExKitFromFsStatus(FsStatus status);
ExStatus    ExKitToExStatus(AbiScStatus status);
ExStatus    ExKitValidateUserPointer(const void* pointer, UInt64 size);
ExStatus    ExKitGetCallerPrivilege(PsProcessPrivilege* privilege, CpContext* context);

#endif
