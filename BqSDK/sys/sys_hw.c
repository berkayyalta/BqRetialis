// Made by Berkay

#include "sys_hw.h"

BqStatus BqIn8(UInt16 port, UInt8* value)
{
    if (value == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScIn8Form form;
    form.port  = port;
    form.value = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_IN8, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *value = form.value;
    }

    return status;
}

BqStatus BqIoRead8(UInt16 port, UInt8* value)
{
    return BqIn8(port, value);
}

BqStatus BqOut8(UInt16 port, UInt8 data)
{
    AbiScOut8Form form;
    form.port = port;
    form.data = data;
    return BqSyscall(ABI_SC_HW_OUT8, &form);
}

BqStatus BqIoWrite8(UInt16 port, UInt8 data)
{
    return BqOut8(port, data);
}

BqStatus BqIn16(UInt16 port, UInt16* value)
{
    if (value == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScIn16Form form;
    form.port  = port;
    form.value = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_IN16, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *value = form.value;
    }

    return status;
}

BqStatus BqIoRead16(UInt16 port, UInt16* value)
{
    return BqIn16(port, value);
}

BqStatus BqOut16(UInt16 port, UInt16 data)
{
    AbiScOut16Form form;
    form.port = port;
    form.data = data;
    return BqSyscall(ABI_SC_HW_OUT16, &form);
}

BqStatus BqIoWrite16(UInt16 port, UInt16 data)
{
    return BqOut16(port, data);
}

BqStatus BqIn32(UInt16 port, UInt32* value)
{
    if (value == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScIn32Form form;
    form.port  = port;
    form.value = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_IN32, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *value = form.value;
    }

    return status;
}

BqStatus BqIoRead32(UInt16 port, UInt32* value)
{
    return BqIn32(port, value);
}

BqStatus BqOut32(UInt16 port, UInt32 data)
{
    AbiScOut32Form form;
    form.port = port;
    form.data = data;
    return BqSyscall(ABI_SC_HW_OUT32, &form);
}

BqStatus BqIoWrite32(UInt16 port, UInt32 data)
{
    return BqOut32(port, data);
}

BqStatus BqMaskIrq(UInt8 irq)
{
    AbiScMaskIrqForm form;
    form.irq = irq;
    return BqSyscall(ABI_SC_HW_MASK_IRQ, &form);
}

BqStatus BqUnmaskIrq(UInt8 irq)
{
    AbiScUnmaskIrqForm form;
    form.irq = irq;
    return BqSyscall(ABI_SC_HW_UNMASK_IRQ, &form);
}

BqStatus BqSendEoi(UInt8 irq)
{
    AbiScSendEoiForm form;
    form.irq = irq;
    return BqSyscall(ABI_SC_HW_SEND_EOI, &form);
}

BqStatus BqSetTimerFrequency(UInt32 frequency)
{
    AbiScSetTimerFrequencyForm form;
    form.frequency = frequency;
    return BqSyscall(ABI_SC_HW_SET_TIMER_FREQUENCY, &form);
}

BqStatus BqGetTimerFrequency(UInt32* frequency)
{
    if (frequency == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetTimerFrequencyForm form;
    form.frequency = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_GET_TIMER_FREQUENCY, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *frequency = form.frequency;
    }

    return status;
}

BqStatus BqGetTimerTicks(UInt64* ticks)
{
    if (ticks == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetTimerTicksForm form;
    form.ticks = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_GET_TIMER_TICKS, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *ticks = form.ticks;
    }

    return status;
}

BqStatus BqReadPciConfig32(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt32* value)
{
    if (value == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadPciConfig32Form form;
    form.bus      = bus;
    form.slot     = slot;
    form.function = function;
    form.offset   = offset;
    form.value    = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_READ_PCI_CONFIG32, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *value = form.value;
    }

    return status;
}

BqStatus BqWritePciConfig32(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt32 value)
{
    AbiScWritePciConfig32Form form;
    form.bus      = bus;
    form.slot     = slot;
    form.function = function;
    form.offset   = offset;
    form.value    = value;
    return BqSyscall(ABI_SC_HW_WRITE_PCI_CONFIG32, &form);
}

BqStatus BqGetDeviceCount(UInt32* count)
{
    if (count == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetDeviceCountForm form;
    form.count = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_GET_DEVICE_COUNT, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *count = form.count;
    }

    return status;
}

