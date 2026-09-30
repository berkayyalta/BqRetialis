// Made by Berkay

#ifndef CP_MADT_H
#define CP_MADT_H

#include "../bk/bk_types.h"

#define CP_MADT_LAPIC_ENABLED    0x00000001U
#define CP_MADT_LAPIC_ONLINE_CAP 0x00000002U

enum CpMadtRecordType : UInt32
{
    CP_MADT_TYPE_LOCAL_APIC          = 0,
    CP_MADT_TYPE_IO_APIC             = 1,
    CP_MADT_TYPE_IRQ_OVERRIDE        = 2,
    CP_MADT_TYPE_NMI_SOURCE          = 3,
    CP_MADT_TYPE_LOCAL_APIC_NMI      = 4,
    CP_MADT_TYPE_LOCAL_APIC_OVERRIDE = 5
};

struct CpMadtSdtHeader
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

struct CpMadtRecordHeader
{
    UInt8 record_type;
    UInt8 record_length;
}
__attribute__((packed));

struct CpMadtLocalApic
{
    struct CpMadtRecordHeader header;
    UInt8                     processor_id;
    UInt8                     apic_id;
    UInt32                    flags;
}
__attribute__((packed));

struct CpMadtLocalApicOverride
{
    struct CpMadtRecordHeader header;
    UInt16                    reserved;
    UInt64                    local_apic_address;
}
__attribute__((packed));

struct CpMadt
{
    struct CpMadtSdtHeader header;
    UInt32                 local_apic_address;
    UInt32                 flags;
}
__attribute__((packed));

#endif
