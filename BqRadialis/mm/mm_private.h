// Made by Berkay

#ifndef MM_PRIVATE_H
#define MM_PRIVATE_H

#include "mm_public.h"

#include "mm_kernel_layout.h"
#include "mm_memory_map.h"
#include "mm_framebuffer.h"
#include "mm_pm.h"
#include "mm_vm.h"
#include "mm_hp.h"

#include "../bk/bk_public.h"
#include "../cp/cp_public.h"

typedef struct MmKernelLayout MmKernelLayout;

typedef enum   MmMemoryMapHardwareType MmMemoryMapHardwareType;
typedef enum   MmMemoryMapAcpiAttribute MmMemoryMapAcpiAttribute;
typedef struct MmMemoryMapEntry MmMemoryMapEntry;
typedef struct MmMemoryMap MmMemoryMap;

typedef struct MmFramebuffer MmFramebuffer;

typedef struct MmPmLedger MmPmLedger;

typedef enum    MmVmFlags MmVmFlags;
typedef enum    MmVmMasks MmVmMasks;
typedef struct  MmVmPageTable MmVmPageTable;
typedef struct  MmVmFreeRegion MmVmFreeRegion;
typedef struct  MmVmLedger MmVmLedger;

typedef struct MmHpBlock MmHpBlock;
typedef struct MmHpLedger MmHpLedger;

void MmPmLedgerAllocateSpace(MmKernelLayout* layout, MmMemoryMap* map);
void MmPmLedgerPopulate(MmKernelLayout* layout, MmMemoryMap* map);
void MmPmLedgerLoad(void);

UInt64 MmPmAllocatePhysicalRegion(UInt64 highest_acceptable_physical_address, UInt64 page_count);
void MmPmReleasePhysicalRegion(UInt64 physical_address, UInt64 page_count);

void MmPmSetPageUsed(UInt64 page_index);
void MmPmSetPageFree(UInt64 page_index);
UInt8 MmPmGetPageStatus(UInt64 page_index);
void MmPmRelocateBitmapToHhdm(void);

MmStatus MmVmLedgerAllocateSpace(void);
MmStatus MmVmLedgerPopulate(MmKernelLayout* layout, MmMemoryMap* map);
MmStatus MmVmLedgerLoad(void);

MmStatus MmVmAllocateVirtualRegion(UInt64* virtual_address, UInt64 page_count, UInt64 flags);
MmStatus MmVmReleaseVirtualRegion(UInt64 virtual_address, UInt64 page_count);

MmStatus MmVmMapVirtualRegion(UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags);
MmStatus MmVmUnmapVirtualRegion(UInt64 virtual_address, UInt64 page_count);

MmStatus MmVmMapPage(UInt64 virtual_address, UInt64 physical_address, UInt64 flags);
MmStatus MmVmUnmapPage(UInt64 virtual_address);
MmStatus MmVmMapPageInSpace(UInt64 pml4_phys, UInt64 virtual_address, UInt64 physical_address, UInt64 flags);
MmStatus MmVmUnmapPageInSpace(UInt64 pml4_phys, UInt64 virtual_address);
MmStatus MmVmGetPhysicalAddress(UInt64* physical_address, UInt64 virtual_address);

MmStatus MmVmGetOrCreateTable(MmVmPageTable** next_table, MmVmPageTable* parent_table, UInt32 entry_index, UInt64 flags);
MmStatus MmVmInvalidatePage(UInt64 virtual_address);
MmStatus MmVmPhysicalToVirtual(void** virtual_address, UInt64 physical_address);
MmStatus MmVmIsTableEmpty(UInt8* is_empty, MmVmPageTable* table);

MmStatus MmVmCreateAddressSpace(UInt64* page_table_address);
MmStatus MmVmDestroyAddressSpace(UInt64 page_table_address);
MmStatus MmVmMapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags);
MmStatus MmVmUnmapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 page_count);
MmStatus MmVmGetPhysicalAddressInSpace(UInt64* physical_address, UInt64 page_table_address, UInt64 virtual_address);

MmStatus MmHpLedgerAllocateSpace(void);
MmStatus MmHpLedgerPopulate(void);
MmStatus MmHpLedgerLoad(void);

MmStatus MmHpAllocateBlock(void** heap_address, UInt64 byte_count);
MmStatus MmHpReallocateBlock(void** new_heap_address, void* old_heap_address, UInt64 byte_count);
MmStatus MmHpReleaseBlock(void* heap_address);

MmStatus MmHpExpandSpace(UInt64 byte_count);
MmStatus MmHpSplitBlock(MmHpBlock* block, UInt64 byte_count);
MmStatus MmHpMergeBlock(MmHpBlock* block);

MmStatus MmKitCopyPhysicalMemory(void* destination_address, void* source_address, UInt64 byte_count);
MmStatus MmKitFillPhysicalMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count);
MmStatus MmKitZeroPhysicalMemory(void* destination_address, UInt64 byte_count);
MmStatus MmKitComparePhysicalMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count);

MmStatus MmKitCopyVirtualMemory(void* destination_address, void* source_address, UInt64 byte_count);
MmStatus MmKitFillVirtualMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count);
MmStatus MmKitZeroVirtualMemory(void* destination_address, UInt64 byte_count);
MmStatus MmKitCompareVirtualMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count);

MmStatus MmKitCopyHeapMemory(void* destination_address, void* source_address, UInt64 byte_count);
MmStatus MmKitFillHeapMemory(void* destination_address, UInt8 byte_value, UInt64 byte_count);
MmStatus MmKitZeroHeapMemory(void* destination_address, UInt64 byte_count);
MmStatus MmKitCompareHeapMemory(UInt8* is_equal, void* first_address, void* second_address, UInt64 byte_count);

#endif
