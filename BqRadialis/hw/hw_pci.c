// Made by Berkay

#include "hw_private.h"

static HwPciLedger s_pci_ledger;

HwStatus HwPciLedgerAllocateSpace(void)
{
    MmStatus mm_status = MmZeroVirtualMemory(&s_pci_ledger, sizeof(HwPciLedger));
    if (mm_status != MM_STATUS_SUCCESS)
    {
        return HW_STATUS_IO_FAILURE;
    }

    BkAcpiMcfg* mcfg = BkGetAcpiMcfg();
    if (mcfg != 0 && mcfg->header.length > sizeof(HwMcfg))
    {
        HwMcfg* hw_mcfg = (HwMcfg*)mcfg;
        UInt64 payload_length = (UInt64)(hw_mcfg->header.length - sizeof(HwMcfg));
        UInt64 entry_count    = payload_length / sizeof(HwMcfgAllocation);

        if (entry_count > 0)
        {
            HwMcfgAllocation* allocations = (HwMcfgAllocation*)((UInt8*)hw_mcfg + sizeof(HwMcfg));
            UInt64 phys_base = allocations[0].base_address;
            UInt16 seg_group = allocations[0].pci_segment_group;
            UInt8  start_bus = allocations[0].start_bus;
            UInt8  end_bus   = allocations[0].end_bus;

            if (phys_base != 0 && seg_group == 0)
            {
                UInt64 bus_count  = (UInt64)(end_bus - start_bus) + 1ULL;
                UInt64 total_size = bus_count * 32ULL * 8ULL * 4096ULL;
                UInt64 page_count = total_size / 4096ULL;
                UInt64 virt_base  = 0xFFFF800000000000ULL + phys_base;

                if (MmMapVirtualRegion(virt_base, phys_base, page_count, 0x13ULL) == MM_STATUS_SUCCESS)
                {
                    s_pci_ledger.ecam_virtual_base = virt_base;
                    s_pci_ledger.ecam_start_bus    = start_bus;
                    s_pci_ledger.ecam_end_bus      = end_bus;
                }
            }
        }
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwPciLedgerPopulate(void)
{
    UInt32 reg3 = 0;
    if (HwPciReadConfig32(&reg3, 0, 0, 0, 0x0CU) == HW_STATUS_SUCCESS)
    {
        UInt8 header_type = (UInt8)((reg3 >> 16) & 0xFFU);
        if ((header_type & 0x80U) != 0)
        {
            for (UInt32 function = 0; function < HW_PCI_MAX_FUNCTIONS; function++)
            {
                UInt32 reg0 = 0;
                if (HwPciReadConfig32(&reg0, 0, 0, (UInt8)function, HW_PCI_OFFSET_VENDOR_ID) == HW_STATUS_SUCCESS &&
                    (UInt16)(reg0 & 0xFFFFU) != HW_PCI_VENDOR_INVALID)
                {
                    HwPciScanBus((UInt8)function);
                }
            }

            return HW_STATUS_SUCCESS;
        }
    }

    return HwPciScanBus(0);
}

HwStatus HwPciLedgerLoad(void)
{
    HwStatus status = HwPciLedgerAllocateSpace();
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    return HwPciLedgerPopulate();
}

HwStatus HwPciReadConfig32(UInt32* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset)
{
    if (value == 0 || slot >= HW_PCI_MAX_SLOTS || function >= HW_PCI_MAX_FUNCTIONS)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    if (s_pci_ledger.ecam_virtual_base != 0 &&
        bus >= s_pci_ledger.ecam_start_bus &&
        bus <= s_pci_ledger.ecam_end_bus)
    {
        UInt64 bus_offset  = (UInt64)(bus - s_pci_ledger.ecam_start_bus) << 20;
        UInt64 slot_offset = (UInt64)slot << 15;
        UInt64 func_offset = (UInt64)function << 12;
        UInt64 reg_offset  = (UInt64)(offset & 0xFCU);

        volatile UInt32* ecam_reg = (volatile UInt32*)(s_pci_ledger.ecam_virtual_base + bus_offset + slot_offset + func_offset + reg_offset);
        *value = *ecam_reg;
        return HW_STATUS_SUCCESS;
    }

    UInt32 address = HW_PCI_ADDRESS_ENABLE
                   | ((UInt32)bus << 16)
                   | ((UInt32)slot << 11)
                   | ((UInt32)function << 8)
                   | ((UInt32)offset & 0xFCU);

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_pci_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    HwIoOut32(HW_PCI_ADDRESS_PORT, address);
    *value = HwIoIn32(HW_PCI_DATA_PORT);

    __sync_lock_release(&s_pci_ledger.spin_lock);

    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwPciWriteConfig32(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt32 value)
{
    if (slot >= HW_PCI_MAX_SLOTS || function >= HW_PCI_MAX_FUNCTIONS)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    if (s_pci_ledger.ecam_virtual_base != 0 &&
        bus >= s_pci_ledger.ecam_start_bus &&
        bus <= s_pci_ledger.ecam_end_bus)
    {
        UInt64 bus_offset  = (UInt64)(bus - s_pci_ledger.ecam_start_bus) << 20;
        UInt64 slot_offset = (UInt64)slot << 15;
        UInt64 func_offset = (UInt64)function << 12;
        UInt64 reg_offset  = (UInt64)(offset & 0xFCU);

        volatile UInt32* ecam_reg = (volatile UInt32*)(s_pci_ledger.ecam_virtual_base + bus_offset + slot_offset + func_offset + reg_offset);
        *ecam_reg = value;
        return HW_STATUS_SUCCESS;
    }

    UInt32 address = HW_PCI_ADDRESS_ENABLE
                   | ((UInt32)bus << 16)
                   | ((UInt32)slot << 11)
                   | ((UInt32)function << 8)
                   | ((UInt32)offset & 0xFCU);

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_pci_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    HwIoOut32(HW_PCI_ADDRESS_PORT, address);
    HwIoOut32(HW_PCI_DATA_PORT, value);

    __sync_lock_release(&s_pci_ledger.spin_lock);

    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwPciGetDeviceCount(UInt32* count)
{
    if (count == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    *count = s_pci_ledger.device_count;

    return HW_STATUS_SUCCESS;
}

HwStatus HwPciGetDevice(HwPciDevice* device, UInt32 index)
{
    if (device == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    if (index >= s_pci_ledger.device_count)
    {
        return HW_STATUS_DEVICE_NOT_FOUND;
    }

    MmStatus mm_status = MmCopyVirtualMemory(
        device,
        &s_pci_ledger.devices[index],
        sizeof(HwPciDevice)
    );
    if (mm_status != MM_STATUS_SUCCESS)
    {
        return HW_STATUS_IO_FAILURE;
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwPciScanBus(UInt8 bus)
{
    if (s_pci_ledger.visited_buses[bus] != 0)
    {
        return HW_STATUS_SUCCESS;
    }

    s_pci_ledger.visited_buses[bus] = 1;

    for (UInt32 slot = 0; slot < HW_PCI_MAX_SLOTS; slot++)
    {
        UInt32 reg0 = 0;
        HwStatus status = HwPciReadConfig32(&reg0, bus, (UInt8)slot, 0, HW_PCI_OFFSET_VENDOR_ID);
        if (status != HW_STATUS_SUCCESS)
        {
            continue;
        }

        UInt16 vendor_id = (UInt16)(reg0 & 0xFFFFU);
        if (vendor_id == HW_PCI_VENDOR_INVALID)
        {
            continue;
        }

        HwPciProbeFunction(bus, (UInt8)slot, 0);

        UInt32 reg3 = 0;
        status = HwPciReadConfig32(&reg3, bus, (UInt8)slot, 0, 0x0CU);
        if (status != HW_STATUS_SUCCESS)
        {
            continue;
        }

        UInt8 header_type = (UInt8)((reg3 >> 16) & 0xFFU);
        if ((header_type & 0x80U) != 0)
        {
            for (UInt32 function = 1; function < HW_PCI_MAX_FUNCTIONS; function++)
            {
                HwPciProbeFunction(bus, (UInt8)slot, (UInt8)function);
            }
        }
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwPciProbeFunction(UInt8 bus, UInt8 slot, UInt8 function)
{
    if (s_pci_ledger.device_count >= HW_PCI_MAX_DEVICES)
    {
        return HW_STATUS_DEVICE_BUSY;
    }

    UInt32 reg0 = 0;
    HwStatus status = HwPciReadConfig32(&reg0, bus, slot, function, HW_PCI_OFFSET_VENDOR_ID);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt16 vendor_id = (UInt16)(reg0 & 0xFFFFU);
    UInt16 device_id = (UInt16)((reg0 >> 16) & 0xFFFFU);

    if (vendor_id == HW_PCI_VENDOR_INVALID)
    {
        return HW_STATUS_DEVICE_NOT_FOUND;
    }

    UInt32 reg2 = 0;
    status = HwPciReadConfig32(&reg2, bus, slot, function, HW_PCI_OFFSET_REVISION_ID);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 reg3 = 0;
    status = HwPciReadConfig32(&reg3, bus, slot, function, 0x0CU);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 reg15 = 0;
    status = HwPciReadConfig32(&reg15, bus, slot, function, HW_PCI_OFFSET_IRQ_LINE);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    HwPciDevice* device = &s_pci_ledger.devices[s_pci_ledger.device_count];

    device->bus         = bus;
    device->slot        = slot;
    device->function    = function;
    device->header_type = (UInt8)((reg3 >> 16) & 0xFFU);
    device->vendor_id   = vendor_id;
    device->device_id   = device_id;
    device->class_code  = (UInt8)((reg2 >> 24) & 0xFFU);
    device->subclass    = (UInt8)((reg2 >> 16) & 0xFFU);
    device->prog_if     = (UInt8)((reg2 >> 8) & 0xFFU);
    device->revision_id = (UInt8)(reg2 & 0xFFU);
    device->irq_line    = (UInt8)(reg15 & 0xFFU);
    device->irq_pin     = (UInt8)((reg15 >> 8) & 0xFFU);
    device->reserved    = 0;

    if ((device->header_type & 0x7FU) == 0x00U)
    {
        HwPciDecodeBars(device);
    }

    s_pci_ledger.device_count++;

    if (((device->header_type & 0x7FU) == 0x01U) ||
        (device->class_code == 0x06U && device->subclass == 0x04U))
    {
        UInt32 bus_reg = 0;
        if (HwPciReadConfig32(&bus_reg, bus, slot, function, HW_PCI_OFFSET_PRIMARY_BUS) == HW_STATUS_SUCCESS)
        {
            UInt8 secondary_bus = (UInt8)((bus_reg >> 8) & 0xFFU);
            if (secondary_bus != 0 && secondary_bus != bus)
            {
                HwPciScanBus(secondary_bus);
            }
        }
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwPciDecodeBars(HwPciDevice* device)
{
    if (device == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt32 original_cmd = 0;
    HwStatus status = HwPciReadConfig32(
        &original_cmd,
        device->bus,
        device->slot,
        device->function,
        HW_PCI_OFFSET_COMMAND
    );
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 disabled_cmd = original_cmd & ~(HW_PCI_COMMAND_IO_SPACE | HW_PCI_COMMAND_MEMORY_SPACE);
    HwPciWriteConfig32(device->bus, device->slot, device->function, HW_PCI_OFFSET_COMMAND, disabled_cmd);

    for (UInt32 i = 0; i < HW_PCI_MAX_BARS; i++)
    {
        UInt8  bar_offset = (UInt8)(HW_PCI_OFFSET_BAR0 + (i * 4U));
        UInt32 bar_low    = 0;

        HwPciReadConfig32(&bar_low, device->bus, device->slot, device->function, bar_offset);

        if (bar_low == 0)
        {
            continue;
        }

        if ((bar_low & 0x01U) != 0)
        {
            HwPciWriteConfig32(device->bus, device->slot, device->function, bar_offset, 0xFFFFFFFFU);

            UInt32 mask_low = 0;
            HwPciReadConfig32(&mask_low, device->bus, device->slot, device->function, bar_offset);
            HwPciWriteConfig32(device->bus, device->slot, device->function, bar_offset, bar_low);

            UInt32 masked = mask_low & 0xFFFFFFFCU;
            UInt32 base   = bar_low & 0xFFFFFFFCU;

            if (base != 0 && masked != 0)
            {
                device->bars[i].base_address    = (UInt64)base;
                device->bars[i].region_size     = (UInt64)((~masked) + 1U);
                device->bars[i].bar_type        = HW_PCI_BAR_TYPE_IO_PORT;
                device->bars[i].is_prefetchable = 0;
            }
        }
        else
        {
            UInt32 mem_type   = (bar_low >> 1) & 0x03U;
            UInt32 prefetch   = (bar_low >> 3) & 0x01U;

            if (mem_type == 0x02U && (i + 1U) < HW_PCI_MAX_BARS)
            {
                UInt8  high_offset = (UInt8)(bar_offset + 4U);
                UInt32 bar_high    = 0;

                HwPciReadConfig32(&bar_high, device->bus, device->slot, device->function, high_offset);

                HwPciWriteConfig32(device->bus, device->slot, device->function, bar_offset, 0xFFFFFFFFU);
                HwPciWriteConfig32(device->bus, device->slot, device->function, high_offset, 0xFFFFFFFFU);

                UInt32 mask_low  = 0;
                UInt32 mask_high = 0;
                HwPciReadConfig32(&mask_low, device->bus, device->slot, device->function, bar_offset);
                HwPciReadConfig32(&mask_high, device->bus, device->slot, device->function, high_offset);

                HwPciWriteConfig32(device->bus, device->slot, device->function, bar_offset, bar_low);
                HwPciWriteConfig32(device->bus, device->slot, device->function, high_offset, bar_high);

                UInt64 base = ((UInt64)bar_high << 32) | (UInt64)(bar_low & 0xFFFFFFF0U);
                UInt64 mask = ((UInt64)mask_high << 32) | (UInt64)(mask_low & 0xFFFFFFF0U);

                if (base != 0 && mask != 0)
                {
                    device->bars[i].base_address    = base;
                    device->bars[i].region_size     = (~mask) + 1ULL;
                    device->bars[i].bar_type        = HW_PCI_BAR_TYPE_MMIO64;
                    device->bars[i].is_prefetchable = prefetch;
                }

                i++;
            }
            else if (mem_type == 0x00U)
            {
                HwPciWriteConfig32(device->bus, device->slot, device->function, bar_offset, 0xFFFFFFFFU);

                UInt32 mask_low = 0;
                HwPciReadConfig32(&mask_low, device->bus, device->slot, device->function, bar_offset);
                HwPciWriteConfig32(device->bus, device->slot, device->function, bar_offset, bar_low);

                UInt32 base   = bar_low & 0xFFFFFFF0U;
                UInt32 masked = mask_low & 0xFFFFFFF0U;

                if (base != 0 && masked != 0)
                {
                    device->bars[i].base_address    = (UInt64)base;
                    device->bars[i].region_size     = (UInt64)((~masked) + 1U);
                    device->bars[i].bar_type        = HW_PCI_BAR_TYPE_MMIO32;
                    device->bars[i].is_prefetchable = prefetch;
                }
            }
        }
    }

    HwPciWriteConfig32(device->bus, device->slot, device->function, HW_PCI_OFFSET_COMMAND, original_cmd);

    return HW_STATUS_SUCCESS;
}
