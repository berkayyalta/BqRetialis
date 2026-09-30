// Made by Berkay

#include "hw_private.h"

void HwLoad(void)
{
    HwPicLoad();
    HwPitLoad();
}

HwStatus HwInit(void)
{
    HwStatus status = HwKitMapAcpiMmio();
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    status = HwPciLedgerLoad();
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }
    return HwDevLedgerLoad();
}

UInt8 HwIn8(UInt16 port)
{
    return HwIoIn8(port);
}

void HwOut8(UInt16 port, UInt8 data)
{
    HwIoOut8(port, data);
}

UInt16 HwIn16(UInt16 port)
{
    return HwIoIn16(port);
}

void HwOut16(UInt16 port, UInt16 data)
{
    HwIoOut16(port, data);
}

UInt32 HwIn32(UInt16 port)
{
    return HwIoIn32(port);
}

void HwOut32(UInt16 port, UInt32 data)
{
    HwIoOut32(port, data);
}

void HwMaskIrq(UInt8 irq)
{
    HwPicMaskIrq(irq);
}

void HwUnmaskIrq(UInt8 irq)
{
    HwPicUnmaskIrq(irq);
}

void HwSendEoi(UInt8 irq)
{
    HwPicSendEoi(irq);
}

void HwSetTimerFrequency(UInt32 frequency)
{
    HwPitSetFrequency(frequency);
}

UInt32 HwGetTimerFrequency(void)
{
    return HwPitGetFrequency();
}

void HwIncrementTimerTicks(void)
{
    HwPitIncrementTicks();
}

UInt64 HwGetTimerTicks(void)
{
    return HwPitGetTicks();
}

HwStatus HwReadPciConfig32(UInt32* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset)
{
    return HwPciReadConfig32(value, bus, slot, function, offset);
}

HwStatus HwWritePciConfig32(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt32 value)
{
    return HwPciWriteConfig32(bus, slot, function, offset, value);
}

HwStatus HwGetDeviceCount(UInt32* count)
{
    return HwDevGetDeviceCount(count);
}

HwStatus HwGetDevice(HwDevice* device, UInt32 index)
{
    return HwDevGetDevice((HwDevDevice*)device, index);
}

HwStatus HwRouteAcpiIrq(UInt8 irq_source, UInt8 vector, UInt8 target_apic_id, UInt8 masked)
{
    return HwKitRouteAcpiIrq(irq_source, vector, target_apic_id, masked);
}

HwStatus HwReadPciConfig8(UInt8* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset)
{
    return HwKitReadPciConfig8(value, bus, slot, function, offset);
}

HwStatus HwWritePciConfig8(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt8 value)
{
    return HwKitWritePciConfig8(bus, slot, function, offset, value);
}

HwStatus HwReadPciConfig16(UInt16* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset)
{
    return HwKitReadPciConfig16(value, bus, slot, function, offset);
}

HwStatus HwWritePciConfig16(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt16 value)
{
    return HwKitWritePciConfig16(bus, slot, function, offset, value);
}

HwStatus HwEnablePciDevice(UInt8 bus, UInt8 slot, UInt8 function)
{
    return HwKitEnablePciDevice(bus, slot, function);
}

HwStatus HwFindPciCapability(UInt8* cap_offset, UInt8 bus, UInt8 slot, UInt8 function, UInt8 cap_id)
{
    return HwKitFindPciCapability(cap_offset, bus, slot, function, cap_id);
}

HwStatus HwEnablePciMsi(UInt8 bus, UInt8 slot, UInt8 function, UInt8 vector, UInt8 target_apic_id)
{
    return HwKitEnablePciMsi(bus, slot, function, vector, target_apic_id);
}

HwStatus HwFindDeviceByClass(HwDevice* device, HwDeviceClass device_class, UInt32 occurrence)
{
    return HwKitFindDeviceByClass((HwDevDevice*)device, (HwDevClass)device_class, occurrence);
}

HwStatus HwFindDeviceById(HwDevice* device, UInt16 vendor_id, UInt16 device_id, UInt32 occurrence)
{
    return HwKitFindDeviceById((HwDevDevice*)device, vendor_id, device_id, occurrence);
}

HwStatus HwFindDeviceByLocation(HwDevice* device, UInt8 bus, UInt8 slot, UInt8 function)
{
    return HwKitFindDeviceByLocation((HwDevDevice*)device, bus, slot, function);
}

HwStatus HwFindDeviceResource(HwDeviceResource* resource, HwDevice* device, HwDeviceResourceType resource_type, UInt32 occurrence)
{
    return HwKitFindDeviceResource((HwDevResource*)resource, (HwDevDevice*)device, (HwDevResourceType)resource_type, occurrence);
}
