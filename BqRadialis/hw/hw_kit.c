// Made by Berkay

#include "hw_private.h"

HwStatus HwKitMapAcpiMmio(void)
{
    BkAcpiMadt* madt = BkGetAcpiMadt();

    if (madt != 0)
    {
        UInt64 ioapic_flags = MM_VIRTUAL_MEMORY_FLAG_PRESENT | MM_VIRTUAL_MEMORY_FLAG_WRITABLE | MM_VIRTUAL_MEMORY_FLAG_CACHE_DISABLE;
        UInt8* ptr          = (UInt8*)(madt + 1);
        UInt8* end          = (UInt8*)madt + madt->header.length;

        while (ptr < end)
        {
            if (ptr[0] == 1)
            {
                UInt32 ioapic_phys = *(UInt32*)(ptr + 4);
                UInt64 ioapic_page = (ioapic_phys / MM_VIRTUAL_MEMORY_PAGE_SIZE) * MM_VIRTUAL_MEMORY_PAGE_SIZE;

                MmMapVirtualRegion(ioapic_page, ioapic_page, 1, ioapic_flags);
            }

            ptr += ptr[1];
        }
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwKitResolveAcpiIrq(UInt32* gsi, UInt16* flags, UInt8 irq_source)
{
    if (gsi == 0 || flags == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    BkAcpiMadt* madt = BkGetAcpiMadt();
    if (madt == NULL)
    {
        *gsi   = (UInt32)irq_source;
        *flags = 0;
        return HW_STATUS_SUCCESS;
    }

    HwMadt* hw_madt    = (HwMadt*)madt;
    UInt8* current_ptr = (UInt8*)hw_madt + sizeof(HwMadt);
    UInt8* end_ptr     = (UInt8*)hw_madt + hw_madt->header.length;

    while ((current_ptr + sizeof(HwMadtRecordHeader)) <= end_ptr)
    {
        HwMadtRecordHeader* record = (HwMadtRecordHeader*)current_ptr;
        if (record->record_length < sizeof(HwMadtRecordHeader) || (current_ptr + record->record_length) > end_ptr)
        {
            break;
        }

        if (record->record_type == HW_MADT_TYPE_IRQ_OVERRIDE &&
            record->record_length >= sizeof(HwMadtIrqOverride))
        {
            HwMadtIrqOverride* iso = (HwMadtIrqOverride*)current_ptr;
            if (iso->irq_source == irq_source)
            {
                *gsi   = iso->gsi;
                *flags = iso->flags;
                return HW_STATUS_SUCCESS;
            }
        }

        current_ptr += record->record_length;
    }

    *gsi   = (UInt32)irq_source;
    *flags = 0;

    return HW_STATUS_SUCCESS;
}

HwStatus HwKitFindAcpiIoApicForGsi(UInt64* physical_base, UInt32* gsi_base, UInt8* io_apic_id, UInt32 gsi)
{
    if (physical_base == 0 || gsi_base == 0 || io_apic_id == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    BkAcpiMadt* madt = BkGetAcpiMadt();
    if (madt == NULL)
    {
        return HW_STATUS_DEVICE_NOT_FOUND;
    }

    HwMadt* hw_madt    = (HwMadt*)madt;
    UInt8* current_ptr = (UInt8*)hw_madt + sizeof(HwMadt);
    UInt8* end_ptr     = (UInt8*)hw_madt + hw_madt->header.length;

    UInt64 best_physical = 0;
    UInt32 best_gsi_base = 0;
    UInt8  best_id       = 0;
    UInt8  found         = 0;

    while ((current_ptr + sizeof(HwMadtRecordHeader)) <= end_ptr)
    {
        HwMadtRecordHeader* record = (HwMadtRecordHeader*)current_ptr;
        if (record->record_length < sizeof(HwMadtRecordHeader) || (current_ptr + record->record_length) > end_ptr)
        {
            break;
        }

        if (record->record_type == HW_MADT_TYPE_IO_APIC &&
            record->record_length >= sizeof(HwMadtIoApic))
        {
            HwMadtIoApic* io_apic = (HwMadtIoApic*)current_ptr;
            if (gsi >= io_apic->gsi_base)
            {
                if (found == 0 || io_apic->gsi_base >= best_gsi_base)
                {
                    best_physical = (UInt64)io_apic->io_apic_address;
                    best_gsi_base = io_apic->gsi_base;
                    best_id       = io_apic->io_apic_id;
                    found         = 1;
                }
            }
        }

        current_ptr += record->record_length;
    }

    if (found == 0)
    {
        return HW_STATUS_DEVICE_NOT_FOUND;
    }

    *physical_base = best_physical;
    *gsi_base      = best_gsi_base;
    *io_apic_id    = best_id;

    return HW_STATUS_SUCCESS;
}

HwStatus HwKitRouteAcpiIrq(UInt8 irq_source, UInt8 vector, UInt8 target_apic_id, UInt8 masked)
{
    if (vector < 32U)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt32 gsi   = 0;
    UInt16 flags = 0;

    HwStatus status = HwKitResolveAcpiIrq(&gsi, &flags, irq_source);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt64 ioapic_phys = 0;
    UInt32 gsi_base    = 0;
    UInt8  ioapic_id   = 0;

    status = HwKitFindAcpiIoApicForGsi(&ioapic_phys, &gsi_base, &ioapic_id, gsi);
    if (status != HW_STATUS_SUCCESS || ioapic_phys == 0 || gsi < gsi_base)
    {
        return HW_STATUS_DEVICE_NOT_FOUND;
    }

    UInt64 page_phys   = (ioapic_phys / 4096ULL) * 4096ULL;
    UInt64 page_virt   = 0xFFFF800000000000ULL + page_phys;
    UInt64 mapped_phys = 0;

    if (MmGetPhysicalAddress(&mapped_phys, page_virt) != MM_STATUS_SUCCESS)
    {
        UInt64 ioapic_flags = MM_VIRTUAL_MEMORY_FLAG_PRESENT | MM_VIRTUAL_MEMORY_FLAG_WRITABLE | MM_VIRTUAL_MEMORY_FLAG_CACHE_DISABLE;
        if (MmMapVirtualRegion(page_virt, page_phys, 1, ioapic_flags) != MM_STATUS_SUCCESS)
        {
            return HW_STATUS_IO_FAILURE;
        }
    }

    UInt64 ioapic_virt = 0xFFFF800000000000ULL + ioapic_phys;
    UInt32 pin         = gsi - gsi_base;
    UInt32 reg_low     = 0x10U + (pin * 2U);
    UInt32 reg_high    = reg_low + 1U;

    UInt32 low_val = (UInt32)vector;

    if ((flags & 0x0003U) == 0x0003U)
    {
        low_val |= (1U << 13);
    }

    if (((flags >> 2) & 0x0003U) == 0x0003U)
    {
        low_val |= (1U << 15);
    }

    if (masked != 0)
    {
        low_val |= (1U << 16);
    }

    UInt32 high_val = ((UInt32)target_apic_id) << 24;

    volatile UInt32* index_reg = (volatile UInt32*)(ioapic_virt + 0x00ULL);
    volatile UInt32* data_reg  = (volatile UInt32*)(ioapic_virt + 0x10ULL);

    *index_reg = reg_high;
    *data_reg  = high_val;

    *index_reg = reg_low;
    *data_reg  = low_val;

    return HW_STATUS_SUCCESS;
}

HwStatus HwKitReadPciConfig8(UInt8* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset)
{
    if (value == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt32 dword_value = 0;
    HwStatus status = HwPciReadConfig32(&dword_value, bus, slot, function, offset);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 byte_shift = ((UInt32)offset & 0x03U) * 8U;
    *value = (UInt8)((dword_value >> byte_shift) & 0xFFU);

    return HW_STATUS_SUCCESS;
}

HwStatus HwKitWritePciConfig8(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt8 value)
{
    UInt32 dword_value = 0;
    HwStatus status = HwPciReadConfig32(&dword_value, bus, slot, function, offset);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 byte_shift = ((UInt32)offset & 0x03U) * 8U;
    UInt32 byte_mask  = 0xFFU << byte_shift;

    dword_value = (dword_value & ~byte_mask) | ((UInt32)value << byte_shift);

    return HwPciWriteConfig32(bus, slot, function, offset, dword_value);
}

HwStatus HwKitReadPciConfig16(UInt16* value, UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset)
{
    if (value == 0 || (offset & 0x01U) != 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt32 dword_value = 0;
    HwStatus status = HwPciReadConfig32(&dword_value, bus, slot, function, offset);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 word_shift = ((UInt32)offset & 0x02U) * 8U;
    *value = (UInt16)((dword_value >> word_shift) & 0xFFFFU);

    return HW_STATUS_SUCCESS;
}

HwStatus HwKitWritePciConfig16(UInt8 bus, UInt8 slot, UInt8 function, UInt8 offset, UInt16 value)
{
    if ((offset & 0x01U) != 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt32 dword_value = 0;
    HwStatus status = HwPciReadConfig32(&dword_value, bus, slot, function, offset);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 word_shift = ((UInt32)offset & 0x02U) * 8U;
    UInt32 word_mask  = 0xFFFFU << word_shift;

    dword_value = (dword_value & ~word_mask) | ((UInt32)value << word_shift);

    return HwPciWriteConfig32(bus, slot, function, offset, dword_value);
}

HwStatus HwKitEnablePciDevice(UInt8 bus, UInt8 slot, UInt8 function)
{
    UInt32 command_reg = 0;
    HwStatus status = HwPciReadConfig32(&command_reg, bus, slot, function, HW_PCI_OFFSET_COMMAND);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    command_reg |= (HW_PCI_COMMAND_IO_SPACE | HW_PCI_COMMAND_MEMORY_SPACE | HW_PCI_COMMAND_BUS_MASTER);

    return HwPciWriteConfig32(bus, slot, function, HW_PCI_OFFSET_COMMAND, command_reg);
}

HwStatus HwKitFindPciCapability(UInt8* cap_offset, UInt8 bus, UInt8 slot, UInt8 function, UInt8 cap_id)
{
    if (cap_offset == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt16 status_reg = 0;
    HwStatus status = HwKitReadPciConfig16(&status_reg, bus, slot, function, HW_PCI_OFFSET_STATUS);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    if ((status_reg & 0x0010U) == 0)
    {
        return HW_STATUS_DEVICE_NOT_FOUND;
    }

    UInt8 ptr = 0;
    status = HwKitReadPciConfig8(&ptr, bus, slot, function, HW_PCI_OFFSET_CAP_PTR);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    ptr &= 0xFCU;

    for (UInt32 hops = 0; hops < 48U && ptr >= 0x40U; hops++)
    {
        UInt8 current_id = 0;
        UInt8 next_ptr   = 0;

        if (HwKitReadPciConfig8(&current_id, bus, slot, function, ptr) != HW_STATUS_SUCCESS)
        {
            break;
        }

        if (current_id == cap_id)
        {
            *cap_offset = ptr;
            return HW_STATUS_SUCCESS;
        }

        if (HwKitReadPciConfig8(&next_ptr, bus, slot, function, (UInt8)(ptr + 1U)) != HW_STATUS_SUCCESS)
        {
            break;
        }

        ptr = next_ptr & 0xFCU;
    }

    return HW_STATUS_DEVICE_NOT_FOUND;
}

HwStatus HwKitEnablePciMsi(UInt8 bus, UInt8 slot, UInt8 function, UInt8 vector, UInt8 target_apic_id)
{
    if (vector < 32U)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt8 cap_ptr = 0;
    HwStatus status = HwKitFindPciCapability(&cap_ptr, bus, slot, function, HW_PCI_CAP_ID_MSI);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt16 msg_ctrl = 0;
    status = HwKitReadPciConfig16(&msg_ctrl, bus, slot, function, (UInt8)(cap_ptr + 2U));
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 msg_addr = 0xFEE00000U | (((UInt32)target_apic_id) << 12);
    UInt16 msg_data = (UInt16)vector;

    HwPciWriteConfig32(bus, slot, function, (UInt8)(cap_ptr + 4U), msg_addr);

    if ((msg_ctrl & (1U << 7)) != 0)
    {
        HwPciWriteConfig32(bus, slot, function, (UInt8)(cap_ptr + 8U), 0U);
        HwKitWritePciConfig16(bus, slot, function, (UInt8)(cap_ptr + 12U), msg_data);
    }
    else
    {
        HwKitWritePciConfig16(bus, slot, function, (UInt8)(cap_ptr + 8U), msg_data);
    }

    msg_ctrl &= (UInt16)~(0x0070U);
    msg_ctrl |= 0x0001U;

    return HwKitWritePciConfig16(bus, slot, function, (UInt8)(cap_ptr + 2U), msg_ctrl);
}

HwStatus HwKitFindDeviceByClass(HwDevDevice* device, HwDevClass device_class, UInt32 occurrence)
{
    if (device == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt32 count = 0;
    HwStatus status = HwDevGetDeviceCount(&count);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 match_index = 0;

    for (UInt32 i = 0; i < count; i++)
    {
        HwDevDevice candidate;
        status = HwDevGetDevice(&candidate, i);
        if (status != HW_STATUS_SUCCESS)
        {
            continue;
        }

        if (candidate.device_class == device_class)
        {
            if (match_index == occurrence)
            {
                MmStatus mm_status = MmCopyVirtualMemory(device, &candidate, sizeof(HwDevDevice));
                if (mm_status != MM_STATUS_SUCCESS)
                {
                    return HW_STATUS_IO_FAILURE;
                }

                return HW_STATUS_SUCCESS;
            }

            match_index++;
        }
    }

    return HW_STATUS_DEVICE_NOT_FOUND;
}

HwStatus HwKitFindDeviceById(HwDevDevice* device, UInt16 vendor_id, UInt16 device_id, UInt32 occurrence)
{
    if (device == 0 || vendor_id == 0 || vendor_id == HW_PCI_VENDOR_INVALID)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt32 count = 0;
    HwStatus status = HwDevGetDeviceCount(&count);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 match_index = 0;

    for (UInt32 i = 0; i < count; i++)
    {
        HwDevDevice candidate;
        status = HwDevGetDevice(&candidate, i);
        if (status != HW_STATUS_SUCCESS)
        {
            continue;
        }

        if (candidate.bus_type == HW_DEV_BUS_PCI &&
            candidate.vendor_id == vendor_id &&
            candidate.device_id == device_id)
        {
            if (match_index == occurrence)
            {
                MmStatus mm_status = MmCopyVirtualMemory(device, &candidate, sizeof(HwDevDevice));
                if (mm_status != MM_STATUS_SUCCESS)
                {
                    return HW_STATUS_IO_FAILURE;
                }

                return HW_STATUS_SUCCESS;
            }

            match_index++;
        }
    }

    return HW_STATUS_DEVICE_NOT_FOUND;
}

HwStatus HwKitFindDeviceByLocation(HwDevDevice* device, UInt8 bus, UInt8 slot, UInt8 function)
{
    if (device == 0 || slot >= HW_PCI_MAX_SLOTS || function >= HW_PCI_MAX_FUNCTIONS)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt32 count = 0;
    HwStatus status = HwDevGetDeviceCount(&count);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    for (UInt32 i = 0; i < count; i++)
    {
        HwDevDevice candidate;
        status = HwDevGetDevice(&candidate, i);
        if (status != HW_STATUS_SUCCESS)
        {
            continue;
        }

        if (candidate.bus_type == HW_DEV_BUS_PCI &&
            candidate.bus == bus &&
            candidate.slot == slot &&
            candidate.function == function)
        {
            MmStatus mm_status = MmCopyVirtualMemory(device, &candidate, sizeof(HwDevDevice));
            if (mm_status != MM_STATUS_SUCCESS)
            {
                return HW_STATUS_IO_FAILURE;
            }

            return HW_STATUS_SUCCESS;
        }
    }

    return HW_STATUS_DEVICE_NOT_FOUND;
}

HwStatus HwKitFindDeviceResource(HwDevResource* resource, HwDevDevice* device, HwDevResourceType resource_type, UInt32 occurrence)
{
    if (resource == 0 || device == 0 || resource_type == HW_DEV_RESOURCE_NONE)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    UInt32 match_index = 0;

    for (UInt32 i = 0; i < device->resource_count && i < HW_DEV_MAX_RESOURCES; i++)
    {
        if (device->resources[i].resource_type == resource_type)
        {
            if (match_index == occurrence)
            {
                MmStatus mm_status = MmCopyVirtualMemory(
                    resource,
                    &device->resources[i],
                    sizeof(HwDevResource)
                );
                if (mm_status != MM_STATUS_SUCCESS)
                {
                    return HW_STATUS_IO_FAILURE;
                }

                return HW_STATUS_SUCCESS;
            }

            match_index++;
        }
    }

    return HW_STATUS_DEVICE_NOT_FOUND;
}
