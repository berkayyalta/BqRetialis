// Made by Berkay

#include "hw_private.h"

static HwDevLedger s_dev_ledger;

HwStatus HwDevLedgerAllocateSpace(void)
{
    MmStatus mm_status = MmZeroVirtualMemory(&s_dev_ledger, sizeof(HwDevLedger));
    if (mm_status != MM_STATUS_SUCCESS)
    {
        return HW_STATUS_IO_FAILURE;
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwDevLedgerPopulate(HwFramebuffer* framebuffer)
{
    HwStatus status = HwDevRegisterPlatformDevices();
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    if (framebuffer != 0 && framebuffer->physical_base != 0 && framebuffer->region_size != 0)
    {
        status = HwDevRegisterFramebuffer(framebuffer);
        if (status != HW_STATUS_SUCCESS)
        {
            return status;
        }
    }

    return HwDevRegisterPciDevices();
}

HwStatus HwDevLedgerLoad(void)
{
    HwStatus status = HwDevLedgerAllocateSpace();
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    BkFramebuffer bk_framebuffer = BkGetFramebuffer();
    HwFramebuffer* framebuffer   = (HwFramebuffer*)&bk_framebuffer;

    return HwDevLedgerPopulate(framebuffer);
}

HwStatus HwDevGetDeviceCount(UInt32* count)
{
    if (count == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    *count = s_dev_ledger.device_count;

    return HW_STATUS_SUCCESS;
}

HwStatus HwDevGetDevice(HwDevDevice* device, UInt32 index)
{
    if (device == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    if (index >= s_dev_ledger.device_count)
    {
        return HW_STATUS_DEVICE_NOT_FOUND;
    }

    MmStatus mm_status = MmCopyVirtualMemory(
        device,
        &s_dev_ledger.devices[index],
        sizeof(HwDevDevice)
    );
    if (mm_status != MM_STATUS_SUCCESS)
    {
        return HW_STATUS_IO_FAILURE;
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwDevRegisterPlatformDevices(void)
{
    if ((s_dev_ledger.device_count + 4U) > HW_DEV_MAX_COUNT)
    {
        return HW_STATUS_DEVICE_BUSY;
    }

    HwDevDevice* kbd = &s_dev_ledger.devices[s_dev_ledger.device_count];
    kbd->index          = s_dev_ledger.device_count;
    kbd->bus_type       = HW_DEV_BUS_PLATFORM;
    kbd->device_class   = HW_DEV_CLASS_INPUT;
    kbd->irq_line       = HW_DEV_PS2_KEYBOARD_IRQ;
    kbd->resource_count = 3U;

    kbd->resources[0].base_address  = HW_DEV_PS2_DATA_PORT;
    kbd->resources[0].region_size   = 1ULL;
    kbd->resources[0].resource_type = HW_DEV_RESOURCE_IO_PORT;
    kbd->resources[0].flags         = 0U;

    kbd->resources[1].base_address  = HW_DEV_PS2_COMMAND_PORT;
    kbd->resources[1].region_size   = 1ULL;
    kbd->resources[1].resource_type = HW_DEV_RESOURCE_IO_PORT;
    kbd->resources[1].flags         = 0U;

    kbd->resources[2].base_address  = HW_DEV_PS2_KEYBOARD_IRQ;
    kbd->resources[2].region_size   = 1ULL;
    kbd->resources[2].resource_type = HW_DEV_RESOURCE_IRQ;
    kbd->resources[2].flags         = 0U;

    s_dev_ledger.device_count++;

    HwDevDevice* mouse = &s_dev_ledger.devices[s_dev_ledger.device_count];
    mouse->index          = s_dev_ledger.device_count;
    mouse->bus_type       = HW_DEV_BUS_PLATFORM;
    mouse->device_class   = HW_DEV_CLASS_INPUT;
    mouse->irq_line       = HW_DEV_PS2_MOUSE_IRQ;
    mouse->resource_count = 3U;

    mouse->resources[0].base_address  = HW_DEV_PS2_DATA_PORT;
    mouse->resources[0].region_size   = 1ULL;
    mouse->resources[0].resource_type = HW_DEV_RESOURCE_IO_PORT;
    mouse->resources[0].flags         = 0U;

    mouse->resources[1].base_address  = HW_DEV_PS2_COMMAND_PORT;
    mouse->resources[1].region_size   = 1ULL;
    mouse->resources[1].resource_type = HW_DEV_RESOURCE_IO_PORT;
    mouse->resources[1].flags         = 0U;

    mouse->resources[2].base_address  = HW_DEV_PS2_MOUSE_IRQ;
    mouse->resources[2].region_size   = 1ULL;
    mouse->resources[2].resource_type = HW_DEV_RESOURCE_IRQ;
    mouse->resources[2].flags         = 0U;

    s_dev_ledger.device_count++;

    HwDevDevice* rtc = &s_dev_ledger.devices[s_dev_ledger.device_count];
    rtc->index          = s_dev_ledger.device_count;
    rtc->bus_type       = HW_DEV_BUS_PLATFORM;
    rtc->device_class   = HW_DEV_CLASS_TIMER_RTC;
    rtc->irq_line       = HW_DEV_RTC_IRQ;
    rtc->resource_count = 3U;

    rtc->resources[0].base_address  = HW_DEV_RTC_INDEX_PORT;
    rtc->resources[0].region_size   = 1ULL;
    rtc->resources[0].resource_type = HW_DEV_RESOURCE_IO_PORT;
    rtc->resources[0].flags         = 0U;

    rtc->resources[1].base_address  = HW_DEV_RTC_DATA_PORT;
    rtc->resources[1].region_size   = 1ULL;
    rtc->resources[1].resource_type = HW_DEV_RESOURCE_IO_PORT;
    rtc->resources[1].flags         = 0U;

    rtc->resources[2].base_address  = HW_DEV_RTC_IRQ;
    rtc->resources[2].region_size   = 1ULL;
    rtc->resources[2].resource_type = HW_DEV_RESOURCE_IRQ;
    rtc->resources[2].flags         = 0U;

    s_dev_ledger.device_count++;

    HwDevDevice* com1 = &s_dev_ledger.devices[s_dev_ledger.device_count];
    com1->index          = s_dev_ledger.device_count;
    com1->bus_type       = HW_DEV_BUS_PLATFORM;
    com1->device_class   = HW_DEV_CLASS_SERIAL_PORT;
    com1->irq_line       = HW_DEV_COM1_IRQ;
    com1->resource_count = 2U;

    com1->resources[0].base_address  = HW_DEV_COM1_BASE_PORT;
    com1->resources[0].region_size   = HW_DEV_COM1_PORT_COUNT;
    com1->resources[0].resource_type = HW_DEV_RESOURCE_IO_PORT;
    com1->resources[0].flags         = 0U;

    com1->resources[1].base_address  = HW_DEV_COM1_IRQ;
    com1->resources[1].region_size   = 1ULL;
    com1->resources[1].resource_type = HW_DEV_RESOURCE_IRQ;
    com1->resources[1].flags         = 0U;

    s_dev_ledger.device_count++;

    return HW_STATUS_SUCCESS;
}

HwStatus HwDevRegisterFramebuffer(HwFramebuffer* framebuffer)
{
    if (framebuffer == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    if (s_dev_ledger.device_count >= HW_DEV_MAX_COUNT)
    {
        return HW_STATUS_DEVICE_BUSY;
    }

    HwDevDevice* fb_dev = &s_dev_ledger.devices[s_dev_ledger.device_count];

    fb_dev->index          = s_dev_ledger.device_count;
    fb_dev->bus_type       = HW_DEV_BUS_FIRMWARE;
    fb_dev->device_class   = HW_DEV_CLASS_DISPLAY;
    fb_dev->irq_line       = 0xFFU;
    fb_dev->resource_count = 1U;

    fb_dev->resources[0].base_address  = framebuffer->physical_base;
    fb_dev->resources[0].region_size   = framebuffer->region_size;
    fb_dev->resources[0].resource_type = HW_DEV_RESOURCE_FRAMEBUFFER;
    fb_dev->resources[0].flags         = 1U;

    fb_dev->display.width  = framebuffer->width;
    fb_dev->display.height = framebuffer->height;
    fb_dev->display.pitch  = framebuffer->pitch;
    fb_dev->display.bpp    = framebuffer->bpp;

    s_dev_ledger.device_count++;

    return HW_STATUS_SUCCESS;
}

HwStatus HwDevRegisterPciDevices(void)
{
    UInt32 pci_count = 0;
    HwStatus status = HwPciGetDeviceCount(&pci_count);
    if (status != HW_STATUS_SUCCESS)
    {
        return status;
    }

    for (UInt32 i = 0; i < pci_count; i++)
    {
        if (s_dev_ledger.device_count >= HW_DEV_MAX_COUNT)
        {
            return HW_STATUS_DEVICE_BUSY;
        }

        HwPciDevice pci_dev;
        status = HwPciGetDevice(&pci_dev, i);
        if (status != HW_STATUS_SUCCESS)
        {
            continue;
        }

        HwDevClass dev_class = HW_DEV_CLASS_UNKNOWN;
        HwDevTranslatePciClass(&dev_class, pci_dev.class_code, pci_dev.subclass);

        HwDevDevice* entry = &s_dev_ledger.devices[s_dev_ledger.device_count];

        entry->index          = s_dev_ledger.device_count;
        entry->bus_type       = HW_DEV_BUS_PCI;
        entry->device_class   = dev_class;
        entry->vendor_id      = pci_dev.vendor_id;
        entry->device_id      = pci_dev.device_id;
        entry->bus            = pci_dev.bus;
        entry->slot           = pci_dev.slot;
        entry->function       = pci_dev.function;
        entry->class_code     = pci_dev.class_code;
        entry->subclass       = pci_dev.subclass;
        entry->prog_if        = pci_dev.prog_if;
        entry->revision_id    = pci_dev.revision_id;
        entry->irq_line       = pci_dev.irq_line;
        entry->resource_count = 0U;

        for (UInt32 bar_idx = 0; bar_idx < HW_PCI_MAX_BARS; bar_idx++)
        {
            if (pci_dev.bars[bar_idx].bar_type == HW_PCI_BAR_TYPE_NONE)
            {
                continue;
            }

            if (entry->resource_count >= HW_DEV_MAX_RESOURCES)
            {
                break;
            }

            UInt32 res_idx = entry->resource_count;
            entry->resources[res_idx].base_address = pci_dev.bars[bar_idx].base_address;
            entry->resources[res_idx].region_size  = pci_dev.bars[bar_idx].region_size;
            entry->resources[res_idx].flags        = pci_dev.bars[bar_idx].is_prefetchable;

            if (pci_dev.bars[bar_idx].bar_type == HW_PCI_BAR_TYPE_IO_PORT)
            {
                entry->resources[res_idx].resource_type = HW_DEV_RESOURCE_IO_PORT;
            }
            else
            {
                entry->resources[res_idx].resource_type = HW_DEV_RESOURCE_MMIO;
            }

            entry->resource_count++;
        }

        if (pci_dev.irq_line != 0U && pci_dev.irq_line != 0xFFU)
        {
            if (entry->resource_count < HW_DEV_MAX_RESOURCES)
            {
                UInt32 res_idx = entry->resource_count;
                entry->resources[res_idx].base_address  = (UInt64)pci_dev.irq_line;
                entry->resources[res_idx].region_size   = 1ULL;
                entry->resources[res_idx].resource_type = HW_DEV_RESOURCE_IRQ;
                entry->resources[res_idx].flags         = (UInt32)pci_dev.irq_pin;
                entry->resource_count++;
            }
        }

        s_dev_ledger.device_count++;
    }

    return HW_STATUS_SUCCESS;
}

HwStatus HwDevTranslatePciClass(HwDevClass* dev_class, UInt8 class_code, UInt8 subclass)
{
    if (dev_class == 0)
    {
        return HW_STATUS_INVALID_ARGUMENT;
    }

    switch (class_code)
    {
        case 0x01U:
            *dev_class = HW_DEV_CLASS_STORAGE;
            break;

        case 0x02U:
            *dev_class = HW_DEV_CLASS_NETWORK;
            break;

        case 0x03U:
            *dev_class = HW_DEV_CLASS_DISPLAY;
            break;

        case 0x04U:
            *dev_class = HW_DEV_CLASS_MULTIMEDIA;
            break;

        case 0x06U:
            *dev_class = HW_DEV_CLASS_BRIDGE;
            break;

        case 0x07U:
            if (subclass == 0x00U)
            {
                *dev_class = HW_DEV_CLASS_SERIAL_PORT;
            }
            else
            {
                *dev_class = HW_DEV_CLASS_UNKNOWN;
            }
            break;

        case 0x08U:
            if (subclass == 0x03U)
            {
                *dev_class = HW_DEV_CLASS_TIMER_RTC;
            }
            else
            {
                *dev_class = HW_DEV_CLASS_UNKNOWN;
            }
            break;

        case 0x09U:
            *dev_class = HW_DEV_CLASS_INPUT;
            break;

        case 0x0BU:
            *dev_class = HW_DEV_CLASS_PROCESSOR;
            break;

        case 0x0CU:
            *dev_class = HW_DEV_CLASS_SERIAL_BUS;
            break;

        default:
            *dev_class = HW_DEV_CLASS_UNKNOWN;
            break;
    }

    return HW_STATUS_SUCCESS;
}
