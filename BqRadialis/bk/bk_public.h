// Made by Berkay

#ifndef BK_PUBLIC_H
#define BK_PUBLIC_H

#include "bk_status.h"
#include "bk_panic.h"
#include "bk_kernel_layout.h"
#include "bk_memory_map.h"
#include "bk_acpi.h"
#include "bk_framebuffer.h"
#include "bk_boot_info.h"

typedef enum   BkStatus BkStatus;

typedef struct BkPanicReport BkPanicReport;

typedef struct BkKernelLayout BkKernelLayout;

typedef enum   BkMemoryMapHardwareType BkMemoryMapHardwareType;
typedef enum   BkMemoryMapAcpiAttribute BkMemoryMapAcpiAttribute;
typedef struct BkMemoryMapEntry BkMemoryMapEntry;
typedef struct BkMemoryMap BkMemoryMap;

typedef enum   BkAcpiMadtRecordType BkAcpiMadtRecordType;
typedef struct BkAcpiSdtHeader BkAcpiSdtHeader;
typedef struct BkAcpiGas BkAcpiGas;
typedef struct BkAcpiMadtRecordHeader BkAcpiMadtRecordHeader;
typedef struct BkAcpiMadtLocalApic BkAcpiMadtLocalApic;
typedef struct BkAcpiMadtIoApic BkAcpiMadtIoApic;
typedef struct BkAcpiMadtIrqOverride BkAcpiMadtIrqOverride;
typedef struct BkAcpiMadtLocalApicOverride BkAcpiMadtLocalApicOverride;
typedef struct BkAcpiMadt BkAcpiMadt;
typedef struct BkAcpiFadt BkAcpiFadt;
typedef struct BkAcpiHpet BkAcpiHpet;
typedef struct BkAcpiMcfgAllocation BkAcpiMcfgAllocation;
typedef struct BkAcpiMcfg BkAcpiMcfg;
typedef struct BkAcpiRoot BkAcpiRoot;

typedef struct BkFramebuffer BkFramebuffer;

typedef struct BkBootModule BkBootModule;
typedef struct BkBootInfo BkBootInfo;

void BkLoad(BkBootInfo* boot_info);

BkStatus BkInit(void);

void BkPanic(BkPanicReport* report) __attribute__((noreturn));

BkKernelLayout BkGetKernelLayout(void);

BkMemoryMap    BkGetMemoryMap(void);

BkAcpiRoot     BkGetAcpiRoot(void);
BkAcpiMadt*    BkGetAcpiMadt(void);
BkAcpiFadt*    BkGetAcpiFadt(void);
BkAcpiHpet*    BkGetAcpiHpet(void);
BkAcpiMcfg*    BkGetAcpiMcfg(void);

BkFramebuffer  BkGetFramebuffer(void);

BkBootInfo     BkGetBootInfo(void);

#endif
