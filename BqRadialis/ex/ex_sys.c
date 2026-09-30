// Made by Berkay

#include "ex_private.h"

static ExSysLedger s_sys_ledger;

static const struct ExSyscallDescriptor s_dispatch_table[ABI_SC_COUNT] =
{
    [ABI_SC_BK_GET_KERNEL_LAYOUT] = { ABI_SC_BK_GET_KERNEL_LAYOUT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetKernelLayoutForm), ExSysHdlGetKernelLayout, "BkGetKernelLayout" },
    [ABI_SC_BK_GET_MEMORY_MAP] = { ABI_SC_BK_GET_MEMORY_MAP, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetMemoryMapForm), ExSysHdlGetMemoryMap, "BkGetMemoryMap" },
    [ABI_SC_BK_GET_ACPI_ROOT] = { ABI_SC_BK_GET_ACPI_ROOT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetAcpiRootForm), ExSysHdlGetAcpiRoot, "BkGetAcpiRoot" },
    [ABI_SC_BK_GET_ACPI_MADT] = { ABI_SC_BK_GET_ACPI_MADT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetAcpiMadtForm), ExSysHdlGetAcpiMadt, "BkGetAcpiMadt" },
    [ABI_SC_BK_GET_ACPI_FADT] = { ABI_SC_BK_GET_ACPI_FADT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetAcpiFadtForm), ExSysHdlGetAcpiFadt, "BkGetAcpiFadt" },
    [ABI_SC_BK_GET_ACPI_HPET] = { ABI_SC_BK_GET_ACPI_HPET, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetAcpiHpetForm), ExSysHdlGetAcpiHpet, "BkGetAcpiHpet" },
    [ABI_SC_BK_GET_ACPI_MCFG] = { ABI_SC_BK_GET_ACPI_MCFG, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetAcpiMcfgForm), ExSysHdlGetAcpiMcfg, "BkGetAcpiMcfg" },
    [ABI_SC_BK_GET_FRAMEBUFFER] = { ABI_SC_BK_GET_FRAMEBUFFER, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetFramebufferForm), ExSysHdlGetFramebuffer, "BkGetFramebuffer" },
    [ABI_SC_BK_GET_BOOT_INFO] = { ABI_SC_BK_GET_BOOT_INFO, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetBootInfoForm), ExSysHdlGetBootInfo, "BkGetBootInfo" },

    [ABI_SC_CP_ENABLE_INTERRUPTS] = { ABI_SC_CP_ENABLE_INTERRUPTS, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScEnableInterruptsForm), ExSysHdlEnableInterrupts, "CpEnableInterrupts" },
    [ABI_SC_CP_DISABLE_INTERRUPTS] = { ABI_SC_CP_DISABLE_INTERRUPTS, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScDisableInterruptsForm), ExSysHdlDisableInterrupts, "CpDisableInterrupts" },
    [ABI_SC_CP_SAVE_AND_DISABLE_INTERRUPTS] = { ABI_SC_CP_SAVE_AND_DISABLE_INTERRUPTS, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScSaveAndDisableInterruptsForm), ExSysHdlSaveAndDisableInterrupts, "CpSaveAndDisableInterrupts" },
    [ABI_SC_CP_RESTORE_INTERRUPTS] = { ABI_SC_CP_RESTORE_INTERRUPTS, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScRestoreInterruptsForm), ExSysHdlRestoreInterrupts, "CpRestoreInterrupts" },
    [ABI_SC_CP_HALT] = { ABI_SC_CP_HALT, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScHaltForm), ExSysHdlHalt, "CpHalt" },
    [ABI_SC_CP_PAUSE] = { ABI_SC_CP_PAUSE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScPauseForm), ExSysHdlPause, "CpPause" },
    [ABI_SC_CP_REGISTER_INTERRUPT_HANDLER] = { ABI_SC_CP_REGISTER_INTERRUPT_HANDLER, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScRegisterInterruptHandlerForm), ExSysHdlRegisterInterruptHandler, "CpRegisterInterruptHandler" },
    [ABI_SC_CP_UNREGISTER_INTERRUPT_HANDLER] = { ABI_SC_CP_UNREGISTER_INTERRUPT_HANDLER, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScUnregisterInterruptHandlerForm), ExSysHdlUnregisterInterruptHandler, "CpUnregisterInterruptHandler" },
    [ABI_SC_CP_TRIGGER_YIELD] = { ABI_SC_CP_TRIGGER_YIELD, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScTriggerYieldForm), ExSysHdlTriggerYield, "CpTriggerYield" },
    [ABI_SC_CP_GET_FAULT_ADDRESS] = { ABI_SC_CP_GET_FAULT_ADDRESS, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetFaultAddressForm), ExSysHdlGetFaultAddress, "CpGetFaultAddress" },
    [ABI_SC_CP_SET_USER_TLS_BASE] = { ABI_SC_CP_SET_USER_TLS_BASE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScSetUserTlsBaseForm), ExSysHdlSetUserTlsBase, "CpSetUserTlsBase" },
    [ABI_SC_CP_GET_USER_TLS_BASE] = { ABI_SC_CP_GET_USER_TLS_BASE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetUserTlsBaseForm), ExSysHdlGetUserTlsBase, "CpGetUserTlsBase" },
    [ABI_SC_CP_GET_APIC_ID] = { ABI_SC_CP_GET_APIC_ID, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetApicIdForm), ExSysHdlGetApicId, "CpGetApicId" },
    [ABI_SC_CP_SEND_APIC_EOI] = { ABI_SC_CP_SEND_APIC_EOI, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScSendApicEoiForm), ExSysHdlSendApicEoi, "CpSendApicEoi" },
    [ABI_SC_CP_SEND_IPI] = { ABI_SC_CP_SEND_IPI, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScSendIpiForm), ExSysHdlSendIpi, "CpSendIpi" },
    [ABI_SC_CP_DELAY_APIC_TIMER] = { ABI_SC_CP_DELAY_APIC_TIMER, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScDelayApicTimerForm), ExSysHdlDelayApicTimer, "CpDelayApicTimer" },
    [ABI_SC_CP_START_APIC_TIMER] = { ABI_SC_CP_START_APIC_TIMER, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScStartApicTimerForm), ExSysHdlStartApicTimer, "CpStartApicTimer" },
    [ABI_SC_CP_STOP_APIC_TIMER] = { ABI_SC_CP_STOP_APIC_TIMER, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScStopApicTimerForm), ExSysHdlStopApicTimer, "CpStopApicTimer" },
    [ABI_SC_CP_GET_CORE_COUNT] = { ABI_SC_CP_GET_CORE_COUNT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetCoreCountForm), ExSysHdlGetCoreCount, "CpGetCoreCount" },
    [ABI_SC_CP_GET_ONLINE_CORE_COUNT] = { ABI_SC_CP_GET_ONLINE_CORE_COUNT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetOnlineCoreCountForm), ExSysHdlGetOnlineCoreCount, "CpGetOnlineCoreCount" },
    [ABI_SC_CP_GET_CURRENT_CORE_INDEX] = { ABI_SC_CP_GET_CURRENT_CORE_INDEX, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetCurrentCoreIndexForm), ExSysHdlGetCurrentCoreIndex, "CpGetCurrentCoreIndex" },
    [ABI_SC_CP_SAVE_FPU_STATE] = { ABI_SC_CP_SAVE_FPU_STATE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScSaveFpuStateForm), ExSysHdlSaveFpuState, "CpSaveFpuState" },
    [ABI_SC_CP_RESTORE_FPU_STATE] = { ABI_SC_CP_RESTORE_FPU_STATE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScRestoreFpuStateForm), ExSysHdlRestoreFpuState, "CpRestoreFpuState" },
    [ABI_SC_CP_GET_FPU_STATE_SIZE] = { ABI_SC_CP_GET_FPU_STATE_SIZE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetFpuStateSizeForm), ExSysHdlGetFpuStateSize, "CpGetFpuStateSize" },
    [ABI_SC_CP_SHOOTDOWN_TLB] = { ABI_SC_CP_SHOOTDOWN_TLB, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScShootdownTlbForm), ExSysHdlShootdownTlb, "CpShootdownTlb" },
    [ABI_SC_CP_BROADCAST_IPI] = { ABI_SC_CP_BROADCAST_IPI, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScBroadcastIpiForm), ExSysHdlBroadcastIpi, "CpBroadcastIpi" },
    [ABI_SC_CP_GET_CORE_APIC_ID] = { ABI_SC_CP_GET_CORE_APIC_ID, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetCoreApicIdForm), ExSysHdlGetCoreApicId, "CpGetCoreApicId" },
    [ABI_SC_CP_COPY_FPU_STATE] = { ABI_SC_CP_COPY_FPU_STATE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScCopyFpuStateForm), ExSysHdlCopyFpuState, "CpCopyFpuState" },
    [ABI_SC_CP_ZERO_FPU_STATE] = { ABI_SC_CP_ZERO_FPU_STATE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScZeroFpuStateForm), ExSysHdlZeroFpuState, "CpZeroFpuState" },
    [ABI_SC_CP_COPY_CONTEXT] = { ABI_SC_CP_COPY_CONTEXT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScCopyContextForm), ExSysHdlCopyContext, "CpCopyContext" },
    [ABI_SC_CP_ZERO_CONTEXT] = { ABI_SC_CP_ZERO_CONTEXT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScZeroContextForm), ExSysHdlZeroContext, "CpZeroContext" },
    [ABI_SC_CP_COMPARE_CONTEXT] = { ABI_SC_CP_COMPARE_CONTEXT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScCompareContextForm), ExSysHdlCompareContext, "CpCompareContext" },

    [ABI_SC_MM_ALLOCATE_VIRTUAL_REGION] = { ABI_SC_MM_ALLOCATE_VIRTUAL_REGION, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScAllocateVirtualRegionForm), ExSysHdlAllocateVirtualRegion, "MmAllocateVirtualRegion" },
    [ABI_SC_MM_RELEASE_VIRTUAL_REGION] = { ABI_SC_MM_RELEASE_VIRTUAL_REGION, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScReleaseVirtualRegionForm), ExSysHdlReleaseVirtualRegion, "MmReleaseVirtualRegion" },
    [ABI_SC_MM_MAP_VIRTUAL_REGION] = { ABI_SC_MM_MAP_VIRTUAL_REGION, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScMapVirtualRegionForm), ExSysHdlMapVirtualRegion, "MmMapVirtualRegion" },
    [ABI_SC_MM_UNMAP_VIRTUAL_REGION] = { ABI_SC_MM_UNMAP_VIRTUAL_REGION, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScUnmapVirtualRegionForm), ExSysHdlUnmapVirtualRegion, "MmUnmapVirtualRegion" },
    [ABI_SC_MM_GET_PHYSICAL_ADDRESS] = { ABI_SC_MM_GET_PHYSICAL_ADDRESS, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetPhysicalAddressForm), ExSysHdlGetPhysicalAddress, "MmGetPhysicalAddress" },
    [ABI_SC_MM_ALLOCATE_HEAP_BLOCK] = { ABI_SC_MM_ALLOCATE_HEAP_BLOCK, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScAllocateHeapBlockForm), ExSysHdlAllocateHeapBlock, "MmAllocateHeapBlock" },
    [ABI_SC_MM_REALLOCATE_HEAP_BLOCK] = { ABI_SC_MM_REALLOCATE_HEAP_BLOCK, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScReallocateHeapBlockForm), ExSysHdlReallocateHeapBlock, "MmReallocateHeapBlock" },
    [ABI_SC_MM_RELEASE_HEAP_BLOCK] = { ABI_SC_MM_RELEASE_HEAP_BLOCK, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScReleaseHeapBlockForm), ExSysHdlReleaseHeapBlock, "MmReleaseHeapBlock" },
    [ABI_SC_MM_COPY_PHYSICAL_MEMORY] = { ABI_SC_MM_COPY_PHYSICAL_MEMORY, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScCopyPhysicalMemoryForm), ExSysHdlCopyPhysicalMemory, "MmCopyPhysicalMemory" },
    [ABI_SC_MM_FILL_PHYSICAL_MEMORY] = { ABI_SC_MM_FILL_PHYSICAL_MEMORY, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScFillPhysicalMemoryForm), ExSysHdlFillPhysicalMemory, "MmFillPhysicalMemory" },
    [ABI_SC_MM_ZERO_PHYSICAL_MEMORY] = { ABI_SC_MM_ZERO_PHYSICAL_MEMORY, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScZeroPhysicalMemoryForm), ExSysHdlZeroPhysicalMemory, "MmZeroPhysicalMemory" },
    [ABI_SC_MM_COMPARE_PHYSICAL_MEMORY] = { ABI_SC_MM_COMPARE_PHYSICAL_MEMORY, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScComparePhysicalMemoryForm), ExSysHdlComparePhysicalMemory, "MmComparePhysicalMemory" },
    [ABI_SC_MM_COPY_VIRTUAL_MEMORY] = { ABI_SC_MM_COPY_VIRTUAL_MEMORY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScCopyVirtualMemoryForm), ExSysHdlCopyVirtualMemory, "MmCopyVirtualMemory" },
    [ABI_SC_MM_FILL_VIRTUAL_MEMORY] = { ABI_SC_MM_FILL_VIRTUAL_MEMORY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScFillVirtualMemoryForm), ExSysHdlFillVirtualMemory, "MmFillVirtualMemory" },
    [ABI_SC_MM_ZERO_VIRTUAL_MEMORY] = { ABI_SC_MM_ZERO_VIRTUAL_MEMORY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScZeroVirtualMemoryForm), ExSysHdlZeroVirtualMemory, "MmZeroVirtualMemory" },
    [ABI_SC_MM_COMPARE_VIRTUAL_MEMORY] = { ABI_SC_MM_COMPARE_VIRTUAL_MEMORY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScCompareVirtualMemoryForm), ExSysHdlCompareVirtualMemory, "MmCompareVirtualMemory" },
    [ABI_SC_MM_COPY_HEAP_MEMORY] = { ABI_SC_MM_COPY_HEAP_MEMORY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScCopyHeapMemoryForm), ExSysHdlCopyHeapMemory, "MmCopyHeapMemory" },
    [ABI_SC_MM_FILL_HEAP_MEMORY] = { ABI_SC_MM_FILL_HEAP_MEMORY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScFillHeapMemoryForm), ExSysHdlFillHeapMemory, "MmFillHeapMemory" },
    [ABI_SC_MM_ZERO_HEAP_MEMORY] = { ABI_SC_MM_ZERO_HEAP_MEMORY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScZeroHeapMemoryForm), ExSysHdlZeroHeapMemory, "MmZeroHeapMemory" },
    [ABI_SC_MM_COMPARE_HEAP_MEMORY] = { ABI_SC_MM_COMPARE_HEAP_MEMORY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScCompareHeapMemoryForm), ExSysHdlCompareHeapMemory, "MmCompareHeapMemory" },
    [ABI_SC_MM_CREATE_ADDRESS_SPACE] = { ABI_SC_MM_CREATE_ADDRESS_SPACE, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScCreateAddressSpaceForm), ExSysHdlCreateAddressSpace, "MmCreateAddressSpace" },
    [ABI_SC_MM_DESTROY_ADDRESS_SPACE] = { ABI_SC_MM_DESTROY_ADDRESS_SPACE, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScDestroyAddressSpaceForm), ExSysHdlDestroyAddressSpace, "MmDestroyAddressSpace" },
    [ABI_SC_MM_MAP_VIRTUAL_REGION_IN_SPACE] = { ABI_SC_MM_MAP_VIRTUAL_REGION_IN_SPACE, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScMapVirtualRegionInSpaceForm), ExSysHdlMapVirtualRegionInSpace, "MmMapVirtualRegionInSpace" },
    [ABI_SC_MM_UNMAP_VIRTUAL_REGION_IN_SPACE] = { ABI_SC_MM_UNMAP_VIRTUAL_REGION_IN_SPACE, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScUnmapVirtualRegionInSpaceForm), ExSysHdlUnmapVirtualRegionInSpace, "MmUnmapVirtualRegionInSpace" },
    [ABI_SC_MM_GET_PHYSICAL_ADDRESS_IN_SPACE] = { ABI_SC_MM_GET_PHYSICAL_ADDRESS_IN_SPACE, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScGetPhysicalAddressInSpaceForm), ExSysHdlGetPhysicalAddressInSpace, "MmGetPhysicalAddressInSpace" },

    [ABI_SC_HW_IN8] = { ABI_SC_HW_IN8, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScIn8Form), ExSysHdlIn8, "HwIn8" },
    [ABI_SC_HW_OUT8] = { ABI_SC_HW_OUT8, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScOut8Form), ExSysHdlOut8, "HwOut8" },
    [ABI_SC_HW_IN16] = { ABI_SC_HW_IN16, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScIn16Form), ExSysHdlIn16, "HwIn16" },
    [ABI_SC_HW_OUT16] = { ABI_SC_HW_OUT16, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScOut16Form), ExSysHdlOut16, "HwOut16" },
    [ABI_SC_HW_IN32] = { ABI_SC_HW_IN32, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScIn32Form), ExSysHdlIn32, "HwIn32" },
    [ABI_SC_HW_OUT32] = { ABI_SC_HW_OUT32, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScOut32Form), ExSysHdlOut32, "HwOut32" },
    [ABI_SC_HW_MASK_IRQ] = { ABI_SC_HW_MASK_IRQ, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScMaskIrqForm), ExSysHdlMaskIrq, "HwMaskIrq" },
    [ABI_SC_HW_UNMASK_IRQ] = { ABI_SC_HW_UNMASK_IRQ, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScUnmaskIrqForm), ExSysHdlUnmaskIrq, "HwUnmaskIrq" },
    [ABI_SC_HW_SEND_EOI] = { ABI_SC_HW_SEND_EOI, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScSendEoiForm), ExSysHdlSendEoi, "HwSendEoi" },
    [ABI_SC_HW_SET_TIMER_FREQUENCY] = { ABI_SC_HW_SET_TIMER_FREQUENCY, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScSetTimerFrequencyForm), ExSysHdlSetTimerFrequency, "HwSetTimerFrequency" },
    [ABI_SC_HW_GET_TIMER_FREQUENCY] = { ABI_SC_HW_GET_TIMER_FREQUENCY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetTimerFrequencyForm), ExSysHdlGetTimerFrequency, "HwGetTimerFrequency" },
    [ABI_SC_HW_GET_TIMER_TICKS] = { ABI_SC_HW_GET_TIMER_TICKS, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetTimerTicksForm), ExSysHdlGetTimerTicks, "HwGetTimerTicks" },
    [ABI_SC_HW_READ_PCI_CONFIG32] = { ABI_SC_HW_READ_PCI_CONFIG32, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScReadPciConfig32Form), ExSysHdlReadPciConfig32, "HwReadPciConfig32" },
    [ABI_SC_HW_WRITE_PCI_CONFIG32] = { ABI_SC_HW_WRITE_PCI_CONFIG32, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScWritePciConfig32Form), ExSysHdlWritePciConfig32, "HwWritePciConfig32" },
    [ABI_SC_HW_GET_DEVICE_COUNT] = { ABI_SC_HW_GET_DEVICE_COUNT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetDeviceCountForm), ExSysHdlGetDeviceCount, "HwGetDeviceCount" },
    [ABI_SC_HW_GET_DEVICE] = { ABI_SC_HW_GET_DEVICE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetDeviceForm), ExSysHdlGetDevice, "HwGetDevice" },
    [ABI_SC_HW_ROUTE_ACPI_IRQ] = { ABI_SC_HW_ROUTE_ACPI_IRQ, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScRouteAcpiIrqForm), ExSysHdlRouteAcpiIrq, "HwRouteAcpiIrq" },
    [ABI_SC_HW_READ_PCI_CONFIG8] = { ABI_SC_HW_READ_PCI_CONFIG8, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScReadPciConfig8Form), ExSysHdlReadPciConfig8, "HwReadPciConfig8" },
    [ABI_SC_HW_WRITE_PCI_CONFIG8] = { ABI_SC_HW_WRITE_PCI_CONFIG8, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScWritePciConfig8Form), ExSysHdlWritePciConfig8, "HwWritePciConfig8" },
    [ABI_SC_HW_READ_PCI_CONFIG16] = { ABI_SC_HW_READ_PCI_CONFIG16, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScReadPciConfig16Form), ExSysHdlReadPciConfig16, "HwReadPciConfig16" },
    [ABI_SC_HW_WRITE_PCI_CONFIG16] = { ABI_SC_HW_WRITE_PCI_CONFIG16, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScWritePciConfig16Form), ExSysHdlWritePciConfig16, "HwWritePciConfig16" },
    [ABI_SC_HW_ENABLE_PCI_DEVICE] = { ABI_SC_HW_ENABLE_PCI_DEVICE, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScEnablePciDeviceForm), ExSysHdlEnablePciDevice, "HwEnablePciDevice" },
    [ABI_SC_HW_FIND_PCI_CAPABILITY] = { ABI_SC_HW_FIND_PCI_CAPABILITY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScFindPciCapabilityForm), ExSysHdlFindPciCapability, "HwFindPciCapability" },
    [ABI_SC_HW_ENABLE_PCI_MSI] = { ABI_SC_HW_ENABLE_PCI_MSI, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScEnablePciMsiForm), ExSysHdlEnablePciMsi, "HwEnablePciMsi" },
    [ABI_SC_HW_FIND_DEVICE_BY_CLASS] = { ABI_SC_HW_FIND_DEVICE_BY_CLASS, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScFindDeviceByClassForm), ExSysHdlFindDeviceByClass, "HwFindDeviceByClass" },
    [ABI_SC_HW_FIND_DEVICE_BY_ID] = { ABI_SC_HW_FIND_DEVICE_BY_ID, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScFindDeviceByIdForm), ExSysHdlFindDeviceById, "HwFindDeviceById" },
    [ABI_SC_HW_FIND_DEVICE_BY_LOCATION] = { ABI_SC_HW_FIND_DEVICE_BY_LOCATION, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScFindDeviceByLocationForm), ExSysHdlFindDeviceByLocation, "HwFindDeviceByLocation" },
    [ABI_SC_HW_FIND_DEVICE_RESOURCE] = { ABI_SC_HW_FIND_DEVICE_RESOURCE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScFindDeviceResourceForm), ExSysHdlFindDeviceResource, "HwFindDeviceResource" },

    [ABI_SC_PS_SPINLOCK_CREATE] = { ABI_SC_PS_SPINLOCK_CREATE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScSpinlockCreateForm), ExSysHdlSpinlockCreate, "PsSpinlockCreate" },
    [ABI_SC_PS_SPINLOCK_ACQUIRE] = { ABI_SC_PS_SPINLOCK_ACQUIRE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScSpinlockAcquireForm), ExSysHdlSpinlockAcquire, "PsSpinlockAcquire" },
    [ABI_SC_PS_SPINLOCK_RELEASE] = { ABI_SC_PS_SPINLOCK_RELEASE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScSpinlockReleaseForm), ExSysHdlSpinlockRelease, "PsSpinlockRelease" },
    [ABI_SC_PS_SPINLOCK_DESTROY] = { ABI_SC_PS_SPINLOCK_DESTROY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScSpinlockDestroyForm), ExSysHdlSpinlockDestroy, "PsSpinlockDestroy" },
    [ABI_SC_PS_MUTEX_CREATE] = { ABI_SC_PS_MUTEX_CREATE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScMutexCreateForm), ExSysHdlMutexCreate, "PsMutexCreate" },
    [ABI_SC_PS_MUTEX_ACQUIRE] = { ABI_SC_PS_MUTEX_ACQUIRE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScMutexAcquireForm), ExSysHdlMutexAcquire, "PsMutexAcquire" },
    [ABI_SC_PS_MUTEX_RELEASE] = { ABI_SC_PS_MUTEX_RELEASE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScMutexReleaseForm), ExSysHdlMutexRelease, "PsMutexRelease" },
    [ABI_SC_PS_MUTEX_DESTROY] = { ABI_SC_PS_MUTEX_DESTROY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScMutexDestroyForm), ExSysHdlMutexDestroy, "PsMutexDestroy" },
    [ABI_SC_PS_GET_LOCK] = { ABI_SC_PS_GET_LOCK, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetLockForm), ExSysHdlGetLock, "PsGetLock" },
    [ABI_SC_PS_PROCESS_CREATE] = { ABI_SC_PS_PROCESS_CREATE, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScProcessCreateForm), ExSysHdlProcessCreate, "PsProcessCreate" },
    [ABI_SC_PS_PROCESS_DESTROY] = { ABI_SC_PS_PROCESS_DESTROY, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScProcessDestroyForm), ExSysHdlProcessDestroy, "PsProcessDestroy" },
    [ABI_SC_PS_PROCESS_GET] = { ABI_SC_PS_PROCESS_GET, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScProcessGetForm), ExSysHdlProcessGet, "PsProcessGet" },
    [ABI_SC_PS_PROCESS_GET_CURRENT_ID] = { ABI_SC_PS_PROCESS_GET_CURRENT_ID, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScProcessGetCurrentIdForm), ExSysHdlProcessGetCurrentId, "PsProcessGetCurrentId" },
    [ABI_SC_PS_THREAD_CREATE] = { ABI_SC_PS_THREAD_CREATE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScThreadCreateForm), ExSysHdlThreadCreate, "PsThreadCreate" },
    [ABI_SC_PS_THREAD_DESTROY] = { ABI_SC_PS_THREAD_DESTROY, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScThreadDestroyForm), ExSysHdlThreadDestroy, "PsThreadDestroy" },
    [ABI_SC_PS_THREAD_GET] = { ABI_SC_PS_THREAD_GET, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScThreadGetForm), ExSysHdlThreadGet, "PsThreadGet" },
    [ABI_SC_PS_THREAD_GET_CURRENT_ID] = { ABI_SC_PS_THREAD_GET_CURRENT_ID, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScThreadGetCurrentIdForm), ExSysHdlThreadGetCurrentId, "PsThreadGetCurrentId" },
    [ABI_SC_PS_THREAD_GET_CONTEXT] = { ABI_SC_PS_THREAD_GET_CONTEXT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScThreadGetContextForm), ExSysHdlThreadGetContext, "PsThreadGetContext" },
    [ABI_SC_PS_THREAD_SET_CONTEXT] = { ABI_SC_PS_THREAD_SET_CONTEXT, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScThreadSetContextForm), ExSysHdlThreadSetContext, "PsThreadSetContext" },
    [ABI_SC_PS_YIELD] = { ABI_SC_PS_YIELD, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScYieldForm), ExSysHdlYield, "PsYield" },
    [ABI_SC_PS_SLEEP] = { ABI_SC_PS_SLEEP, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScSleepForm), ExSysHdlSleep, "PsSleep" },
    [ABI_SC_PS_BLOCK] = { ABI_SC_PS_BLOCK, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScBlockForm), ExSysHdlBlock, "PsBlock" },
    [ABI_SC_PS_UNBLOCK] = { ABI_SC_PS_UNBLOCK, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScUnblockForm), ExSysHdlUnblock, "PsUnblock" },

    [ABI_SC_FS_REGISTER_DRIVER] = { ABI_SC_FS_REGISTER_DRIVER, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScRegisterDriverForm), ExSysHdlRegisterDriver, "FsRegisterDriver" },
    [ABI_SC_FS_UNREGISTER_DRIVER] = { ABI_SC_FS_UNREGISTER_DRIVER, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScUnregisterDriverForm), ExSysHdlUnregisterDriver, "FsUnregisterDriver" },
    [ABI_SC_FS_GET_DRIVER] = { ABI_SC_FS_GET_DRIVER, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetDriverForm), ExSysHdlGetDriver, "FsGetDriver" },
    [ABI_SC_FS_FIND_DRIVER_BY_TYPE] = { ABI_SC_FS_FIND_DRIVER_BY_TYPE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScFindDriverByTypeForm), ExSysHdlFindDriverByType, "FsFindDriverByType" },
    [ABI_SC_FS_DRIVER_READ] = { ABI_SC_FS_DRIVER_READ, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScDriverReadForm), ExSysHdlDriverRead, "FsDriverRead" },
    [ABI_SC_FS_DRIVER_WRITE] = { ABI_SC_FS_DRIVER_WRITE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScDriverWriteForm), ExSysHdlDriverWrite, "FsDriverWrite" },
    [ABI_SC_FS_DRIVER_CONTROL] = { ABI_SC_FS_DRIVER_CONTROL, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScDriverControlForm), ExSysHdlDriverControl, "FsDriverControl" },
    [ABI_SC_FS_MOUNT_VOLUME] = { ABI_SC_FS_MOUNT_VOLUME, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScMountVolumeForm), ExSysHdlMountVolume, "FsMountVolume" },
    [ABI_SC_FS_UNMOUNT_VOLUME] = { ABI_SC_FS_UNMOUNT_VOLUME, PS_PROCESS_PRIVILEGE_KERNEL, sizeof(AbiScUnmountVolumeForm), ExSysHdlUnmountVolume, "FsUnmountVolume" },
    [ABI_SC_FS_GET_VOLUME] = { ABI_SC_FS_GET_VOLUME, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetVolumeForm), ExSysHdlGetVolume, "FsGetVolume" },
    [ABI_SC_FS_FIND_VOLUME_BY_PATH] = { ABI_SC_FS_FIND_VOLUME_BY_PATH, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScFindVolumeByPathForm), ExSysHdlFindVolumeByPath, "FsFindVolumeByPath" },
    [ABI_SC_FS_CREATE_NODE] = { ABI_SC_FS_CREATE_NODE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScCreateNodeForm), ExSysHdlCreateNode, "FsCreateNode" },
    [ABI_SC_FS_DESTROY_NODE] = { ABI_SC_FS_DESTROY_NODE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScDestroyNodeForm), ExSysHdlDestroyNode, "FsDestroyNode" },
    [ABI_SC_FS_GET_NODE] = { ABI_SC_FS_GET_NODE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetNodeForm), ExSysHdlGetNode, "FsGetNode" },
    [ABI_SC_FS_RESOLVE_PATH] = { ABI_SC_FS_RESOLVE_PATH, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScResolvePathForm), ExSysHdlResolvePath, "FsResolvePath" },
    [ABI_SC_FS_GET_NODE_BY_PATH] = { ABI_SC_FS_GET_NODE_BY_PATH, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetNodeByPathForm), ExSysHdlGetNodeByPath, "FsGetNodeByPath" },
    [ABI_SC_FS_BIND_NODE_BUFFER] = { ABI_SC_FS_BIND_NODE_BUFFER, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScBindNodeBufferForm), ExSysHdlBindNodeBuffer, "FsBindNodeBuffer" },
    [ABI_SC_FS_READ_NODE_BUFFER] = { ABI_SC_FS_READ_NODE_BUFFER, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScReadNodeBufferForm), ExSysHdlReadNodeBuffer, "FsReadNodeBuffer" },
    [ABI_SC_FS_WRITE_NODE_BUFFER] = { ABI_SC_FS_WRITE_NODE_BUFFER, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScWriteNodeBufferForm), ExSysHdlWriteNodeBuffer, "FsWriteNodeBuffer" },
    [ABI_SC_FS_OPEN_FILE] = { ABI_SC_FS_OPEN_FILE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScOpenFileForm), ExSysHdlOpenFile, "FsOpenFile" },
    [ABI_SC_FS_OPEN_FILE_BY_NODE] = { ABI_SC_FS_OPEN_FILE_BY_NODE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScOpenFileByNodeForm), ExSysHdlOpenFileByNode, "FsOpenFileByNode" },
    [ABI_SC_FS_CLOSE_FILE] = { ABI_SC_FS_CLOSE_FILE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScCloseFileForm), ExSysHdlCloseFile, "FsCloseFile" },
    [ABI_SC_FS_READ_FILE] = { ABI_SC_FS_READ_FILE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScReadFileForm), ExSysHdlReadFile, "FsReadFile" },
    [ABI_SC_FS_WRITE_FILE] = { ABI_SC_FS_WRITE_FILE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScWriteFileForm), ExSysHdlWriteFile, "FsWriteFile" },
    [ABI_SC_FS_SEEK_FILE] = { ABI_SC_FS_SEEK_FILE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScSeekFileForm), ExSysHdlSeekFile, "FsSeekFile" },
    [ABI_SC_FS_GET_FILE] = { ABI_SC_FS_GET_FILE, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetFileForm), ExSysHdlGetFile, "FsGetFile" },

    [ABI_SC_EX_GET_SYSCALL_COUNT] = { ABI_SC_EX_GET_SYSCALL_COUNT, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetSyscallCountForm), ExSysHdlGetSyscallCount, "ExGetSyscallCount" },
    [ABI_SC_EX_GET_SYSCALL_DESCRIPTOR] = { ABI_SC_EX_GET_SYSCALL_DESCRIPTOR, PS_PROCESS_PRIVILEGE_USER, sizeof(AbiScGetSyscallDescriptorForm), ExSysHdlGetSyscallDescriptor, "ExGetSyscallDescriptor" }
};

