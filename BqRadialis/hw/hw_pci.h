// Made by Berkay

#ifndef HW_PCI_H
#define HW_PCI_H

#include "../bk/bk_types.h"

#define HW_PCI_ADDRESS_PORT     0x0CF8U
#define HW_PCI_DATA_PORT        0x0CFCU
#define HW_PCI_ADDRESS_ENABLE   0x80000000U

#define HW_PCI_VENDOR_INVALID   0xFFFFU

#define HW_PCI_MAX_BUSES        256U
#define HW_PCI_MAX_SLOTS        32U
#define HW_PCI_MAX_FUNCTIONS    8U
#define HW_PCI_MAX_BARS         6U
#define HW_PCI_MAX_DEVICES      256U

#define HW_PCI_CAP_ID_MSI       0x05U
#define HW_PCI_CAP_ID_MSIX      0x11U

enum HwPciConfigOffset : UInt32
{
    HW_PCI_OFFSET_VENDOR_ID       = 0x00,
    HW_PCI_OFFSET_DEVICE_ID       = 0x02,
    HW_PCI_OFFSET_COMMAND         = 0x04,
    HW_PCI_OFFSET_STATUS          = 0x06,
    HW_PCI_OFFSET_REVISION_ID     = 0x08,
    HW_PCI_OFFSET_PROG_IF         = 0x09,
    HW_PCI_OFFSET_SUBCLASS        = 0x0A,
    HW_PCI_OFFSET_CLASS_CODE      = 0x0B,
    HW_PCI_OFFSET_HEADER_TYPE     = 0x0E,
    HW_PCI_OFFSET_BAR0            = 0x10,
    HW_PCI_OFFSET_PRIMARY_BUS     = 0x18,
    HW_PCI_OFFSET_SECONDARY_BUS   = 0x19,
    HW_PCI_OFFSET_SUBORDINATE_BUS = 0x1A,
    HW_PCI_OFFSET_CAP_PTR         = 0x34,
    HW_PCI_OFFSET_IRQ_LINE        = 0x3C,
    HW_PCI_OFFSET_IRQ_PIN         = 0x3D
};

enum HwPciCommandFlag : UInt32
{
    HW_PCI_COMMAND_IO_SPACE          = 0x0001,
    HW_PCI_COMMAND_MEMORY_SPACE      = 0x0002,
    HW_PCI_COMMAND_BUS_MASTER        = 0x0004,
    HW_PCI_COMMAND_INTERRUPT_DISABLE = 0x0400
};

enum HwPciBarType : UInt32
{
    HW_PCI_BAR_TYPE_NONE    = 0,
    HW_PCI_BAR_TYPE_MMIO32  = 1,
    HW_PCI_BAR_TYPE_MMIO64  = 2,
    HW_PCI_BAR_TYPE_IO_PORT = 3
};

struct HwPciBar
{
    UInt64              base_address;
    UInt64              region_size;
    enum HwPciBarType   bar_type;
    UInt32              is_prefetchable;
};

struct HwPciDevice
{
    UInt8           bus;
    UInt8           slot;
    UInt8           function;
    UInt8           header_type;
    UInt16          vendor_id;
    UInt16          device_id;
    UInt8           class_code;
    UInt8           subclass;
    UInt8           prog_if;
    UInt8           revision_id;
    UInt8           irq_line;
    UInt8           irq_pin;
    UInt16          reserved;
    struct HwPciBar bars[HW_PCI_MAX_BARS];
};

struct HwPciLedger
{
    struct HwPciDevice  devices[HW_PCI_MAX_DEVICES];
    UInt32              device_count;
    UInt64              ecam_virtual_base;
    UInt8               ecam_start_bus;
    UInt8               ecam_end_bus;
    UInt8               visited_buses[HW_PCI_MAX_BUSES];
    volatile UInt32     spin_lock;
};

#endif
