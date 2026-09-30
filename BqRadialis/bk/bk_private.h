// Made by Berkay

#ifndef BK_PRIVATE_H
#define BK_PRIVATE_H

#include "bk_public.h"

#include "bk_rbp.h"

#include "../cp/cp_public.h"
#include "../mm/mm_public.h"
#include "../hw/hw_public.h"
#include "../ps/ps_public.h"
#include "../fs/fs_public.h"
#include "../ex/ex_public.h"

typedef struct BkRbpKernelLayout BkRbpKernelLayout;

typedef enum   BkRbpMemoryMapHardwareType BkRbpMemoryMapHardwareType;
typedef enum   BkRbpMemoryMapAcpiAttribute BkRbpMemoryMapAcpiAttribute;
typedef struct BkRbpMemoryMapEntry BkRbpMemoryMapEntry;
typedef struct BkRbpMemoryMap BkRbpMemoryMap;

typedef enum   BkRbpAcpiMadtRecordType BkRbpAcpiMadtRecordType;
typedef struct BkRbpAcpiSdtHeader BkRbpAcpiSdtHeader;
typedef struct BkRbpAcpiGas BkRbpAcpiGas;
typedef struct BkRbpAcpiMadtRecordHeader BkRbpAcpiMadtRecordHeader;
typedef struct BkRbpAcpiMadtLocalApic BkRbpAcpiMadtLocalApic;
typedef struct BkRbpAcpiMadtIoApic BkRbpAcpiMadtIoApic;
typedef struct BkRbpAcpiMadtIrqOverride BkRbpAcpiMadtIrqOverride;
typedef struct BkRbpAcpiMadtLocalApicOverride BkRbpAcpiMadtLocalApicOverride;
typedef struct BkRbpAcpiMadt BkRbpAcpiMadt;
typedef struct BkRbpAcpiFadt BkRbpAcpiFadt;
typedef struct BkRbpAcpiHpet BkRbpAcpiHpet;
typedef struct BkRbpAcpiMcfgAllocation BkRbpAcpiMcfgAllocation;
typedef struct BkRbpAcpiMcfg BkRbpAcpiMcfg;
typedef struct BkRbpAcpiRoot BkRbpAcpiRoot;

typedef struct BkRbpFramebuffer BkRbpFramebuffer;

typedef struct BkRbpModule BkRbpModule;
typedef struct BkRbpBootInfo BkRbpBootInfo;

typedef struct BkRbpLedger BkRbpLedger;

void BkRbpLoad(BkRbpBootInfo* boot_info);

BkRbpKernelLayout BkRbpGetKernelLayout(void);

BkRbpMemoryMap    BkRbpGetMemoryMap(void);

BkRbpAcpiRoot     BkRbpGetAcpiRoot(void);
BkRbpAcpiMadt*    BkRbpGetAcpiMadt(void);
BkRbpAcpiFadt*    BkRbpGetAcpiFadt(void);
BkRbpAcpiHpet*    BkRbpGetAcpiHpet(void);
BkRbpAcpiMcfg*    BkRbpGetAcpiMcfg(void);

BkRbpFramebuffer  BkRbpGetFramebuffer(void);

BkRbpBootInfo     BkRbpGetBootInfo(void);

#endif
