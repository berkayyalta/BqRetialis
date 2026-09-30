// Made by Berkay

#ifndef HW_PRIVATE_H
#define HW_PRIVATE_H

#include "hw_public.h"

#include "hw_madt.h"
#include "hw_mcfg.h"
#include "hw_framebuffer.h"
#include "hw_io.h"
#include "hw_pic.h"
#include "hw_pit.h"
#include "hw_pci.h"
#include "hw_dev.h"

#include "../bk/bk_public.h"
#include "../cp/cp_public.h"
#include "../mm/mm_public.h"

typedef enum   HwMadtRecordType HwMadtRecordType;
typedef struct HwMadtSdtHeader HwMadtSdtHeader;
typedef struct HwMadtRecordHeader HwMadtRecordHeader;
typedef struct HwMadtIoApic HwMadtIoApic;
typedef struct HwMadtIrqOverride HwMadtIrqOverride;
typedef struct HwMadt HwMadt;

typedef struct HwMcfgSdtHeader HwMcfgSdtHeader;
typedef struct HwMcfgAllocation HwMcfgAllocation;
typedef struct HwMcfg HwMcfg;

typedef struct HwFramebuffer HwFramebuffer;

typedef struct HwPitLedger HwPitLedger;

typedef enum    HwPciConfigOffset HwPciConfigOffset;
typedef enum    HwPciCommandFlag HwPciCommandFlag;
typedef enum    HwPciBarType HwPciBarType;
typedef struct  HwPciBar HwPciBar;
typedef struct  HwPciDevice HwPciDevice;
typedef struct  HwPciLedger HwPciLedger;

typedef enum    HwDevBusType HwDevBusType;
typedef enum    HwDevClass HwDevClass;
typedef enum    HwDevResourceType HwDevResourceType;
typedef struct  HwDevResource HwDevResource;
typedef struct  HwDevDisplayInfo HwDevDisplayInfo;
typedef struct  HwDevDevice HwDevDevice;
typedef struct  HwDevLedger HwDevLedger;

UInt8  HwIoIn8(UInt16 port);
void   HwIoOut8(UInt16 port, UInt8 data);
UInt16 HwIoIn16(UInt16 port);
void   HwIoOut16(UInt16 port, UInt16 data);
UInt32 HwIoIn32(UInt16 port);
void   HwIoOut32(UInt16 port, UInt32 data);
void   HwIoWait(void);

void   HwPicLoad(void);
void   HwPicMaskIrq(UInt8 irq);
void   HwPicUnmaskIrq(UInt8 irq);
void   HwPicSendEoi(UInt8 irq);

void   HwPitLoad(void);
void   HwPitSetFrequency(UInt32 frequency);
UInt32 HwPitGetFrequency(void);
void   HwPitIncrementTicks(void);
UInt64 HwPitGetTicks(void);

HwStatus HwPciLedgerAllocateSpace(void);
HwStatus HwPciLedgerPopulate(void);
HwStatus HwPciLedgerLoad(void);

HwStatus HwPciReadConfig32(UInt32* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset);
HwStatus HwPciWriteConfig32(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt32 value);
HwStatus HwPciGetDeviceCount(UInt32* count);
HwStatus HwPciGetDevice(HwPciDevice* device, UInt32 index);

HwStatus HwPciScanBus(UInt8 bus);
HwStatus HwPciProbeFunction(UInt8 bus, UInt8 slot, UInt8 function);
HwStatus HwPciDecodeBars(HwPciDevice* device);

HwStatus HwDevLedgerAllocateSpace(void);
HwStatus HwDevLedgerPopulate(HwFramebuffer* framebuffer);
HwStatus HwDevLedgerLoad(void);

HwStatus HwDevGetDeviceCount(UInt32* count);
HwStatus HwDevGetDevice(HwDevDevice* device, UInt32 index);

HwStatus HwDevRegisterPlatformDevices(void);
HwStatus HwDevRegisterFramebuffer(HwFramebuffer* framebuffer);
HwStatus HwDevRegisterPciDevices(void);
HwStatus HwDevTranslatePciClass(HwDevClass* dev_class, UInt8 class_code, UInt8 subclass);

HwStatus HwKitMapAcpiMmio(void);
HwStatus HwKitResolveAcpiIrq(UInt32* gsi, UInt16* flags, UInt8 irq_source);
HwStatus HwKitFindAcpiIoApicForGsi(UInt64* physical_base, UInt32* gsi_base, UInt8* io_apic_id, UInt32 gsi);
HwStatus HwKitRouteAcpiIrq(UInt8 irq_source, UInt8 vector, UInt8 target_apic_id, UInt8 masked);

HwStatus HwKitReadPciConfig8(UInt8* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset);
HwStatus HwKitWritePciConfig8(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt8 value);
HwStatus HwKitReadPciConfig16(UInt16* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset);
HwStatus HwKitWritePciConfig16(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt16 value);
HwStatus HwKitEnablePciDevice(UInt8 bus, UInt8 slot, UInt8 function);
HwStatus HwKitFindPciCapability(UInt8* cap_offset, UInt8 bus, UInt8 slot, UInt8 function, UInt8 cap_id);
HwStatus HwKitEnablePciMsi(UInt8 bus, UInt8 slot, UInt8 function, UInt8 vector, UInt8 target_apic_id);

HwStatus HwKitFindDeviceByClass(HwDevDevice* device, HwDevClass device_class, UInt32 occurrence);
HwStatus HwKitFindDeviceById(HwDevDevice* device, UInt16 vendor_id, UInt16 device_id, UInt32 occurrence);
HwStatus HwKitFindDeviceByLocation(HwDevDevice* device, UInt8 bus, UInt8 slot, UInt8 function);
HwStatus HwKitFindDeviceResource(HwDevResource* resource, HwDevDevice* device, HwDevResourceType resource_type, UInt32 occurrence);

#endif
