// Made by Berkay

#ifndef SYS_HW_H
#define SYS_HW_H

#include "sys_core.h"

BqStatus BqIn8(UInt16 port, UInt8* value);
BqStatus BqIoRead8(UInt16 port, UInt8* value);
BqStatus BqOut8(UInt16 port, UInt8 data);
BqStatus BqIoWrite8(UInt16 port, UInt8 data);
BqStatus BqIn16(UInt16 port, UInt16* value);
BqStatus BqIoRead16(UInt16 port, UInt16* value);
BqStatus BqOut16(UInt16 port, UInt16 data);
BqStatus BqIoWrite16(UInt16 port, UInt16 data);
BqStatus BqIn32(UInt16 port, UInt32* value);
BqStatus BqIoRead32(UInt16 port, UInt32* value);
BqStatus BqOut32(UInt16 port, UInt32 data);
BqStatus BqIoWrite32(UInt16 port, UInt32 data);
BqStatus BqMaskIrq(UInt8 irq);
BqStatus BqUnmaskIrq(UInt8 irq);
BqStatus BqSendEoi(UInt8 irq);
BqStatus BqSetTimerFrequency(UInt32 frequency);
BqStatus BqGetTimerFrequency(UInt32* frequency);
BqStatus BqGetTimerTicks(UInt64* ticks);
BqStatus BqReadPciConfig32(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt32* value);
BqStatus BqWritePciConfig32(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt32 value);
BqStatus BqGetDeviceCount(UInt32* count);
BqStatus BqGetDevice(UInt32 index, HwDevice* device);
BqStatus BqRouteAcpiIrq(UInt8 irq_source, UInt8 vector, UInt8 target_apic_id, UInt8 masked);
BqStatus BqReadPciConfig8(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt8* value);
BqStatus BqWritePciConfig8(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt8 value);
BqStatus BqReadPciConfig16(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt16* value);
BqStatus BqWritePciConfig16(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt16 value);
BqStatus BqEnablePciDevice(UInt8 bus, UInt8 slot, UInt8 function);
BqStatus BqFindPciCapability(UInt8 bus, UInt8 slot, UInt8 function, UInt8 cap_id, UInt8* cap_offset);
BqStatus BqEnablePciMsi(UInt8 bus, UInt8 slot, UInt8 function, UInt8 vector, UInt8 target_apic_id);
BqStatus BqFindDeviceByClass(HwDeviceClass device_class, UInt32 occurrence, HwDevice* device);
BqStatus BqFindDeviceById(UInt16 vendor_id, UInt16 device_id, UInt32 occurrence, HwDevice* device);
BqStatus BqFindDeviceByLocation(UInt8 bus, UInt8 slot, UInt8 function, HwDevice* device);
BqStatus BqFindDeviceResource(HwDevice device, HwDeviceResourceType resource_type, UInt32 occurrence, HwDeviceResource* resource);

#endif