BqStatus BqGetDevice(UInt32 index, HwDevice* device)
{
    if (device == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetDeviceForm form;
    form.index = index;
    BqStatus status = BqSyscall(ABI_SC_HW_GET_DEVICE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *device = form.device;
    }

    return status;
}

BqStatus BqRouteAcpiIrq(UInt8 irq_source, UInt8 vector, UInt8 target_apic_id, UInt8 masked)
{
    AbiScRouteAcpiIrqForm form;
    form.irq_source     = irq_source;
    form.vector         = vector;
    form.target_apic_id = target_apic_id;
    form.masked         = masked;
    return BqSyscall(ABI_SC_HW_ROUTE_ACPI_IRQ, &form);
}

BqStatus BqReadPciConfig8(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt8* value)
{
    if (value == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadPciConfig8Form form;
    form.bus      = bus;
    form.slot     = slot;
    form.function = function;
    form.offset   = offset;
    form.value    = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_READ_PCI_CONFIG8, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *value = form.value;
    }

    return status;
}

BqStatus BqWritePciConfig8(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt8 value)
{
    AbiScWritePciConfig8Form form;
    form.bus      = bus;
    form.slot     = slot;
    form.function = function;
    form.offset   = offset;
    form.value    = value;
    return BqSyscall(ABI_SC_HW_WRITE_PCI_CONFIG8, &form);
}

BqStatus BqReadPciConfig16(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt16* value)
{
    if (value == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadPciConfig16Form form;
    form.bus      = bus;
    form.slot     = slot;
    form.function = function;
    form.offset   = offset;
    form.value    = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_READ_PCI_CONFIG16, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *value = form.value;
    }

    return status;
}

BqStatus BqWritePciConfig16(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt16 value)
{
    AbiScWritePciConfig16Form form;
    form.bus      = bus;
    form.slot     = slot;
    form.function = function;
    form.offset   = offset;
    form.value    = value;
    return BqSyscall(ABI_SC_HW_WRITE_PCI_CONFIG16, &form);
}

BqStatus BqEnablePciDevice(UInt8 bus, UInt8 slot, UInt8 function)
{
    AbiScEnablePciDeviceForm form;
    form.bus      = bus;
    form.slot     = slot;
    form.function = function;
    return BqSyscall(ABI_SC_HW_ENABLE_PCI_DEVICE, &form);
}

BqStatus BqFindPciCapability(UInt8 bus, UInt8 slot, UInt8 function, UInt8 cap_id, UInt8* cap_offset)
{
    if (cap_offset == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindPciCapabilityForm form;
    form.bus        = bus;
    form.slot       = slot;
    form.function   = function;
    form.cap_id     = cap_id;
    form.cap_offset = 0;
    BqStatus status = BqSyscall(ABI_SC_HW_FIND_PCI_CAPABILITY, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *cap_offset = form.cap_offset;
    }

    return status;
}

BqStatus BqEnablePciMsi(UInt8 bus, UInt8 slot, UInt8 function, UInt8 vector, UInt8 target_apic_id)
{
    AbiScEnablePciMsiForm form;
    form.bus            = bus;
    form.slot           = slot;
    form.function       = function;
    form.vector         = vector;
    form.target_apic_id = target_apic_id;
    return BqSyscall(ABI_SC_HW_ENABLE_PCI_MSI, &form);
}

BqStatus BqFindDeviceByClass(HwDeviceClass device_class, UInt32 occurrence, HwDevice* device)
{
    if (device == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDeviceByClassForm form;
    form.device_class = device_class;
    form.occurrence   = occurrence;
    BqStatus status = BqSyscall(ABI_SC_HW_FIND_DEVICE_BY_CLASS, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *device = form.device;
    }

    return status;
}

BqStatus BqFindDeviceById(UInt16 vendor_id, UInt16 device_id, UInt32 occurrence, HwDevice* device)
{
    if (device == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDeviceByIdForm form;
    form.vendor_id  = vendor_id;
    form.device_id  = device_id;
    form.occurrence = occurrence;
    BqStatus status = BqSyscall(ABI_SC_HW_FIND_DEVICE_BY_ID, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *device = form.device;
    }

    return status;
}

BqStatus BqFindDeviceByLocation(UInt8 bus, UInt8 slot, UInt8 function, HwDevice* device)
{
    if (device == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDeviceByLocationForm form;
    form.bus      = bus;
    form.slot     = slot;
    form.function = function;
    BqStatus status = BqSyscall(ABI_SC_HW_FIND_DEVICE_BY_LOCATION, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *device = form.device;
    }

    return status;
}

BqStatus BqFindDeviceResource(HwDevice device, HwDeviceResourceType resource_type, UInt32 occurrence, HwDeviceResource* resource)
{
    if (resource == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDeviceResourceForm form;
    form.device        = device;
    form.resource_type = resource_type;
    form.occurrence    = occurrence;
    BqStatus status = BqSyscall(ABI_SC_HW_FIND_DEVICE_RESOURCE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *resource = form.resource;
    }

    return status;
}
