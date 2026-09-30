// Made by Berkay

#ifndef ABI_SC_HW_H
#define ABI_SC_HW_H

#include "abi_sc_status.h"
#include "../hw/hw_public.h"

struct AbiScIn8Form
{
    UInt16 port;
    UInt8  value;
};
typedef struct AbiScIn8Form AbiScIn8Form;
AbiScStatus AbiScIn8(AbiScIn8Form* form);

struct AbiScOut8Form
{
    UInt16 port;
    UInt8  data;
};
typedef struct AbiScOut8Form AbiScOut8Form;
AbiScStatus AbiScOut8(AbiScOut8Form* form);

struct AbiScIn16Form
{
    UInt16 port;
    UInt16 value;
};
typedef struct AbiScIn16Form AbiScIn16Form;
AbiScStatus AbiScIn16(AbiScIn16Form* form);

struct AbiScOut16Form
{
    UInt16 port;
    UInt16 data;
};
typedef struct AbiScOut16Form AbiScOut16Form;
AbiScStatus AbiScOut16(AbiScOut16Form* form);

struct AbiScIn32Form
{
    UInt16 port;
    UInt32 value;
};
typedef struct AbiScIn32Form AbiScIn32Form;
AbiScStatus AbiScIn32(AbiScIn32Form* form);

struct AbiScOut32Form
{
    UInt16 port;
    UInt32 data;
};
typedef struct AbiScOut32Form AbiScOut32Form;
AbiScStatus AbiScOut32(AbiScOut32Form* form);

struct AbiScMaskIrqForm
{
    UInt8 irq;
};
typedef struct AbiScMaskIrqForm AbiScMaskIrqForm;
AbiScStatus AbiScMaskIrq(AbiScMaskIrqForm* form);

struct AbiScUnmaskIrqForm
{
    UInt8 irq;
};
typedef struct AbiScUnmaskIrqForm AbiScUnmaskIrqForm;
AbiScStatus AbiScUnmaskIrq(AbiScUnmaskIrqForm* form);

struct AbiScSendEoiForm
{
    UInt8 irq;
};
typedef struct AbiScSendEoiForm AbiScSendEoiForm;
AbiScStatus AbiScSendEoi(AbiScSendEoiForm* form);

struct AbiScSetTimerFrequencyForm
{
    UInt32 frequency;
};
typedef struct AbiScSetTimerFrequencyForm AbiScSetTimerFrequencyForm;
AbiScStatus AbiScSetTimerFrequency(AbiScSetTimerFrequencyForm* form);

struct AbiScGetTimerFrequencyForm
{
    UInt32 frequency;
};
typedef struct AbiScGetTimerFrequencyForm AbiScGetTimerFrequencyForm;
AbiScStatus AbiScGetTimerFrequency(AbiScGetTimerFrequencyForm* form);

struct AbiScGetTimerTicksForm
{
    UInt64 ticks;
};
typedef struct AbiScGetTimerTicksForm AbiScGetTimerTicksForm;
AbiScStatus AbiScGetTimerTicks(AbiScGetTimerTicksForm* form);

struct AbiScReadPciConfig32Form
{
    UInt32 value;
    UInt8  bus;
    UInt8  slot;
    UInt8  function;
    UInt8  offset;
};
typedef struct AbiScReadPciConfig32Form AbiScReadPciConfig32Form;
AbiScStatus AbiScReadPciConfig32(AbiScReadPciConfig32Form* form);

struct AbiScWritePciConfig32Form
{
    UInt8  bus;
    UInt8  slot;
    UInt8  function;
    UInt8  offset;
    UInt32 value;
};
typedef struct AbiScWritePciConfig32Form AbiScWritePciConfig32Form;
AbiScStatus AbiScWritePciConfig32(AbiScWritePciConfig32Form* form);

struct AbiScGetDeviceCountForm
{
    UInt32 count;
};
typedef struct AbiScGetDeviceCountForm AbiScGetDeviceCountForm;
AbiScStatus AbiScGetDeviceCount(AbiScGetDeviceCountForm* form);

struct AbiScGetDeviceForm
{
    HwDevice device;
    UInt32   index;
};
typedef struct AbiScGetDeviceForm AbiScGetDeviceForm;
AbiScStatus AbiScGetDevice(AbiScGetDeviceForm* form);

