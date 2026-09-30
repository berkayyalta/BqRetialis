// Made by Berkay

#ifndef MM_MEMORY_MAP_H
#define MM_MEMORY_MAP_H

#include "../bk/bk_types.h"

enum MmMemoryMapHardwareType : UInt32
{
    MM_MEMORY_MAP_TYPE_INVALID          = 0,
    MM_MEMORY_MAP_TYPE_USABLE           = 1,
    MM_MEMORY_MAP_TYPE_RESERVED         = 2,
    MM_MEMORY_MAP_TYPE_ACPI_RECLAIMABLE = 3,
    MM_MEMORY_MAP_TYPE_ACPI_NVS         = 4,
    MM_MEMORY_MAP_TYPE_BAD              = 5,
    MM_MEMORY_MAP_TYPE_KERNEL           = 6
};

enum MmMemoryMapAcpiAttribute : UInt32
{
    MM_MEMORY_MAP_ACPI_NONE         = 0x00000000,
    MM_MEMORY_MAP_ACPI_ENABLED      = 0x00000001,
    MM_MEMORY_MAP_ACPI_NON_VOLATILE = 0x00000002,
    MM_MEMORY_MAP_ACPI_SLOW_ACCESS  = 0x00000004,
    MM_MEMORY_MAP_ACPI_ERROR_LOG    = 0x00000008
};

struct MmMemoryMapEntry
{
    UInt64                          base_address;
    UInt64                          region_size;
    enum MmMemoryMapHardwareType    hardware_type;
    enum MmMemoryMapAcpiAttribute   acpi_attributes;
}
__attribute__((packed));

struct MmMemoryMap
{
    struct MmMemoryMapEntry* entries;
    UInt32                   count;
};

#endif
