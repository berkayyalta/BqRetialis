// Made by Berkay

#ifndef HW_MADT_H
#define HW_MADT_H

#include "../bk/bk_types.h"

enum HwMadtRecordType : UInt32
{
    HW_MADT_TYPE_LOCAL_APIC          = 0,
    HW_MADT_TYPE_IO_APIC             = 1,
    HW_MADT_TYPE_IRQ_OVERRIDE        = 2,
    HW_MADT_TYPE_NMI_SOURCE          = 3,
    HW_MADT_TYPE_LOCAL_APIC_NMI      = 4,
    HW_MADT_TYPE_LOCAL_APIC_OVERRIDE = 5
};

struct HwMadtSdtHeader
{
    UInt32 signature;
    UInt32 length;
    UInt8  revision;
    UInt8  checksum;
    UInt8  oem_id[6];
    UInt8  oem_table_id[8];
    UInt32 oem_revision;
    UInt32 creator_id;
    UInt32 creator_revision;
}
__attribute__((packed));

struct HwMadtRecordHeader
{
    UInt8 record_type;
    UInt8 record_length;
}
__attribute__((packed));

struct HwMadtIoApic
{
    struct HwMadtRecordHeader header;
    UInt8                     io_apic_id;
    UInt8                     reserved;
    UInt32                    io_apic_address;
    UInt32                    gsi_base;
}
__attribute__((packed));

struct HwMadtIrqOverride
{
    struct HwMadtRecordHeader header;
    UInt8                     bus_source;
    UInt8                     irq_source;
    UInt32                    gsi;
    UInt16                    flags;
}
__attribute__((packed));

struct HwMadt
{
    struct HwMadtSdtHeader header;
    UInt32                 local_apic_address;
    UInt32                 flags;
}
__attribute__((packed));

#endif