ExStatus ExSysInit(void)
{
    for (UInt64 i = 0; i < sizeof(ExSysLedger); i++)
    {
        ((UInt8*)&s_sys_ledger)[i] = 0;
    }

    for (UInt32 i = 0; i < ABI_SC_COUNT; i++)
    {
        s_sys_ledger.dispatch_table[i] = s_dispatch_table[i];
    }

    s_sys_ledger.is_initialized = 1;
    return EX_STATUS_SUCCESS;
}

ExStatus ExSysDispatch(CpContext* context)
{
    if (context == NULL)
    {
        return EX_STATUS_INVALID_ARGUMENT;
    }

    UInt64 syscall_id = context->rax;
    if (syscall_id >= ABI_SC_COUNT)
    {
        context->rax = (UInt64)ABI_SC_STATUS_INVALID_SYSCALL;
        return EX_STATUS_INVALID_SYSCALL;
    }

    const ExSyscallDescriptor* descriptor = &s_sys_ledger.dispatch_table[syscall_id];
    if (descriptor->handler == NULL)
    {
        context->rax = (UInt64)ABI_SC_STATUS_NOT_SUPPORTED;
        return EX_STATUS_NOT_SUPPORTED;
    }

    PsProcessPrivilege caller_privilege = PS_PROCESS_PRIVILEGE_USER;
    ExStatus priv_status = ExKitGetCallerPrivilege(&caller_privilege, context);
    if (priv_status != EX_STATUS_SUCCESS)
    {
        context->rax = (UInt64)ABI_SC_STATUS_ACCESS_DENIED;
        return priv_status;
    }

    if (caller_privilege > descriptor->min_privilege)
    {
        context->rax = (UInt64)ABI_SC_STATUS_ACCESS_DENIED;
        return EX_STATUS_ACCESS_DENIED;
    }

    void* form = (void*)context->rdi;
    if (descriptor->form_size > 0)
    {
        ExStatus val_status = ExKitValidateUserPointer(form, descriptor->form_size);
        if (val_status != EX_STATUS_SUCCESS)
        {
            context->rax = (UInt64)ABI_SC_STATUS_INVALID_ARGUMENT;
            return val_status;
        }
    }

    AbiScStatus sc_status = descriptor->handler(form, caller_privilege);
    context->rax = (UInt64)sc_status;
    s_sys_ledger.total_dispatches++;

    return EX_STATUS_SUCCESS;
}

