// Made by Berkay

#ifndef HW_PUBLIC_H
#define HW_PUBLIC_H

#include "hw_status.h"
#include "hw_device.h"

typedef enum HwStatus HwStatus;

typedef enum    HwDeviceBusType HwDeviceBusType;
typedef enum    HwDeviceClass HwDeviceClass;
typedef enum    HwDeviceResourceType HwDeviceResourceType;
typedef struct  HwDeviceResource HwDeviceResource;
typedef struct  HwDeviceDisplayInfo HwDeviceDisplayInfo;
typedef struct  HwDevice HwDevice;

void HwLoad(void);

HwStatus HwInit(void);

UInt8  HwIn8(UInt16 port);
void   HwOut8(UInt16 port, UInt8 data);
UInt16 HwIn16(UInt16 port);
void   HwOut16(UInt16 port, UInt16 data);
UInt32 HwIn32(UInt16 port);
void   HwOut32(UInt16 port, UInt32 data);

void HwMaskIrq(UInt8 irq);
void HwUnmaskIrq(UInt8 irq);
void HwSendEoi(UInt8 irq);

void   HwSetTimerFrequency(UInt32 frequency);
UInt32 HwGetTimerFrequency(void);
void   HwIncrementTimerTicks(void);
UInt64 HwGetTimerTicks(void);

HwStatus HwReadPciConfig32(UInt32* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset);
HwStatus HwWritePciConfig32(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt32 value);

HwStatus HwGetDeviceCount(UInt32* count);
HwStatus HwGetDevice(HwDevice* device, UInt32 index);

HwStatus HwRouteAcpiIrq(UInt8 irq_source, UInt8 vector, UInt8 target_apic_id, UInt8 masked);

HwStatus HwReadPciConfig8(UInt8* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset);
HwStatus HwWritePciConfig8(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt8 value);
HwStatus HwReadPciConfig16(UInt16* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset);
HwStatus HwWritePciConfig16(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt16 value);
HwStatus HwEnablePciDevice(UInt8 bus, UInt8 slot, UInt8 function);
HwStatus HwFindPciCapability(UInt8* cap_offset, UInt8 bus, UInt8 slot, UInt8 function, UInt8 cap_id);
HwStatus HwEnablePciMsi(UInt8 bus, UInt8 slot, UInt8 function, UInt8 vector, UInt8 target_apic_id);

HwStatus HwFindDeviceByClass(HwDevice* device, HwDeviceClass device_class, UInt32 occurrence);
HwStatus HwFindDeviceById(HwDevice* device, UInt16 vendor_id, UInt16 device_id, UInt32 occurrence);
HwStatus HwFindDeviceByLocation(HwDevice* device, UInt8 bus, UInt8 slot, UInt8 function);
HwStatus HwFindDeviceResource(HwDeviceResource* resource, HwDevice* device, HwDeviceResourceType resource_type, UInt32 occurrence);

#endif