struct AbiScRouteAcpiIrqForm
{
    UInt8 irq_source;
    UInt8 vector;
    UInt8 target_apic_id;
    UInt8 masked;
};
typedef struct AbiScRouteAcpiIrqForm AbiScRouteAcpiIrqForm;
AbiScStatus AbiScRouteAcpiIrq(AbiScRouteAcpiIrqForm* form);

struct AbiScReadPciConfig8Form
{
    UInt8 value;
    UInt8 bus;
    UInt8 slot;
    UInt8 function;
    UInt8 offset;
};
typedef struct AbiScReadPciConfig8Form AbiScReadPciConfig8Form;
AbiScStatus AbiScReadPciConfig8(AbiScReadPciConfig8Form* form);

struct AbiScWritePciConfig8Form
{
    UInt8 bus;
    UInt8 slot;
    UInt8 function;
    UInt8 offset;
    UInt8 value;
};
typedef struct AbiScWritePciConfig8Form AbiScWritePciConfig8Form;
AbiScStatus AbiScWritePciConfig8(AbiScWritePciConfig8Form* form);

struct AbiScReadPciConfig16Form
{
    UInt16 value;
    UInt8  bus;
    UInt8  slot;
    UInt8  function;
    UInt8  offset;
};
typedef struct AbiScReadPciConfig16Form AbiScReadPciConfig16Form;
AbiScStatus AbiScReadPciConfig16(AbiScReadPciConfig16Form* form);

struct AbiScWritePciConfig16Form
{
    UInt8  bus;
    UInt8  slot;
    UInt8  function;
    UInt8  offset;
    UInt16 value;
};
typedef struct AbiScWritePciConfig16Form AbiScWritePciConfig16Form;
AbiScStatus AbiScWritePciConfig16(AbiScWritePciConfig16Form* form);

struct AbiScEnablePciDeviceForm
{
    UInt8 bus;
    UInt8 slot;
    UInt8 function;
};
typedef struct AbiScEnablePciDeviceForm AbiScEnablePciDeviceForm;
AbiScStatus AbiScEnablePciDevice(AbiScEnablePciDeviceForm* form);

struct AbiScFindPciCapabilityForm
{
    UInt8 cap_offset;
    UInt8 bus;
    UInt8 slot;
    UInt8 function;
    UInt8 cap_id;
};
typedef struct AbiScFindPciCapabilityForm AbiScFindPciCapabilityForm;
AbiScStatus AbiScFindPciCapability(AbiScFindPciCapabilityForm* form);

struct AbiScEnablePciMsiForm
{
    UInt8 bus;
    UInt8 slot;
    UInt8 function;
    UInt8 vector;
    UInt8 target_apic_id;
};
typedef struct AbiScEnablePciMsiForm AbiScEnablePciMsiForm;
AbiScStatus AbiScEnablePciMsi(AbiScEnablePciMsiForm* form);

struct AbiScFindDeviceByClassForm
{
    HwDevice      device;
    HwDeviceClass device_class;
    UInt32        occurrence;
};
typedef struct AbiScFindDeviceByClassForm AbiScFindDeviceByClassForm;
AbiScStatus AbiScFindDeviceByClass(AbiScFindDeviceByClassForm* form);

struct AbiScFindDeviceByIdForm
{
    HwDevice device;
    UInt16   vendor_id;
    UInt16   device_id;
    UInt32   occurrence;
};
typedef struct AbiScFindDeviceByIdForm AbiScFindDeviceByIdForm;
AbiScStatus AbiScFindDeviceById(AbiScFindDeviceByIdForm* form);

struct AbiScFindDeviceByLocationForm
{
    HwDevice device;
    UInt8    bus;
    UInt8    slot;
    UInt8    function;
};
typedef struct AbiScFindDeviceByLocationForm AbiScFindDeviceByLocationForm;
AbiScStatus AbiScFindDeviceByLocation(AbiScFindDeviceByLocationForm* form);

struct AbiScFindDeviceResourceForm
{
    HwDeviceResource     resource;
    HwDevice             device;
    HwDeviceResourceType resource_type;
    UInt32               occurrence;
};
typedef struct AbiScFindDeviceResourceForm AbiScFindDeviceResourceForm;
AbiScStatus AbiScFindDeviceResource(AbiScFindDeviceResourceForm* form);

#endif