CpStatus ExSysDispatchHandler(CpContext* context)
{
    ExStatus status = ExSysDispatch(context);
    if (status != EX_STATUS_SUCCESS)
    {
        return CP_STATUS_INVALID_ARGUMENT;
    }

    return CP_STATUS_SUCCESS;
}

ExStatus ExSysGetCount(UInt32* count)
{
    if (count == NULL)
    {
        return EX_STATUS_INVALID_ARGUMENT;
    }

    *count = ABI_SC_COUNT;
    return EX_STATUS_SUCCESS;
}

ExStatus ExSysGetDescriptor(ExSyscallDescriptor* descriptor, UInt32 syscall_id)
{
    if (descriptor == NULL || syscall_id >= ABI_SC_COUNT)
    {
        return EX_STATUS_INVALID_ARGUMENT;
    }

    *descriptor = s_sys_ledger.dispatch_table[syscall_id];
    return EX_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetKernelLayout(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetKernelLayoutForm* typed = (AbiScGetKernelLayoutForm*)form;
    typed->layout = BkGetKernelLayout();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetMemoryMap(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetMemoryMapForm* typed = (AbiScGetMemoryMapForm*)form;
    typed->memory_map = BkGetMemoryMap();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetAcpiRoot(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiRootForm* typed = (AbiScGetAcpiRootForm*)form;
    typed->acpi_root = BkGetAcpiRoot();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetAcpiMadt(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiMadtForm* typed = (AbiScGetAcpiMadtForm*)form;
    typed->madt = BkGetAcpiMadt();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetAcpiFadt(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiFadtForm* typed = (AbiScGetAcpiFadtForm*)form;
    typed->fadt = BkGetAcpiFadt();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetAcpiHpet(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiHpetForm* typed = (AbiScGetAcpiHpetForm*)form;
    typed->hpet = BkGetAcpiHpet();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetAcpiMcfg(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetAcpiMcfgForm* typed = (AbiScGetAcpiMcfgForm*)form;
    typed->mcfg = BkGetAcpiMcfg();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetFramebuffer(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetFramebufferForm* typed = (AbiScGetFramebufferForm*)form;
    typed->framebuffer = BkGetFramebuffer();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetBootInfo(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetBootInfoForm* typed = (AbiScGetBootInfoForm*)form;
    typed->boot_info = BkGetBootInfo();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlEnableInterrupts(void* form, PsProcessPrivilege caller_privilege)
{
    (void)form;
    (void)caller_privilege;
    CpEnableInterrupts();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlDisableInterrupts(void* form, PsProcessPrivilege caller_privilege)
{
    (void)form;
    (void)caller_privilege;
    CpDisableInterrupts();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlSaveAndDisableInterrupts(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSaveAndDisableInterruptsForm* typed = (AbiScSaveAndDisableInterruptsForm*)form;
    CpSaveAndDisableInterrupts(&typed->interrupt_state);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlRestoreInterrupts(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScRestoreInterruptsForm* typed = (AbiScRestoreInterruptsForm*)form;
    CpRestoreInterrupts(typed->interrupt_state);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlHalt(void* form, PsProcessPrivilege caller_privilege)
{
    (void)form;
    (void)caller_privilege;
    CpHalt();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlPause(void* form, PsProcessPrivilege caller_privilege)
{
    (void)form;
    (void)caller_privilege;
    CpPause();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlRegisterInterruptHandler(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScRegisterInterruptHandlerForm* typed = (AbiScRegisterInterruptHandlerForm*)form;
    return ExKitFromCpStatus(CpRegisterInterruptHandler(typed->vector, typed->handler));
}

AbiScStatus ExSysHdlUnregisterInterruptHandler(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScUnregisterInterruptHandlerForm* typed = (AbiScUnregisterInterruptHandlerForm*)form;
    return ExKitFromCpStatus(CpUnregisterInterruptHandler(typed->vector));
}

AbiScStatus ExSysHdlTriggerYield(void* form, PsProcessPrivilege caller_privilege)
{
    (void)form;
    (void)caller_privilege;
    return ExKitFromCpStatus(CpTriggerYield());
}

AbiScStatus ExSysHdlGetFaultAddress(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetFaultAddressForm* typed = (AbiScGetFaultAddressForm*)form;
    return ExKitFromCpStatus(CpGetFaultAddress(&typed->fault_address));
}

AbiScStatus ExSysHdlSetUserTlsBase(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSetUserTlsBaseForm* typed = (AbiScSetUserTlsBaseForm*)form;
    UInt32 low  = (UInt32)(typed->fs_base_address & 0xFFFFFFFFULL);
    UInt32 high = (UInt32)(typed->fs_base_address >> 32);
    __asm__ volatile ("wrmsr" : : "c"(0xC0000100U), "a"(low), "d"(high) : "memory");
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetUserTlsBase(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetUserTlsBaseForm* typed = (AbiScGetUserTlsBaseForm*)form;
    UInt32 low  = 0;
    UInt32 high = 0;
    __asm__ volatile ("rdmsr" : "=a"(low), "=d"(high) : "c"(0xC0000100U));
    typed->fs_base_address = (((UInt64)high) << 32) | (UInt64)low;
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetApicId(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetApicIdForm* typed = (AbiScGetApicIdForm*)form;
    return ExKitFromCpStatus(CpGetApicId(&typed->apic_id));
}

AbiScStatus ExSysHdlSendApicEoi(void* form, PsProcessPrivilege caller_privilege)
{
    (void)form;
    (void)caller_privilege;
    return ExKitFromCpStatus(CpSendApicEoi());
}

AbiScStatus ExSysHdlSendIpi(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSendIpiForm* typed = (AbiScSendIpiForm*)form;
    return ExKitFromCpStatus(CpSendIpi(typed->destination_apic_id, typed->vector, typed->flags));
}

AbiScStatus ExSysHdlDelayApicTimer(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScDelayApicTimerForm* typed = (AbiScDelayApicTimerForm*)form;
    return ExKitFromCpStatus(CpDelayApicTimer(typed->milliseconds));
}

AbiScStatus ExSysHdlStartApicTimer(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScStartApicTimerForm* typed = (AbiScStartApicTimerForm*)form;
    return ExKitFromCpStatus(CpStartApicTimer(typed->vector, typed->frequency));
}

AbiScStatus ExSysHdlStopApicTimer(void* form, PsProcessPrivilege caller_privilege)
{
    (void)form;
    (void)caller_privilege;
    return ExKitFromCpStatus(CpStopApicTimer());
}

AbiScStatus ExSysHdlGetCoreCount(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetCoreCountForm* typed = (AbiScGetCoreCountForm*)form;
    return ExKitFromCpStatus(CpGetCoreCount(&typed->core_count));
}

AbiScStatus ExSysHdlGetOnlineCoreCount(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetOnlineCoreCountForm* typed = (AbiScGetOnlineCoreCountForm*)form;
    return ExKitFromCpStatus(CpGetOnlineCoreCount(&typed->online_count));
}

AbiScStatus ExSysHdlGetCurrentCoreIndex(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetCurrentCoreIndexForm* typed = (AbiScGetCurrentCoreIndexForm*)form;
    return ExKitFromCpStatus(CpGetCurrentCoreIndex(&typed->core_index));
}

AbiScStatus ExSysHdlSaveFpuState(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSaveFpuStateForm* typed = (AbiScSaveFpuStateForm*)form;
    return ExKitFromCpStatus(CpSaveFpuState(typed->fpu_buffer));
}

AbiScStatus ExSysHdlRestoreFpuState(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScRestoreFpuStateForm* typed = (AbiScRestoreFpuStateForm*)form;
    return ExKitFromCpStatus(CpRestoreFpuState(typed->fpu_buffer));
}

AbiScStatus ExSysHdlGetFpuStateSize(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetFpuStateSizeForm* typed = (AbiScGetFpuStateSizeForm*)form;
    return ExKitFromCpStatus(CpGetFpuStateSize(&typed->size, &typed->alignment));
}

AbiScStatus ExSysHdlShootdownTlb(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScShootdownTlbForm* typed = (AbiScShootdownTlbForm*)form;
    return ExKitFromCpStatus(CpShootdownTlb(typed->virtual_address, typed->page_count));
}

AbiScStatus ExSysHdlBroadcastIpi(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScBroadcastIpiForm* typed = (AbiScBroadcastIpiForm*)form;
    return ExKitFromCpStatus(CpBroadcastIpi(typed->vector, typed->flags, typed->include_self));
}

AbiScStatus ExSysHdlGetCoreApicId(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetCoreApicIdForm* typed = (AbiScGetCoreApicIdForm*)form;
    return ExKitFromCpStatus(CpGetCoreApicId(&typed->apic_id, typed->core_index));
}

AbiScStatus ExSysHdlCopyFpuState(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyFpuStateForm* typed = (AbiScCopyFpuStateForm*)form;
    return ExKitFromCpStatus(CpCopyFpuState(typed->destination_buffer, typed->source_buffer));
}

AbiScStatus ExSysHdlZeroFpuState(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroFpuStateForm* typed = (AbiScZeroFpuStateForm*)form;
    return ExKitFromCpStatus(CpZeroFpuState(typed->fpu_buffer));
}

AbiScStatus ExSysHdlCopyContext(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyContextForm* typed = (AbiScCopyContextForm*)form;
    return ExKitFromCpStatus(CpCopyContext(typed->destination_context, typed->source_context));
}

AbiScStatus ExSysHdlZeroContext(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroContextForm* typed = (AbiScZeroContextForm*)form;
    return ExKitFromCpStatus(CpZeroContext(typed->context));
}

AbiScStatus ExSysHdlCompareContext(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCompareContextForm* typed = (AbiScCompareContextForm*)form;
    return ExKitFromCpStatus(CpCompareContext(&typed->is_equal, typed->first_context, typed->second_context));
}

AbiScStatus ExSysHdlAllocateVirtualRegion(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScAllocateVirtualRegionForm* typed = (AbiScAllocateVirtualRegionForm*)form;
    UInt64 address = typed->virtual_address;
    MmStatus status = MmAllocateVirtualRegion(&address, typed->page_count, typed->flags);
    typed->virtual_address = address;
    return ExKitFromMmStatus(status);
}

AbiScStatus ExSysHdlReleaseVirtualRegion(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScReleaseVirtualRegionForm* typed = (AbiScReleaseVirtualRegionForm*)form;
    return ExKitFromMmStatus(MmReleaseVirtualRegion(typed->virtual_address, typed->page_count));
}

AbiScStatus ExSysHdlMapVirtualRegion(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScMapVirtualRegionForm* typed = (AbiScMapVirtualRegionForm*)form;

    UInt32 pid = 0;
    if (PsProcessGetCurrentId(&pid) == PS_STATUS_SUCCESS)
    {
        PsProcess proc;
        if (PsProcessGet(&proc, pid) == PS_STATUS_SUCCESS && proc.page_table_address != 0)
        {
            UInt64 flags = typed->flags | MM_VIRTUAL_MEMORY_FLAG_USER;
            return ExKitFromMmStatus(MmMapVirtualRegionInSpace(proc.page_table_address, typed->virtual_address, typed->physical_address, typed->page_count, flags));
        }
    }

    return ExKitFromMmStatus(MmMapVirtualRegion(typed->virtual_address, typed->physical_address, typed->page_count, typed->flags));
}

AbiScStatus ExSysHdlUnmapVirtualRegion(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScUnmapVirtualRegionForm* typed = (AbiScUnmapVirtualRegionForm*)form;

    UInt32 pid = 0;
    if (PsProcessGetCurrentId(&pid) == PS_STATUS_SUCCESS)
    {
        PsProcess proc;
        if (PsProcessGet(&proc, pid) == PS_STATUS_SUCCESS && proc.page_table_address != 0)
        {
            return ExKitFromMmStatus(MmUnmapVirtualRegionInSpace(proc.page_table_address, typed->virtual_address, typed->page_count));
        }
    }

    return ExKitFromMmStatus(MmUnmapVirtualRegion(typed->virtual_address, typed->page_count));
}

AbiScStatus ExSysHdlGetPhysicalAddress(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetPhysicalAddressForm* typed = (AbiScGetPhysicalAddressForm*)form;

    UInt32 pid = 0;
    if (PsProcessGetCurrentId(&pid) == PS_STATUS_SUCCESS)
    {
        PsProcess proc;
        if (PsProcessGet(&proc, pid) == PS_STATUS_SUCCESS && proc.page_table_address != 0)
        {
            return ExKitFromMmStatus(MmGetPhysicalAddressInSpace(&typed->physical_address, proc.page_table_address, typed->virtual_address));
        }
    }

    return ExKitFromMmStatus(MmGetPhysicalAddress(&typed->physical_address, typed->virtual_address));
}

AbiScStatus ExSysHdlAllocateHeapBlock(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScAllocateHeapBlockForm* typed = (AbiScAllocateHeapBlockForm*)form;
    return ExKitFromMmStatus(MmAllocateHeapBlock(&typed->heap_address, typed->byte_count));
}

AbiScStatus ExSysHdlReallocateHeapBlock(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScReallocateHeapBlockForm* typed = (AbiScReallocateHeapBlockForm*)form;
    return ExKitFromMmStatus(MmReallocateHeapBlock(&typed->new_heap_address, typed->old_heap_address, typed->byte_count));
}

AbiScStatus ExSysHdlReleaseHeapBlock(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScReleaseHeapBlockForm* typed = (AbiScReleaseHeapBlockForm*)form;
    return ExKitFromMmStatus(MmReleaseHeapBlock(typed->heap_address));
}

AbiScStatus ExSysHdlCopyPhysicalMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyPhysicalMemoryForm* typed = (AbiScCopyPhysicalMemoryForm*)form;
    return ExKitFromMmStatus(MmCopyPhysicalMemory(typed->destination_address, typed->source_address, typed->byte_count));
}

AbiScStatus ExSysHdlFillPhysicalMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFillPhysicalMemoryForm* typed = (AbiScFillPhysicalMemoryForm*)form;
    return ExKitFromMmStatus(MmFillPhysicalMemory(typed->destination_address, typed->byte_value, typed->byte_count));
}

AbiScStatus ExSysHdlZeroPhysicalMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroPhysicalMemoryForm* typed = (AbiScZeroPhysicalMemoryForm*)form;
    return ExKitFromMmStatus(MmZeroPhysicalMemory(typed->destination_address, typed->byte_count));
}

AbiScStatus ExSysHdlComparePhysicalMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScComparePhysicalMemoryForm* typed = (AbiScComparePhysicalMemoryForm*)form;
    return ExKitFromMmStatus(MmComparePhysicalMemory(&typed->is_equal, typed->first_address, typed->second_address, typed->byte_count));
}

AbiScStatus ExSysHdlCopyVirtualMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyVirtualMemoryForm* typed = (AbiScCopyVirtualMemoryForm*)form;
    return ExKitFromMmStatus(MmCopyVirtualMemory(typed->destination_address, typed->source_address, typed->byte_count));
}

AbiScStatus ExSysHdlFillVirtualMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFillVirtualMemoryForm* typed = (AbiScFillVirtualMemoryForm*)form;
    return ExKitFromMmStatus(MmFillVirtualMemory(typed->destination_address, typed->byte_value, typed->byte_count));
}

AbiScStatus ExSysHdlZeroVirtualMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroVirtualMemoryForm* typed = (AbiScZeroVirtualMemoryForm*)form;
    return ExKitFromMmStatus(MmZeroVirtualMemory(typed->destination_address, typed->byte_count));
}

AbiScStatus ExSysHdlCompareVirtualMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCompareVirtualMemoryForm* typed = (AbiScCompareVirtualMemoryForm*)form;
    return ExKitFromMmStatus(MmCompareVirtualMemory(&typed->is_equal, typed->first_address, typed->second_address, typed->byte_count));
}

AbiScStatus ExSysHdlCopyHeapMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCopyHeapMemoryForm* typed = (AbiScCopyHeapMemoryForm*)form;
    return ExKitFromMmStatus(MmCopyHeapMemory(typed->destination_address, typed->source_address, typed->byte_count));
}

AbiScStatus ExSysHdlFillHeapMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFillHeapMemoryForm* typed = (AbiScFillHeapMemoryForm*)form;
    return ExKitFromMmStatus(MmFillHeapMemory(typed->destination_address, typed->byte_value, typed->byte_count));
}

AbiScStatus ExSysHdlZeroHeapMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScZeroHeapMemoryForm* typed = (AbiScZeroHeapMemoryForm*)form;
    return ExKitFromMmStatus(MmZeroHeapMemory(typed->destination_address, typed->byte_count));
}

AbiScStatus ExSysHdlCompareHeapMemory(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCompareHeapMemoryForm* typed = (AbiScCompareHeapMemoryForm*)form;
    return ExKitFromMmStatus(MmCompareHeapMemory(&typed->is_equal, typed->first_address, typed->second_address, typed->byte_count));
}

AbiScStatus ExSysHdlCreateAddressSpace(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCreateAddressSpaceForm* typed = (AbiScCreateAddressSpaceForm*)form;
    return ExKitFromMmStatus(MmCreateAddressSpace(&typed->page_table_address));
}

AbiScStatus ExSysHdlDestroyAddressSpace(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScDestroyAddressSpaceForm* typed = (AbiScDestroyAddressSpaceForm*)form;
    return ExKitFromMmStatus(MmDestroyAddressSpace(typed->page_table_address));
}

AbiScStatus ExSysHdlMapVirtualRegionInSpace(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScMapVirtualRegionInSpaceForm* typed = (AbiScMapVirtualRegionInSpaceForm*)form;
    return ExKitFromMmStatus(MmMapVirtualRegionInSpace(typed->page_table_address, typed->virtual_address, typed->physical_address, typed->page_count, typed->flags));
}

AbiScStatus ExSysHdlUnmapVirtualRegionInSpace(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScUnmapVirtualRegionInSpaceForm* typed = (AbiScUnmapVirtualRegionInSpaceForm*)form;
    return ExKitFromMmStatus(MmUnmapVirtualRegionInSpace(typed->page_table_address, typed->virtual_address, typed->page_count));
}

AbiScStatus ExSysHdlGetPhysicalAddressInSpace(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetPhysicalAddressInSpaceForm* typed = (AbiScGetPhysicalAddressInSpaceForm*)form;
    return ExKitFromMmStatus(MmGetPhysicalAddressInSpace(&typed->physical_address, typed->page_table_address, typed->virtual_address));
}

AbiScStatus ExSysHdlIn8(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScIn8Form* typed = (AbiScIn8Form*)form;
    typed->value = HwIn8(typed->port);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlOut8(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScOut8Form* typed = (AbiScOut8Form*)form;
    HwOut8(typed->port, typed->data);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlIn16(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScIn16Form* typed = (AbiScIn16Form*)form;
    typed->value = HwIn16(typed->port);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlOut16(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScOut16Form* typed = (AbiScOut16Form*)form;
    HwOut16(typed->port, typed->data);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlIn32(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScIn32Form* typed = (AbiScIn32Form*)form;
    typed->value = HwIn32(typed->port);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlOut32(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScOut32Form* typed = (AbiScOut32Form*)form;
    HwOut32(typed->port, typed->data);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlMaskIrq(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScMaskIrqForm* typed = (AbiScMaskIrqForm*)form;
    HwMaskIrq(typed->irq);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlUnmaskIrq(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScUnmaskIrqForm* typed = (AbiScUnmaskIrqForm*)form;
    HwUnmaskIrq(typed->irq);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlSendEoi(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSendEoiForm* typed = (AbiScSendEoiForm*)form;
    HwSendEoi(typed->irq);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlSetTimerFrequency(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSetTimerFrequencyForm* typed = (AbiScSetTimerFrequencyForm*)form;
    HwSetTimerFrequency(typed->frequency);
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetTimerFrequency(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetTimerFrequencyForm* typed = (AbiScGetTimerFrequencyForm*)form;
    typed->frequency = HwGetTimerFrequency();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetTimerTicks(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetTimerTicksForm* typed = (AbiScGetTimerTicksForm*)form;
    typed->ticks = HwGetTimerTicks();
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlReadPciConfig32(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadPciConfig32Form* typed = (AbiScReadPciConfig32Form*)form;
    return ExKitFromHwStatus(HwReadPciConfig32(&typed->value, typed->bus, typed->slot, typed->function, typed->offset));
}

AbiScStatus ExSysHdlWritePciConfig32(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScWritePciConfig32Form* typed = (AbiScWritePciConfig32Form*)form;
    return ExKitFromHwStatus(HwWritePciConfig32(typed->bus, typed->slot, typed->function, typed->offset, typed->value));
}

AbiScStatus ExSysHdlGetDeviceCount(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetDeviceCountForm* typed = (AbiScGetDeviceCountForm*)form;
    return ExKitFromHwStatus(HwGetDeviceCount(&typed->count));
}

AbiScStatus ExSysHdlGetDevice(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetDeviceForm* typed = (AbiScGetDeviceForm*)form;
    return ExKitFromHwStatus(HwGetDevice(&typed->device, typed->index));
}

AbiScStatus ExSysHdlRouteAcpiIrq(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScRouteAcpiIrqForm* typed = (AbiScRouteAcpiIrqForm*)form;
    return ExKitFromHwStatus(HwRouteAcpiIrq(typed->irq_source, typed->vector, typed->target_apic_id, typed->masked));
}

AbiScStatus ExSysHdlReadPciConfig8(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadPciConfig8Form* typed = (AbiScReadPciConfig8Form*)form;
    return ExKitFromHwStatus(HwReadPciConfig8(&typed->value, typed->bus, typed->slot, typed->function, typed->offset));
}

AbiScStatus ExSysHdlWritePciConfig8(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScWritePciConfig8Form* typed = (AbiScWritePciConfig8Form*)form;
    return ExKitFromHwStatus(HwWritePciConfig8(typed->bus, typed->slot, typed->function, typed->offset, typed->value));
}

AbiScStatus ExSysHdlReadPciConfig16(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadPciConfig16Form* typed = (AbiScReadPciConfig16Form*)form;
    return ExKitFromHwStatus(HwReadPciConfig16(&typed->value, typed->bus, typed->slot, typed->function, typed->offset));
}

AbiScStatus ExSysHdlWritePciConfig16(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScWritePciConfig16Form* typed = (AbiScWritePciConfig16Form*)form;
    return ExKitFromHwStatus(HwWritePciConfig16(typed->bus, typed->slot, typed->function, typed->offset, typed->value));
}

AbiScStatus ExSysHdlEnablePciDevice(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScEnablePciDeviceForm* typed = (AbiScEnablePciDeviceForm*)form;
    return ExKitFromHwStatus(HwEnablePciDevice(typed->bus, typed->slot, typed->function));
}

AbiScStatus ExSysHdlFindPciCapability(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindPciCapabilityForm* typed = (AbiScFindPciCapabilityForm*)form;
    return ExKitFromHwStatus(HwFindPciCapability(&typed->cap_offset, typed->bus, typed->slot, typed->function, typed->cap_id));
}

AbiScStatus ExSysHdlEnablePciMsi(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScEnablePciMsiForm* typed = (AbiScEnablePciMsiForm*)form;
    return ExKitFromHwStatus(HwEnablePciMsi(typed->bus, typed->slot, typed->function, typed->vector, typed->target_apic_id));
}

AbiScStatus ExSysHdlFindDeviceByClass(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDeviceByClassForm* typed = (AbiScFindDeviceByClassForm*)form;
    return ExKitFromHwStatus(HwFindDeviceByClass(&typed->device, typed->device_class, typed->occurrence));
}

AbiScStatus ExSysHdlFindDeviceById(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDeviceByIdForm* typed = (AbiScFindDeviceByIdForm*)form;
    return ExKitFromHwStatus(HwFindDeviceById(&typed->device, typed->vendor_id, typed->device_id, typed->occurrence));
}

AbiScStatus ExSysHdlFindDeviceByLocation(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDeviceByLocationForm* typed = (AbiScFindDeviceByLocationForm*)form;
    return ExKitFromHwStatus(HwFindDeviceByLocation(&typed->device, typed->bus, typed->slot, typed->function));
}

AbiScStatus ExSysHdlFindDeviceResource(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDeviceResourceForm* typed = (AbiScFindDeviceResourceForm*)form;
    return ExKitFromHwStatus(HwFindDeviceResource(&typed->resource, &typed->device, typed->resource_type, typed->occurrence));
}

AbiScStatus ExSysHdlSpinlockCreate(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSpinlockCreateForm* typed = (AbiScSpinlockCreateForm*)form;
    return ExKitFromPsStatus(PsSpinlockCreate(&typed->lock_id));
}

AbiScStatus ExSysHdlSpinlockAcquire(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSpinlockAcquireForm* typed = (AbiScSpinlockAcquireForm*)form;
    return ExKitFromPsStatus(PsSpinlockAcquire(typed->lock_id));
}

AbiScStatus ExSysHdlSpinlockRelease(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSpinlockReleaseForm* typed = (AbiScSpinlockReleaseForm*)form;
    return ExKitFromPsStatus(PsSpinlockRelease(typed->lock_id));
}

AbiScStatus ExSysHdlSpinlockDestroy(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSpinlockDestroyForm* typed = (AbiScSpinlockDestroyForm*)form;
    return ExKitFromPsStatus(PsSpinlockDestroy(typed->lock_id));
}

AbiScStatus ExSysHdlMutexCreate(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScMutexCreateForm* typed = (AbiScMutexCreateForm*)form;
    return ExKitFromPsStatus(PsMutexCreate(&typed->mutex_id));
}

AbiScStatus ExSysHdlMutexAcquire(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScMutexAcquireForm* typed = (AbiScMutexAcquireForm*)form;
    return ExKitFromPsStatus(PsMutexAcquire(typed->mutex_id));
}

AbiScStatus ExSysHdlMutexRelease(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScMutexReleaseForm* typed = (AbiScMutexReleaseForm*)form;
    return ExKitFromPsStatus(PsMutexRelease(typed->mutex_id));
}

AbiScStatus ExSysHdlMutexDestroy(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScMutexDestroyForm* typed = (AbiScMutexDestroyForm*)form;
    return ExKitFromPsStatus(PsMutexDestroy(typed->mutex_id));
}

AbiScStatus ExSysHdlGetLock(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetLockForm* typed = (AbiScGetLockForm*)form;
    return ExKitFromPsStatus(PsGetLock(&typed->lock, typed->lock_id));
}

AbiScStatus ExSysHdlProcessCreate(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScProcessCreateForm* typed = (AbiScProcessCreateForm*)form;
    return ExKitFromPsStatus(PsProcessCreate(&typed->process_id, typed->privilege, typed->priority));
}

AbiScStatus ExSysHdlProcessDestroy(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScProcessDestroyForm* typed = (AbiScProcessDestroyForm*)form;
    return ExKitFromPsStatus(PsProcessDestroy(typed->process_id));
}

AbiScStatus ExSysHdlProcessGet(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScProcessGetForm* typed = (AbiScProcessGetForm*)form;
    return ExKitFromPsStatus(PsProcessGet(&typed->process, typed->process_id));
}

AbiScStatus ExSysHdlProcessGetCurrentId(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScProcessGetCurrentIdForm* typed = (AbiScProcessGetCurrentIdForm*)form;
    return ExKitFromPsStatus(PsProcessGetCurrentId(&typed->process_id));
}

AbiScStatus ExSysHdlThreadCreate(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadCreateForm* typed = (AbiScThreadCreateForm*)form;
    return ExKitFromPsStatus(PsThreadCreate(&typed->thread_id, typed->process_id, typed->entry, typed->argument, typed->priority));
}

AbiScStatus ExSysHdlThreadDestroy(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadDestroyForm* typed = (AbiScThreadDestroyForm*)form;
    return ExKitFromPsStatus(PsThreadDestroy(typed->thread_id));
}

AbiScStatus ExSysHdlThreadGet(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadGetForm* typed = (AbiScThreadGetForm*)form;
    return ExKitFromPsStatus(PsThreadGet(&typed->thread, typed->thread_id));
}

AbiScStatus ExSysHdlThreadGetCurrentId(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadGetCurrentIdForm* typed = (AbiScThreadGetCurrentIdForm*)form;
    return ExKitFromPsStatus(PsThreadGetCurrentId(&typed->thread_id));
}

AbiScStatus ExSysHdlThreadGetContext(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadGetContextForm* typed = (AbiScThreadGetContextForm*)form;
    return ExKitFromPsStatus(PsThreadGetContext(&typed->context, typed->thread_id));
}

AbiScStatus ExSysHdlThreadSetContext(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScThreadSetContextForm* typed = (AbiScThreadSetContextForm*)form;
    return ExKitFromPsStatus(PsThreadSetContext(typed->thread_id, &typed->context));
}

AbiScStatus ExSysHdlYield(void* form, PsProcessPrivilege caller_privilege)
{
    (void)form;
    (void)caller_privilege;
    return ExKitFromPsStatus(PsYield());
}

AbiScStatus ExSysHdlSleep(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSleepForm* typed = (AbiScSleepForm*)form;
    return ExKitFromPsStatus(PsSleep(typed->ticks));
}

AbiScStatus ExSysHdlBlock(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScBlockForm* typed = (AbiScBlockForm*)form;
    return ExKitFromPsStatus(PsBlock(typed->thread_id));
}

AbiScStatus ExSysHdlUnblock(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScUnblockForm* typed = (AbiScUnblockForm*)form;
    return ExKitFromPsStatus(PsUnblock(typed->thread_id));
}

AbiScStatus ExSysHdlRegisterDriver(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScRegisterDriverForm* typed = (AbiScRegisterDriverForm*)form;
    return ExKitFromFsStatus(FsRegisterDriver(&typed->driver_id, &typed->driver));
}

AbiScStatus ExSysHdlUnregisterDriver(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScUnregisterDriverForm* typed = (AbiScUnregisterDriverForm*)form;
    return ExKitFromFsStatus(FsUnregisterDriver(typed->driver_id));
}

AbiScStatus ExSysHdlGetDriver(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetDriverForm* typed = (AbiScGetDriverForm*)form;
    return ExKitFromFsStatus(FsGetDriver(&typed->driver, typed->driver_id));
}

AbiScStatus ExSysHdlFindDriverByType(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDriverByTypeForm* typed = (AbiScFindDriverByTypeForm*)form;
    return ExKitFromFsStatus(FsFindDriverByType(&typed->driver, typed->type));
}

AbiScStatus ExSysHdlDriverRead(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScDriverReadForm* typed = (AbiScDriverReadForm*)form;
    return ExKitFromFsStatus(FsDriverRead(typed->driver_id, typed->device_id, typed->offset, typed->buffer, typed->byte_count, &typed->bytes_read));
}

AbiScStatus ExSysHdlDriverWrite(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScDriverWriteForm* typed = (AbiScDriverWriteForm*)form;
    return ExKitFromFsStatus(FsDriverWrite(typed->driver_id, typed->device_id, typed->offset, typed->buffer, typed->byte_count, &typed->bytes_written));
}

AbiScStatus ExSysHdlDriverControl(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScDriverControlForm* typed = (AbiScDriverControlForm*)form;
    return ExKitFromFsStatus(FsDriverControl(typed->driver_id, typed->device_id, typed->control_code, typed->argument));
}

AbiScStatus ExSysHdlMountVolume(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScMountVolumeForm* typed = (AbiScMountVolumeForm*)form;
    return ExKitFromFsStatus(FsMountVolume(&typed->volume_id, typed->driver_id, typed->type, typed->mount_path));
}

AbiScStatus ExSysHdlUnmountVolume(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScUnmountVolumeForm* typed = (AbiScUnmountVolumeForm*)form;
    return ExKitFromFsStatus(FsUnmountVolume(typed->volume_id));
}

AbiScStatus ExSysHdlGetVolume(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetVolumeForm* typed = (AbiScGetVolumeForm*)form;
    return ExKitFromFsStatus(FsGetVolume(&typed->volume, typed->volume_id));
}

AbiScStatus ExSysHdlFindVolumeByPath(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindVolumeByPathForm* typed = (AbiScFindVolumeByPathForm*)form;
    return ExKitFromFsStatus(FsFindVolumeByPath(&typed->volume, typed->path));
}

AbiScStatus ExSysHdlCreateNode(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCreateNodeForm* typed = (AbiScCreateNodeForm*)form;
    return ExKitFromFsStatus(FsCreateNode(&typed->node_id, typed->parent_id, typed->name, typed->type, typed->flags));
}

AbiScStatus ExSysHdlDestroyNode(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScDestroyNodeForm* typed = (AbiScDestroyNodeForm*)form;
    return ExKitFromFsStatus(FsDestroyNode(typed->node_id));
}

AbiScStatus ExSysHdlGetNode(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetNodeForm* typed = (AbiScGetNodeForm*)form;
    return ExKitFromFsStatus(FsGetNode(&typed->node, typed->node_id));
}

AbiScStatus ExSysHdlResolvePath(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScResolvePathForm* typed = (AbiScResolvePathForm*)form;
    return ExKitFromFsStatus(FsResolvePath(&typed->node_id, typed->path));
}

AbiScStatus ExSysHdlGetNodeByPath(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetNodeByPathForm* typed = (AbiScGetNodeByPathForm*)form;
    return ExKitFromFsStatus(FsGetNodeByPath(&typed->node, typed->path));
}

AbiScStatus ExSysHdlBindNodeBuffer(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScBindNodeBufferForm* typed = (AbiScBindNodeBufferForm*)form;
    return ExKitFromFsStatus(FsBindNodeBuffer(typed->node_id, typed->buffer, typed->size));
}

AbiScStatus ExSysHdlReadNodeBuffer(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadNodeBufferForm* typed = (AbiScReadNodeBufferForm*)form;
    return ExKitFromFsStatus(FsReadNodeBuffer(typed->node_id, typed->buffer, typed->buffer_capacity, &typed->bytes_read));
}

AbiScStatus ExSysHdlWriteNodeBuffer(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScWriteNodeBufferForm* typed = (AbiScWriteNodeBufferForm*)form;
    return ExKitFromFsStatus(FsWriteNodeBuffer(typed->node_id, typed->buffer, typed->size));
}

AbiScStatus ExSysHdlOpenFile(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScOpenFileForm* typed = (AbiScOpenFileForm*)form;
    return ExKitFromFsStatus(FsOpenFile(&typed->file_id, typed->path, typed->mode));
}

AbiScStatus ExSysHdlOpenFileByNode(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScOpenFileByNodeForm* typed = (AbiScOpenFileByNodeForm*)form;
    return ExKitFromFsStatus(FsOpenFileByNode(&typed->file_id, typed->node_id, typed->mode));
}

AbiScStatus ExSysHdlCloseFile(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScCloseFileForm* typed = (AbiScCloseFileForm*)form;
    return ExKitFromFsStatus(FsCloseFile(typed->file_id));
}

AbiScStatus ExSysHdlReadFile(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadFileForm* typed = (AbiScReadFileForm*)form;
    return ExKitFromFsStatus(FsReadFile(typed->file_id, typed->buffer, typed->byte_count, &typed->bytes_read));
}

AbiScStatus ExSysHdlWriteFile(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScWriteFileForm* typed = (AbiScWriteFileForm*)form;
    return ExKitFromFsStatus(FsWriteFile(typed->file_id, typed->buffer, typed->byte_count, &typed->bytes_written));
}

AbiScStatus ExSysHdlSeekFile(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScSeekFileForm* typed = (AbiScSeekFileForm*)form;
    return ExKitFromFsStatus(FsSeekFile(typed->file_id, typed->offset, typed->origin, &typed->new_offset));
}

AbiScStatus ExSysHdlGetFile(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetFileForm* typed = (AbiScGetFileForm*)form;
    return ExKitFromFsStatus(FsGetFile(&typed->file, typed->file_id));
}

AbiScStatus ExSysHdlGetSyscallCount(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetSyscallCountForm* typed = (AbiScGetSyscallCountForm*)form;
    typed->count = ABI_SC_COUNT;
    return ABI_SC_STATUS_SUCCESS;
}

AbiScStatus ExSysHdlGetSyscallDescriptor(void* form, PsProcessPrivilege caller_privilege)
{
    (void)caller_privilege;
    if (form == NULL)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetSyscallDescriptorForm* typed = (AbiScGetSyscallDescriptorForm*)form;
    if (typed->syscall_id >= ABI_SC_COUNT)
    {
        return ABI_SC_STATUS_INVALID_ARGUMENT;
    }

    const ExSyscallDescriptor* desc = &s_sys_ledger.dispatch_table[typed->syscall_id];
    typed->min_privilege = desc->min_privilege;
    typed->form_size     = desc->form_size;

    const char* src = desc->name;
    if (src != NULL)
    {
        UInt32 idx = 0;
        while (src[idx] != '\0' && idx < 31)
        {
            typed->name[idx] = src[idx];
            idx++;
        }
        typed->name[idx] = '\0';
    }
    else
    {
        typed->name[0] = '\0';
    }

    return ABI_SC_STATUS_SUCCESS;
}
