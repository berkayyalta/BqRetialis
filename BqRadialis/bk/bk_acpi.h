// Made by Berkay

#ifndef BK_ACPI_H
#define BK_ACPI_H

#include "bk_types.h"

#define BK_ACPI_MADT_LAPIC_ENABLED    0x00000001U
#define BK_ACPI_MADT_LAPIC_ONLINE_CAP 0x00000002U

enum BkAcpiMadtRecordType : UInt32
{
    BK_ACPI_MADT_TYPE_LOCAL_APIC          = 0,
    BK_ACPI_MADT_TYPE_IO_APIC             = 1,
    BK_ACPI_MADT_TYPE_IRQ_OVERRIDE        = 2,
    BK_ACPI_MADT_TYPE_NMI_SOURCE          = 3,
    BK_ACPI_MADT_TYPE_LOCAL_APIC_NMI      = 4,
    BK_ACPI_MADT_TYPE_LOCAL_APIC_OVERRIDE = 5
};

struct BkAcpiSdtHeader
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

struct BkAcpiGas
{
    UInt8  address_space_id;
    UInt8  register_bit_width;
    UInt8  register_bit_offset;
    UInt8  access_size;
    UInt64 base_address;
}
__attribute__((packed));

struct BkAcpiMadtRecordHeader
{
    UInt8 record_type;
    UInt8 record_length;
}
__attribute__((packed));

struct BkAcpiMadtLocalApic
{
    struct BkAcpiMadtRecordHeader header;
    UInt8                         processor_id;
    UInt8                         apic_id;
    UInt32                        flags;
}
__attribute__((packed));

struct BkAcpiMadtIoApic
{
    struct BkAcpiMadtRecordHeader header;
    UInt8                         io_apic_id;
    UInt8                         reserved;
    UInt32                        io_apic_address;
    UInt32                        gsi_base;
}
__attribute__((packed));

struct BkAcpiMadtIrqOverride
{
    struct BkAcpiMadtRecordHeader header;
    UInt8                         bus_source;
    UInt8                         irq_source;
    UInt32                        gsi;
    UInt16                        flags;
}
__attribute__((packed));

struct BkAcpiMadtLocalApicOverride
{
    struct BkAcpiMadtRecordHeader header;
    UInt16                        reserved;
    UInt64                        local_apic_address;
}
__attribute__((packed));

struct BkAcpiMadt
{
    struct BkAcpiSdtHeader header;
    UInt32                 local_apic_address;
    UInt32                 flags;
}
__attribute__((packed));

struct BkAcpiFadt
{
    struct BkAcpiSdtHeader header;
    UInt32                 firmware_ctrl;
    UInt32                 dsdt_address;
    UInt8                  reserved0;
    UInt8                  preferred_pm_profile;
    UInt16                 sci_interrupt;
    UInt32                 smi_command_port;
    UInt8                  acpi_enable;
    UInt8                  acpi_disable;
    UInt8                  s4bios_req;
    UInt8                  pstate_control;
    UInt32                 pm1a_event_block;
    UInt32                 pm1b_event_block;
    UInt32                 pm1a_control_block;
    UInt32                 pm1b_control_block;
    UInt32                 pm2_control_block;
    UInt32                 pm_timer_block;
    UInt32                 gpe0_block;
    UInt32                 gpe1_block;
    UInt8                  pm1_event_length;
    UInt8                  pm1_control_length;
    UInt8                  pm2_control_length;
    UInt8                  pm_timer_length;
    UInt8                  gpe0_length;
    UInt8                  gpe1_length;
    UInt8                  gpe1_base;
    UInt8                  cstate_control;
    UInt16                 worst_c2_latency;
    UInt16                 worst_c3_latency;
    UInt16                 flush_size;
    UInt16                 flush_stride;
    UInt8                  duty_offset;
    UInt8                  duty_width;
    UInt8                  day_alarm;
    UInt8                  month_alarm;
    UInt8                  century;
    UInt16                 boot_architecture_flags;
    UInt8                  reserved1;
    UInt32                 flags;
}
__attribute__((packed));

struct BkAcpiHpet
{
    struct BkAcpiSdtHeader header;
    UInt32                 hardware_block_id;
    struct BkAcpiGas       base_address;
    UInt8                  hpet_number;
    UInt16                 minimum_tick;
    UInt8                  page_protection;
}
__attribute__((packed));

struct BkAcpiMcfgAllocation
{
    UInt64 base_address;
    UInt16 pci_segment_group;
    UInt8  start_bus;
    UInt8  end_bus;
    UInt32 reserved;
}
__attribute__((packed));

struct BkAcpiMcfg
{
    struct BkAcpiSdtHeader header;
    UInt64                 reserved;
}
__attribute__((packed));

struct BkAcpiRoot
{
    struct BkAcpiMadt* madt;
    struct BkAcpiFadt* fadt;
    struct BkAcpiHpet* hpet;
    struct BkAcpiMcfg* mcfg;
};

#endif
