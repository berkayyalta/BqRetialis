// Made by Berkay

#ifndef HW_DEVICE_H
#define HW_DEVICE_H

#include "../bk/bk_types.h"

#define HW_DEVICE_MAX_RESOURCES 8U

enum HwDeviceBusType : UInt32
{
    HW_DEVICE_BUS_NONE     = 0,
    HW_DEVICE_BUS_PLATFORM = 1,
    HW_DEVICE_BUS_PCI      = 2,
    HW_DEVICE_BUS_FIRMWARE = 3
};

enum HwDeviceClass : UInt32
{
    HW_DEVICE_CLASS_UNKNOWN     = 0,
    HW_DEVICE_CLASS_STORAGE     = 1,
    HW_DEVICE_CLASS_NETWORK     = 2,
    HW_DEVICE_CLASS_DISPLAY     = 3,
    HW_DEVICE_CLASS_SERIAL_BUS  = 4,
    HW_DEVICE_CLASS_INPUT       = 5,
    HW_DEVICE_CLASS_SERIAL_PORT = 6,
    HW_DEVICE_CLASS_TIMER_RTC   = 7,
    HW_DEVICE_CLASS_BRIDGE      = 8,
    HW_DEVICE_CLASS_MULTIMEDIA  = 9,
    HW_DEVICE_CLASS_PROCESSOR   = 10
};

enum HwDeviceResourceType : UInt32
{
    HW_DEVICE_RESOURCE_NONE        = 0,
    HW_DEVICE_RESOURCE_MMIO        = 1,
    HW_DEVICE_RESOURCE_IO_PORT     = 2,
    HW_DEVICE_RESOURCE_IRQ         = 3,
    HW_DEVICE_RESOURCE_FRAMEBUFFER = 4
};

struct HwDeviceResource
{
    UInt64                      base_address;
    UInt64                      region_size;
    enum HwDeviceResourceType   resource_type;
    UInt32                      flags;
};

struct HwDeviceDisplayInfo
{
    UInt32 width;
    UInt32 height;
    UInt32 pitch;
    UInt32 bpp;
};

struct HwDevice
{
    UInt32                      index;
    enum HwDeviceBusType        bus_type;
    enum HwDeviceClass          device_class;
    UInt16                      vendor_id;
    UInt16                      device_id;
    UInt8                       bus;
    UInt8                       slot;
    UInt8                       function;
    UInt8                       class_code;
    UInt8                       subclass;
    UInt8                       prog_if;
    UInt8                       revision_id;
    UInt8                       irq_line;
    UInt32                      resource_count;
    struct HwDeviceResource     resources[HW_DEVICE_MAX_RESOURCES];
    struct HwDeviceDisplayInfo  display;
};

#endif
