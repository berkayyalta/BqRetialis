// Made by Berkay

#ifndef BK_RBP_H
#define BK_RBP_H

#include "bk_types.h"

struct BkRbpKernelLayout
{
    UInt64 physical_base;
    UInt64 virtual_base;
    UInt64 region_size;
}
__attribute__((packed));

enum BkRbpMemoryMapHardwareType : UInt32
{
    BK_RBP_MEMORY_MAP_TYPE_INVALID          = 0,
    BK_RBP_MEMORY_MAP_TYPE_USABLE           = 1,
    BK_RBP_MEMORY_MAP_TYPE_RESERVED         = 2,
    BK_RBP_MEMORY_MAP_TYPE_ACPI_RECLAIMABLE = 3,
    BK_RBP_MEMORY_MAP_TYPE_ACPI_NVS         = 4,
    BK_RBP_MEMORY_MAP_TYPE_BAD              = 5,
    BK_RBP_MEMORY_MAP_TYPE_KERNEL           = 6
};

enum BkRbpMemoryMapAcpiAttribute : UInt32
{
    BK_RBP_MEMORY_MAP_ACPI_NONE         = 0x00000000,
    BK_RBP_MEMORY_MAP_ACPI_ENABLED      = 0x00000001,
    BK_RBP_MEMORY_MAP_ACPI_NON_VOLATILE = 0x00000002,
    BK_RBP_MEMORY_MAP_ACPI_SLOW_ACCESS  = 0x00000004,
    BK_RBP_MEMORY_MAP_ACPI_ERROR_LOG    = 0x00000008
};

struct BkRbpMemoryMapEntry
{
    UInt64                           base_address;
    UInt64                           region_size;
    enum BkRbpMemoryMapHardwareType  hardware_type;
    enum BkRbpMemoryMapAcpiAttribute acpi_attributes;
}
__attribute__((packed));

struct BkRbpMemoryMap
{
    struct BkRbpMemoryMapEntry* entries;
    UInt32                      count;
};

enum BkRbpAcpiMadtRecordType : UInt32
{
    BK_RBP_ACPI_MADT_TYPE_LOCAL_APIC          = 0,
    BK_RBP_ACPI_MADT_TYPE_IO_APIC             = 1,
    BK_RBP_ACPI_MADT_TYPE_IRQ_OVERRIDE        = 2,
    BK_RBP_ACPI_MADT_TYPE_NMI_SOURCE          = 3,
    BK_RBP_ACPI_MADT_TYPE_LOCAL_APIC_NMI      = 4,
    BK_RBP_ACPI_MADT_TYPE_LOCAL_APIC_OVERRIDE = 5
};

struct BkRbpAcpiSdtHeader
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

struct BkRbpAcpiGas
{
    UInt8  address_space_id;
    UInt8  register_bit_width;
    UInt8  register_bit_offset;
    UInt8  access_size;
    UInt64 base_address;
}
__attribute__((packed));

struct BkRbpAcpiMadtRecordHeader
{
    UInt8 record_type;
    UInt8 record_length;
}
__attribute__((packed));

struct BkRbpAcpiMadtLocalApic
{
    struct BkRbpAcpiMadtRecordHeader header;
    UInt8                            processor_id;
    UInt8                            apic_id;
    UInt32                           flags;
}
__attribute__((packed));

struct BkRbpAcpiMadtIoApic
{
    struct BkRbpAcpiMadtRecordHeader header;
    UInt8                            io_apic_id;
    UInt8                            reserved;
    UInt32                           io_apic_address;
    UInt32                           gsi_base;
}
__attribute__((packed));

struct BkRbpAcpiMadtIrqOverride
{
    struct BkRbpAcpiMadtRecordHeader header;
    UInt8                            bus_source;
    UInt8                            irq_source;
    UInt32                           gsi;
    UInt16                           flags;
}
__attribute__((packed));

struct BkRbpAcpiMadtLocalApicOverride
{
    struct BkRbpAcpiMadtRecordHeader header;
    UInt16                           reserved;
    UInt64                           local_apic_address;
}
__attribute__((packed));

struct BkRbpAcpiMadt
{
    struct BkRbpAcpiSdtHeader header;
    UInt32                    local_apic_address;
    UInt32                    flags;
}
__attribute__((packed));

struct BkRbpAcpiFadt
{
    struct BkRbpAcpiSdtHeader header;
    UInt32                    firmware_ctrl;
    UInt32                    dsdt_address;
    UInt8                     reserved0;
    UInt8                     preferred_pm_profile;
    UInt16                    sci_interrupt;
    UInt32                    smi_command_port;
    UInt8                     acpi_enable;
    UInt8                     acpi_disable;
    UInt8                     s4bios_req;
    UInt8                     pstate_control;
    UInt32                    pm1a_event_block;
    UInt32                    pm1b_event_block;
    UInt32                    pm1a_control_block;
    UInt32                    pm1b_control_block;
    UInt32                    pm2_control_block;
    UInt32                    pm_timer_block;
    UInt32                    gpe0_block;
    UInt32                    gpe1_block;
    UInt8                     pm1_event_length;
    UInt8                     pm1_control_length;
    UInt8                     pm2_control_length;
    UInt8                     pm_timer_length;
    UInt8                     gpe0_length;
    UInt8                     gpe1_length;
    UInt8                     gpe1_base;
    UInt8                     cstate_control;
    UInt16                    worst_c2_latency;
    UInt16                    worst_c3_latency;
    UInt16                    flush_size;
    UInt16                    flush_stride;
    UInt8                     duty_offset;
    UInt8                     duty_width;
    UInt8                     day_alarm;
    UInt8                     month_alarm;
    UInt8                     century;
    UInt16                    boot_architecture_flags;
    UInt8                     reserved1;
    UInt32                    flags;
}
__attribute__((packed));

struct BkRbpAcpiHpet
{
    struct BkRbpAcpiSdtHeader header;
    UInt32                    hardware_block_id;
    struct BkRbpAcpiGas       base_address;
    UInt8                     hpet_number;
    UInt16                    minimum_tick;
    UInt8                     page_protection;
}
__attribute__((packed));

struct BkRbpAcpiMcfgAllocation
{
    UInt64 base_address;
    UInt16 pci_segment_group;
    UInt8  start_bus;
    UInt8  end_bus;
    UInt32 reserved;
}
__attribute__((packed));

struct BkRbpAcpiMcfg
{
    struct BkRbpAcpiSdtHeader header;
    UInt64                    reserved;
}
__attribute__((packed));

struct BkRbpAcpiRoot
{
    struct BkRbpAcpiMadt* madt;
    struct BkRbpAcpiFadt* fadt;
    struct BkRbpAcpiHpet* hpet;
    struct BkRbpAcpiMcfg* mcfg;
};

struct BkRbpFramebuffer
{
    UInt64 physical_base;
    UInt64 region_size;
    UInt32 width;
    UInt32 height;
    UInt32 pitch;
    UInt32 bpp;
}
__attribute__((packed));

struct BkRbpModule
{
    UInt64 physical_base;
    UInt64 size;
    char   path[128];
};

struct BkRbpBootInfo
{
    struct BkRbpKernelLayout kernel_layout;
    struct BkRbpMemoryMap    memory_map;
    struct BkRbpAcpiRoot     acpi_root;
    struct BkRbpFramebuffer  framebuffer;
    struct BkRbpModule*      modules;
    UInt32                   module_count;
};

struct BkRbpLedger
{
    struct BkRbpBootInfo boot_info;
};

#endif
