// Made by Berkay

#include "mm_private.h"

static MmPmLedger s_pm_ledger;

void MmPmLedgerAllocateSpace(MmKernelLayout* layout, MmMemoryMap* map)
{
    UInt64 highest_physical_address = 0;

    for (UInt64 i = 0; i < map->count; i++)
    {
        MmMemoryMapEntry* entry = &map->entries[i];
        UInt64 region_top = entry->base_address + entry->region_size;

        if (region_top > highest_physical_address)
        {
            highest_physical_address = region_top;
        }
    }

    s_pm_ledger.total_pages = (highest_physical_address + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE;

    UInt64 ledger_bytes          = (s_pm_ledger.total_pages + 7) / 8;
    UInt64 kernel_physical_start = (layout->physical_base / MM_PM_PAGE_SIZE) * MM_PM_PAGE_SIZE;
    UInt64 kernel_physical_end   = ((layout->physical_base + layout->region_size + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE) * MM_PM_PAGE_SIZE;

    BkBootInfo boot_info = BkGetBootInfo();
    BkFramebuffer fb = BkGetFramebuffer();

    for (UInt64 i = 0; i < map->count; i++)
    {
        MmMemoryMapEntry* entry = &map->entries[i];

        if (entry->hardware_type == MM_MEMORY_MAP_TYPE_USABLE)
        {
            UInt64 candidate_base = ((entry->base_address + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE) * MM_PM_PAGE_SIZE;
            UInt64 region_top     = entry->base_address + entry->region_size;

            if (candidate_base < (32ULL * MM_PM_PAGE_SIZE))
            {
                candidate_base = 32ULL * MM_PM_PAGE_SIZE;
            }

            UInt8 adjusted = 1;
            while (adjusted != 0)
            {
                adjusted = 0;

                if (candidate_base < kernel_physical_end && (candidate_base + ledger_bytes) > kernel_physical_start)
                {
                    candidate_base = kernel_physical_end;
                    adjusted = 1;
                }

                if (fb.region_size > 0 && candidate_base < (fb.physical_base + fb.region_size) && (candidate_base + ledger_bytes) > fb.physical_base)
                {
                    candidate_base = ((fb.physical_base + fb.region_size + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE) * MM_PM_PAGE_SIZE;
                    adjusted = 1;
                }

                for (UInt32 m = 0; m < boot_info.module_count && m < 16; m++)
                {
                    if (boot_info.modules[m].physical_base != 0 && boot_info.modules[m].size != 0)
                    {
                        UInt64 mod_end = ((boot_info.modules[m].physical_base + boot_info.modules[m].size + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE) * MM_PM_PAGE_SIZE;
                        if (candidate_base < mod_end && (candidate_base + ledger_bytes) > boot_info.modules[m].physical_base)
                        {
                            candidate_base = mod_end;
                            adjusted = 1;
                        }
                    }
                }
            }

            if (region_top > candidate_base && (region_top - candidate_base) >= ledger_bytes)
            {
                s_pm_ledger.bitmap = (UInt8*)candidate_base;

                return;
            }
        }
    }

    BkPanicReport report =
    {
        .message  = "Insufficient continuous RAM to allocate physical ledger",
        .file     = __FILE__,
        .function = __func__,
        .line     = __LINE__
    };
    BkPanic(&report);
}

void MmPmLedgerPopulate(MmKernelLayout* layout, MmMemoryMap* map)
{
    UInt64 total_bytes = (s_pm_ledger.total_pages + 7) / 8;

    for (UInt64 i = 0; i < total_bytes; i++)
    {
        s_pm_ledger.bitmap[i] = 0xFF;
    }

    s_pm_ledger.free_pages        = 0;
    s_pm_ledger.last_scanned_page = 256;
    s_pm_ledger.spin_lock         = 0;

    for (UInt64 i = 0; i < map->count; i++)
    {
        MmMemoryMapEntry* entry = &map->entries[i];

        if (entry->hardware_type == MM_MEMORY_MAP_TYPE_USABLE)
        {
            UInt64 start_page = (entry->base_address + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE;
            UInt64 end_page   = (entry->base_address + entry->region_size) / MM_PM_PAGE_SIZE;

            for (UInt64 page = start_page; page < end_page; page++)
            {
                if (page < 32ULL)
                {
                    continue;
                }

                if (MmPmGetPageStatus(page) == 1)
                {
                    MmPmSetPageFree(page);
                    s_pm_ledger.free_pages++;
                }
            }
        }
    }

    UInt64 ledger_physical_address = (UInt64)s_pm_ledger.bitmap;
    UInt64 ledger_start_page       = ledger_physical_address / MM_PM_PAGE_SIZE;
    UInt64 ledger_end_page         = (ledger_physical_address + total_bytes + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE;

    for (UInt64 page = ledger_start_page; page < ledger_end_page; page++)
    {
        if (MmPmGetPageStatus(page) == 0)
        {
            MmPmSetPageUsed(page);
            s_pm_ledger.free_pages--;
        }
    }

    UInt64 kernel_start_page = layout->physical_base / MM_PM_PAGE_SIZE;
    UInt64 kernel_end_page   = (layout->physical_base + layout->region_size + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE;

    for (UInt64 page = kernel_start_page; page < kernel_end_page; page++)
    {
        if (MmPmGetPageStatus(page) == 0)
        {
            MmPmSetPageUsed(page);
            s_pm_ledger.free_pages--;
        }
    }

    BkBootInfo boot_info = BkGetBootInfo();
    for (UInt32 i = 0; i < boot_info.module_count && i < 16; i++)
    {
        if (boot_info.modules[i].physical_base != 0 && boot_info.modules[i].size != 0)
        {
            UInt64 mod_start_page = boot_info.modules[i].physical_base / MM_PM_PAGE_SIZE;
            UInt64 mod_end_page   = (boot_info.modules[i].physical_base + boot_info.modules[i].size + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE;

            for (UInt64 page = mod_start_page; page < mod_end_page; page++)
            {
                if (MmPmGetPageStatus(page) == 0)
                {
                    MmPmSetPageUsed(page);
                    s_pm_ledger.free_pages--;
                }
            }
        }
    }

    BkFramebuffer fb = BkGetFramebuffer();
    if (fb.physical_base != 0 && fb.region_size != 0)
    {
        UInt64 fb_start_page = fb.physical_base / MM_PM_PAGE_SIZE;
        UInt64 fb_end_page   = (fb.physical_base + fb.region_size + (MM_PM_PAGE_SIZE - 1)) / MM_PM_PAGE_SIZE;

        for (UInt64 page = fb_start_page; page < fb_end_page; page++)
        {
            if (page < s_pm_ledger.total_pages && MmPmGetPageStatus(page) == 0)
            {
                MmPmSetPageUsed(page);
                s_pm_ledger.free_pages--;
            }
        }
    }
}

void MmPmLedgerLoad(void)
{
    BkKernelLayout bk_layout = BkGetKernelLayout();
    BkMemoryMap    bk_map    = BkGetMemoryMap();

    MmKernelLayout* layout = (MmKernelLayout*)&bk_layout;
    MmMemoryMap*    map    = (MmMemoryMap*)&bk_map;

    MmPmLedgerAllocateSpace(layout, map);
    MmPmLedgerPopulate(layout, map);
}

UInt64 MmPmAllocatePhysicalRegion(UInt64 highest_acceptable_physical_address, UInt64 page_count)
{
    if (page_count == 0)
    {
        return 0;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_pm_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    if (s_pm_ledger.free_pages < page_count)
    {
        __sync_lock_release(&s_pm_ledger.spin_lock);
        if ((rflags & 0x0200ULL) != 0ULL)
        {
            __asm__ volatile ("sti" : : : "memory");
        }
        return 0;
    }

    UInt64 max_page = highest_acceptable_physical_address / MM_PM_PAGE_SIZE;
    if (max_page >= s_pm_ledger.total_pages)
    {
        max_page = s_pm_ledger.total_pages - 1;
    }

    if (max_page + 1 < page_count)
    {
        __sync_lock_release(&s_pm_ledger.spin_lock);
        if ((rflags & 0x0200ULL) != 0ULL)
        {
            __asm__ volatile ("sti" : : : "memory");
        }
        return 0;
    }

    UInt64 search_limit = max_page - page_count + 1;

    if (s_pm_ledger.last_scanned_page <= search_limit)
    {
        for (UInt64 i = s_pm_ledger.last_scanned_page; i <= search_limit; )
        {
            UInt64 free_run = 0;
            for (UInt64 j = 0; j < page_count; j++)
            {
                if (MmPmGetPageStatus(i + j) != 0)
                {
                    break;
                }
                free_run++;
            }

            if (free_run == page_count)
            {
                for (UInt64 j = 0; j < page_count; j++)
                {
                    MmPmSetPageUsed(i + j);
                }

                s_pm_ledger.free_pages -= page_count;
                s_pm_ledger.last_scanned_page = i + page_count;
                if (s_pm_ledger.last_scanned_page >= s_pm_ledger.total_pages)
                {
                    s_pm_ledger.last_scanned_page = 0;
                }

                __sync_lock_release(&s_pm_ledger.spin_lock);
                if ((rflags & 0x0200ULL) != 0ULL)
                {
                    __asm__ volatile ("sti" : : : "memory");
                }

                return i * MM_PM_PAGE_SIZE;
            }

            i += free_run + 1;
        }
    }

    if (s_pm_ledger.last_scanned_page > 32ULL)
    {
        UInt64 wrap_limit = (s_pm_ledger.last_scanned_page <= search_limit)
                            ? (s_pm_ledger.last_scanned_page - 1)
                            : search_limit;

        for (UInt64 i = 32ULL; i <= wrap_limit; )
        {
            UInt64 free_run = 0;
            for (UInt64 j = 0; j < page_count; j++)
            {
                if (MmPmGetPageStatus(i + j) != 0)
                {
                    break;
                }
                free_run++;
            }

            if (free_run == page_count)
            {
                for (UInt64 j = 0; j < page_count; j++)
                {
                    MmPmSetPageUsed(i + j);
                }

                s_pm_ledger.free_pages -= page_count;
                if (i >= 256ULL)
                {
                    s_pm_ledger.last_scanned_page = i + page_count;
                    if (s_pm_ledger.last_scanned_page >= s_pm_ledger.total_pages)
                    {
                        s_pm_ledger.last_scanned_page = 256ULL;
                    }
                }

                __sync_lock_release(&s_pm_ledger.spin_lock);
                if ((rflags & 0x0200ULL) != 0ULL)
                {
                    __asm__ volatile ("sti" : : : "memory");
                }

                return i * MM_PM_PAGE_SIZE;
            }

            i += free_run + 1;
        }
    }

    __sync_lock_release(&s_pm_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return 0;
}

void MmPmReleasePhysicalRegion(UInt64 physical_address, UInt64 page_count)
{
    if (physical_address == 0 || (physical_address % MM_PM_PAGE_SIZE) != 0 || page_count == 0)
    {
        return;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_pm_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    UInt64 page_index = physical_address / MM_PM_PAGE_SIZE;

    if (page_index >= s_pm_ledger.total_pages || page_count > (s_pm_ledger.total_pages - page_index))
    {
        __sync_lock_release(&s_pm_ledger.spin_lock);
        if ((rflags & 0x0200ULL) != 0ULL)
        {
            __asm__ volatile ("sti" : : : "memory");
        }
        return;
    }

    for (UInt64 i = 0; i < page_count; i++)
    {
        UInt64 current_page = page_index + i;

        if (MmPmGetPageStatus(current_page) != 0)
        {
            MmPmSetPageFree(current_page);
            s_pm_ledger.free_pages++;
        }
    }

    if (page_index >= 256ULL && page_index < s_pm_ledger.last_scanned_page)
    {
        s_pm_ledger.last_scanned_page = page_index;
    }

    __sync_lock_release(&s_pm_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }
}

void MmPmSetPageUsed(UInt64 page_index)
{
    if (page_index >= s_pm_ledger.total_pages)
    {
        return;
    }

    UInt64 byte_index = page_index / 8;
    UInt8  bit_offset = page_index % 8;

    s_pm_ledger.bitmap[byte_index] |= (UInt8)(1U << bit_offset);
}

void MmPmSetPageFree(UInt64 page_index)
{
    if (page_index == 0 || page_index >= s_pm_ledger.total_pages)
    {
        return;
    }

    UInt64 byte_index = page_index / 8;
    UInt8  bit_offset = page_index % 8;

    s_pm_ledger.bitmap[byte_index] &= (UInt8)~(1U << bit_offset);
}

UInt8 MmPmGetPageStatus(UInt64 page_index)
{
    if (page_index >= s_pm_ledger.total_pages)
    {
        return 1;
    }

    UInt64 byte_index = page_index / 8;
    UInt8  bit_offset = page_index % 8;

    return (s_pm_ledger.bitmap[byte_index] >> bit_offset) & 1U;
}

void MmPmRelocateBitmapToHhdm(void)
{
    if ((UInt64)s_pm_ledger.bitmap < MM_VM_HHDM_BASE)
    {
        s_pm_ledger.bitmap = (UInt8*)((UInt64)s_pm_ledger.bitmap + MM_VM_HHDM_BASE);
    }
}
