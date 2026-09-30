// Made by Berkay

#ifndef HW_DEV_H
#define HW_DEV_H

#include "../bk/bk_types.h"

#define HW_DEV_MAX_RESOURCES    8U
#define HW_DEV_MAX_COUNT        256U

#define HW_DEV_PS2_DATA_PORT    0x0060U
#define HW_DEV_PS2_COMMAND_PORT 0x0064U
#define HW_DEV_PS2_KEYBOARD_IRQ 1U
#define HW_DEV_PS2_MOUSE_IRQ    12U

#define HW_DEV_RTC_INDEX_PORT   0x0070U
#define HW_DEV_RTC_DATA_PORT    0x0071U
#define HW_DEV_RTC_IRQ          8U

#define HW_DEV_COM1_BASE_PORT   0x03F8U
#define HW_DEV_COM1_PORT_COUNT  8U
#define HW_DEV_COM1_IRQ         4U

enum HwDevBusType : UInt32
{
    HW_DEV_BUS_NONE     = 0,
    HW_DEV_BUS_PLATFORM = 1,
    HW_DEV_BUS_PCI      = 2,
    HW_DEV_BUS_FIRMWARE = 3
};

enum HwDevClass : UInt32
{
    HW_DEV_CLASS_UNKNOWN     = 0,
    HW_DEV_CLASS_STORAGE     = 1,
    HW_DEV_CLASS_NETWORK     = 2,
    HW_DEV_CLASS_DISPLAY     = 3,
    HW_DEV_CLASS_SERIAL_BUS  = 4,
    HW_DEV_CLASS_INPUT       = 5,
    HW_DEV_CLASS_SERIAL_PORT = 6,
    HW_DEV_CLASS_TIMER_RTC   = 7,
    HW_DEV_CLASS_BRIDGE      = 8,
    HW_DEV_CLASS_MULTIMEDIA  = 9,
    HW_DEV_CLASS_PROCESSOR   = 10
};

enum HwDevResourceType : UInt32
{
    HW_DEV_RESOURCE_NONE        = 0,
    HW_DEV_RESOURCE_MMIO        = 1,
    HW_DEV_RESOURCE_IO_PORT     = 2,
    HW_DEV_RESOURCE_IRQ         = 3,
    HW_DEV_RESOURCE_FRAMEBUFFER = 4
};

struct HwDevResource
{
    UInt64                  base_address;
    UInt64                  region_size;
    enum HwDevResourceType  resource_type;
    UInt32                  flags;
};

struct HwDevDisplayInfo
{
    UInt32 width;
    UInt32 height;
    UInt32 pitch;
    UInt32 bpp;
};

struct HwDevDevice
{
    UInt32                  index;
    enum HwDevBusType       bus_type;
    enum HwDevClass         device_class;
    UInt16                  vendor_id;
    UInt16                  device_id;
    UInt8                   bus;
    UInt8                   slot;
    UInt8                   function;
    UInt8                   class_code;
    UInt8                   subclass;
    UInt8                   prog_if;
    UInt8                   revision_id;
    UInt8                   irq_line;
    UInt32                  resource_count;
    struct HwDevResource    resources[HW_DEV_MAX_RESOURCES];
    struct HwDevDisplayInfo display;
};

struct HwDevLedger
{
    struct HwDevDevice  devices[HW_DEV_MAX_COUNT];
    UInt32              device_count;
};

#endif
