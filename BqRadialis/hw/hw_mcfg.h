// Made by Berkay

#ifndef HW_MCFG_H
#define HW_MCFG_H

#include "../bk/bk_types.h"

struct HwMcfgSdtHeader
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

struct HwMcfgAllocation
{
    UInt64 base_address;
    UInt16 pci_segment_group;
    UInt8  start_bus;
    UInt8  end_bus;
    UInt32 reserved;
}
__attribute__((packed));

struct HwMcfg
{
    struct HwMcfgSdtHeader header;
    UInt64                 reserved;
}
__attribute__((packed));

#endif
