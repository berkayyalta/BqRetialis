// Made by Berkay

#ifndef BK_MEMORY_MAP_H
#define BK_MEMORY_MAP_H

#include "bk_types.h"

enum BkMemoryMapHardwareType : UInt32
{
    BK_MEMORY_MAP_TYPE_INVALID          = 0,
    BK_MEMORY_MAP_TYPE_USABLE           = 1,
    BK_MEMORY_MAP_TYPE_RESERVED         = 2,
    BK_MEMORY_MAP_TYPE_ACPI_RECLAIMABLE = 3,
    BK_MEMORY_MAP_TYPE_ACPI_NVS         = 4,
    BK_MEMORY_MAP_TYPE_BAD              = 5,
    BK_MEMORY_MAP_TYPE_KERNEL           = 6
};

enum BkMemoryMapAcpiAttribute : UInt32
{
    BK_MEMORY_MAP_ACPI_NONE         = 0x00000000,
    BK_MEMORY_MAP_ACPI_ENABLED      = 0x00000001,
    BK_MEMORY_MAP_ACPI_NON_VOLATILE = 0x00000002,
    BK_MEMORY_MAP_ACPI_SLOW_ACCESS  = 0x00000004,
    BK_MEMORY_MAP_ACPI_ERROR_LOG    = 0x00000008
};

struct BkMemoryMapEntry
{
    UInt64                          base_address;
    UInt64                          region_size;
    enum BkMemoryMapHardwareType    hardware_type;
    enum BkMemoryMapAcpiAttribute   acpi_attributes;
}
__attribute__((packed));

struct BkMemoryMap
{
    struct BkMemoryMapEntry*    entries;
    UInt32                      count;
};

#endif
