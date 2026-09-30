// Made by Berkay

#include "mm_private.h"

static MmVmLedger s_vm_ledger;

MmStatus MmVmLedgerAllocateSpace(void)
{
    UInt64 pml4_physical_address = MmPmAllocatePhysicalRegion(~0ULL, 1);

    if (pml4_physical_address == 0)
    {
        return MM_STATUS_OUT_OF_MEMORY;
    }

    MmVmPageTable* pml4_table = (MmVmPageTable*)pml4_physical_address;

    for (UInt64 i = 0; i < 512; i++)
    {
        pml4_table->entries[i] = 0ULL;
    }

    s_vm_ledger.pml4_physical_address  = pml4_physical_address;
    s_vm_ledger.virtual_base           = 0xFFFF900000000000ULL;
    s_vm_ledger.virtual_limit          = 0xFFFFFFFFFFE00000ULL;
    s_vm_ledger.next_free_virtual      = s_vm_ledger.virtual_base;
    s_vm_ledger.total_mapped_pages     = 0;
    s_vm_ledger.total_allocated_tables = 1;
    s_vm_ledger.free_region_count      = 0;
    s_vm_ledger.is_paging_active       = 0;
    s_vm_ledger.spin_lock              = 0;

    for (UInt32 i = 0; i < MM_VM_MAX_FREE_REGIONS; i++)
    {
        s_vm_ledger.free_regions[i].virtual_base = 0;
        s_vm_ledger.free_regions[i].page_count   = 0;
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmLedgerPopulate(MmKernelLayout* layout, MmMemoryMap* map)
{
    if (layout == 0 || layout->region_size == 0 || map == 0 || map->count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64 default_flags = MM_VM_FLAG_PRESENT | MM_VM_FLAG_WRITABLE;

    for (UInt64 i = 0; i < map->count; i++)
    {
        MmMemoryMapEntry* entry = &map->entries[i];

        if (entry->region_size == 0 ||
            entry->hardware_type == MM_MEMORY_MAP_TYPE_RESERVED ||
            entry->hardware_type == MM_MEMORY_MAP_TYPE_BAD)
        {
            continue;
        }

        UInt64 start_page = entry->base_address / MM_VM_PAGE_SIZE;
        UInt64 end_page   = (entry->base_address + entry->region_size + (MM_VM_PAGE_SIZE - 1)) / MM_VM_PAGE_SIZE;

        if (end_page <= start_page)
        {
            continue;
        }

        UInt64 hhdm_phys_base  = start_page * MM_VM_PAGE_SIZE;
        UInt64 hhdm_page_count = end_page - start_page;

        MmStatus status = MmVmMapVirtualRegion(
            MM_VM_HHDM_BASE + hhdm_phys_base,
            hhdm_phys_base,
            hhdm_page_count,
            default_flags
        );

        if (status != MM_STATUS_SUCCESS)
        {
            return status;
        }

        if (start_page == 0)
        {
            start_page = 1;
        }

        if (end_page <= start_page)
        {
            continue;
        }

        UInt64 aligned_base = start_page * MM_VM_PAGE_SIZE;
        UInt64 page_count   = end_page - start_page;

        status = MmVmMapVirtualRegion(aligned_base, aligned_base, page_count, default_flags);

        if (status != MM_STATUS_SUCCESS)
        {
            return status;
        }
    }

    BkFramebuffer fb = BkGetFramebuffer();
    if (fb.physical_base != 0 && fb.region_size != 0)
    {
        UInt64 fb_start_page = fb.physical_base / MM_VM_PAGE_SIZE;
        UInt64 fb_end_page   = (fb.physical_base + fb.region_size + (MM_VM_PAGE_SIZE - 1)) / MM_VM_PAGE_SIZE;
        UInt64 fb_phys_base  = fb_start_page * MM_VM_PAGE_SIZE;
        UInt64 fb_page_count = fb_end_page - fb_start_page;

        MmStatus fb_status = MmVmMapVirtualRegion(MM_VM_HHDM_BASE + fb_phys_base, fb_phys_base, fb_page_count, default_flags);
        if (fb_status != MM_STATUS_SUCCESS)
        {
            return fb_status;
        }

        fb_status = MmVmMapVirtualRegion(fb_phys_base, fb_phys_base, fb_page_count, default_flags);
        if (fb_status != MM_STATUS_SUCCESS)
        {
            return fb_status;
        }
    }

    UInt64 kernel_physical_start = (layout->physical_base / MM_VM_PAGE_SIZE) * MM_VM_PAGE_SIZE;
    UInt64 kernel_physical_end   = ((layout->physical_base + layout->region_size + (MM_VM_PAGE_SIZE - 1)) / MM_VM_PAGE_SIZE) * MM_VM_PAGE_SIZE;
    UInt64 kernel_page_count     = (kernel_physical_end - kernel_physical_start) / MM_VM_PAGE_SIZE;
    UInt64 kernel_virtual_start  = (layout->virtual_base / MM_VM_PAGE_SIZE) * MM_VM_PAGE_SIZE;

    MmStatus status = MmVmMapVirtualRegion(kernel_virtual_start, kernel_physical_start, kernel_page_count, default_flags);

    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    BkAcpiMadt* madt = BkGetAcpiMadt();

    if (madt != 0)
    {
        UInt64 lapic_phys  = (UInt64)madt->local_apic_address;
        UInt8* current_ptr = (UInt8*)(madt + 1);
        UInt8* end_ptr     = (UInt8*)madt + madt->header.length;

        while ((current_ptr + sizeof(BkAcpiMadtRecordHeader)) <= end_ptr)
        {
            BkAcpiMadtRecordHeader* record = (BkAcpiMadtRecordHeader*)current_ptr;

            if (record->record_length < sizeof(BkAcpiMadtRecordHeader) || (current_ptr + record->record_length) > end_ptr)
            {
                break;
            }

            if (record->record_type == BK_ACPI_MADT_TYPE_LOCAL_APIC_OVERRIDE &&
                record->record_length >= sizeof(BkAcpiMadtLocalApicOverride))
            {
                BkAcpiMadtLocalApicOverride* lapic_override = (BkAcpiMadtLocalApicOverride*)current_ptr;

                lapic_phys = lapic_override->local_apic_address;
                break;
            }

            current_ptr += record->record_length;
        }

        if (lapic_phys != 0)
        {
            UInt64 lapic_page  = (lapic_phys / MM_VM_PAGE_SIZE) * MM_VM_PAGE_SIZE;
            UInt64 lapic_flags = MM_VM_FLAG_PRESENT | MM_VM_FLAG_WRITABLE | MM_VM_FLAG_CACHE_DISABLE;

            status = MmVmMapVirtualRegion(lapic_page, lapic_page, 1, lapic_flags);

            if (status != MM_STATUS_SUCCESS)
            {
                return status;
            }

            status = MmVmMapVirtualRegion(MM_VM_HHDM_BASE + lapic_page, lapic_page, 1, lapic_flags);

            if (status != MM_STATUS_SUCCESS)
            {
                return status;
            }
        }
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmLedgerLoad(void)
{
    BkKernelLayout bk_layout = BkGetKernelLayout();
    BkMemoryMap    bk_map    = BkGetMemoryMap();

    MmKernelLayout* layout = (MmKernelLayout*)&bk_layout;
    MmMemoryMap*    map    = (MmMemoryMap*)&bk_map;

    MmStatus status = MmVmLedgerAllocateSpace();

    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    status = MmVmLedgerPopulate(layout, map);

    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    CpStatus cp_status = CpSetPageTable(s_vm_ledger.pml4_physical_address);

    if (cp_status != CP_STATUS_SUCCESS)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    s_vm_ledger.is_paging_active = 1;
    MmPmRelocateBitmapToHhdm();

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmAllocateVirtualRegion(UInt64* virtual_address, UInt64 page_count, UInt64 flags)
{
    if (virtual_address == 0 || page_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_vm_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    UInt64 region_size   = page_count * MM_VM_PAGE_SIZE;
    UInt64 base_virtual  = 0;
    UInt8  from_freelist = 0;
    UInt32 free_index    = 0;

    for (UInt32 idx = 0; idx < s_vm_ledger.free_region_count; idx++)
    {
        if (s_vm_ledger.free_regions[idx].page_count >= page_count)
        {
            base_virtual  = s_vm_ledger.free_regions[idx].virtual_base;
            from_freelist = 1;
            free_index    = idx;
            break;
        }
    }

    if (from_freelist == 0)
    {
        if (s_vm_ledger.next_free_virtual + region_size < s_vm_ledger.next_free_virtual ||
            s_vm_ledger.next_free_virtual + region_size > s_vm_ledger.virtual_limit)
        {
            __sync_lock_release(&s_vm_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }
            return MM_STATUS_OUT_OF_MEMORY;
        }

        base_virtual = s_vm_ledger.next_free_virtual;
    }

    for (UInt64 i = 0; i < page_count; i++)
    {
        UInt64 current_virtual = base_virtual + (i * MM_VM_PAGE_SIZE);
        UInt64 physical_page   = MmPmAllocatePhysicalRegion(~0ULL, 1);

        if (physical_page == 0)
        {
            for (UInt64 j = 0; j < i; j++)
            {
                UInt64 rollback_virt = base_virtual + (j * MM_VM_PAGE_SIZE);
                UInt64 rollback_phys = 0;

                if (MmVmGetPhysicalAddress(&rollback_phys, rollback_virt) == MM_STATUS_SUCCESS)
                {
                    MmVmUnmapPage(rollback_virt);
                    MmPmReleasePhysicalRegion(rollback_phys, 1);
                }
            }

            __sync_lock_release(&s_vm_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }
            return MM_STATUS_OUT_OF_MEMORY;
        }

        MmStatus status = MmVmMapPage(current_virtual, physical_page, flags);

        if (status != MM_STATUS_SUCCESS)
        {
            MmPmReleasePhysicalRegion(physical_page, 1);

            for (UInt64 j = 0; j < i; j++)
            {
                UInt64 rollback_virt = base_virtual + (j * MM_VM_PAGE_SIZE);
                UInt64 rollback_phys = 0;

                if (MmVmGetPhysicalAddress(&rollback_phys, rollback_virt) == MM_STATUS_SUCCESS)
                {
                    MmVmUnmapPage(rollback_virt);
                    MmPmReleasePhysicalRegion(rollback_phys, 1);
                }
            }

            __sync_lock_release(&s_vm_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }
            return status;
        }
    }

    if (from_freelist != 0)
    {
        if (s_vm_ledger.free_regions[free_index].page_count == page_count)
        {
            for (UInt32 k = free_index; (k + 1U) < s_vm_ledger.free_region_count; k++)
            {
                s_vm_ledger.free_regions[k] = s_vm_ledger.free_regions[k + 1U];
            }

            s_vm_ledger.free_region_count--;
        }
        else
        {
            s_vm_ledger.free_regions[free_index].virtual_base += region_size;
            s_vm_ledger.free_regions[free_index].page_count   -= page_count;
        }
    }
    else
    {
        s_vm_ledger.next_free_virtual += region_size;
    }

    *virtual_address = base_virtual;

    __sync_lock_release(&s_vm_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmReleaseVirtualRegion(UInt64 virtual_address, UInt64 page_count)
{
    if (page_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if ((virtual_address % MM_VM_PAGE_SIZE) != 0)
    {
        return MM_STATUS_UNALIGNED_ADDRESS;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_vm_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    for (UInt64 i = 0; i < page_count; i++)
    {
        UInt64 current_virtual  = virtual_address + (i * MM_VM_PAGE_SIZE);
        UInt64 physical_address = 0;

        MmStatus status = MmVmGetPhysicalAddress(&physical_address, current_virtual);

        if (status != MM_STATUS_SUCCESS)
        {
            __sync_lock_release(&s_vm_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }
            return status;
        }

        status = MmVmUnmapPage(current_virtual);

        if (status != MM_STATUS_SUCCESS)
        {
            __sync_lock_release(&s_vm_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }
            return status;
        }

        MmPmReleasePhysicalRegion(physical_address, 1);
    }

    UInt64 region_size = page_count * MM_VM_PAGE_SIZE;

    if ((virtual_address + region_size) == s_vm_ledger.next_free_virtual &&
        virtual_address >= s_vm_ledger.virtual_base)
    {
        s_vm_ledger.next_free_virtual = virtual_address;

        while (s_vm_ledger.free_region_count > 0)
        {
            UInt32 last_idx  = s_vm_ledger.free_region_count - 1U;
            UInt64 last_base = s_vm_ledger.free_regions[last_idx].virtual_base;
            UInt64 last_size = s_vm_ledger.free_regions[last_idx].page_count * MM_VM_PAGE_SIZE;

            if ((last_base + last_size) == s_vm_ledger.next_free_virtual)
            {
                s_vm_ledger.next_free_virtual = last_base;
                s_vm_ledger.free_region_count--;
            }
            else
            {
                break;
            }
        }

        __sync_lock_release(&s_vm_ledger.spin_lock);
        if ((rflags & 0x0200ULL) != 0ULL)
        {
            __asm__ volatile ("sti" : : : "memory");
        }

        return MM_STATUS_SUCCESS;
    }

    if (virtual_address >= s_vm_ledger.virtual_base &&
        virtual_address < s_vm_ledger.next_free_virtual &&
        s_vm_ledger.free_region_count < MM_VM_MAX_FREE_REGIONS)
    {
        UInt32 insert_idx = 0;

        while (insert_idx < s_vm_ledger.free_region_count &&
               s_vm_ledger.free_regions[insert_idx].virtual_base < virtual_address)
        {
            insert_idx++;
        }

        for (UInt32 k = s_vm_ledger.free_region_count; k > insert_idx; k--)
        {
            s_vm_ledger.free_regions[k] = s_vm_ledger.free_regions[k - 1U];
        }

        s_vm_ledger.free_regions[insert_idx].virtual_base = virtual_address;
        s_vm_ledger.free_regions[insert_idx].page_count   = page_count;
        s_vm_ledger.free_region_count++;

        for (UInt32 k = 0; (k + 1U) < s_vm_ledger.free_region_count; )
        {
            UInt64 curr_end = s_vm_ledger.free_regions[k].virtual_base +
                              (s_vm_ledger.free_regions[k].page_count * MM_VM_PAGE_SIZE);

            if (curr_end == s_vm_ledger.free_regions[k + 1U].virtual_base)
            {
                s_vm_ledger.free_regions[k].page_count += s_vm_ledger.free_regions[k + 1U].page_count;

                for (UInt32 m = k + 1U; (m + 1U) < s_vm_ledger.free_region_count; m++)
                {
                    s_vm_ledger.free_regions[m] = s_vm_ledger.free_regions[m + 1U];
                }

                s_vm_ledger.free_region_count--;
            }
            else
            {
                k++;
            }
        }
    }

    __sync_lock_release(&s_vm_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmMapVirtualRegion(UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags)
{
    if (page_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if ((virtual_address % MM_VM_PAGE_SIZE) != 0 || (physical_address % MM_VM_PAGE_SIZE) != 0)
    {
        return MM_STATUS_UNALIGNED_ADDRESS;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_vm_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    for (UInt64 i = 0; i < page_count; i++)
    {
        UInt64 current_virtual  = virtual_address + (i * MM_VM_PAGE_SIZE);
        UInt64 current_physical = physical_address + (i * MM_VM_PAGE_SIZE);

        MmStatus status = MmVmMapPage(current_virtual, current_physical, flags);

        if (status != MM_STATUS_SUCCESS)
        {
            for (UInt64 j = 0; j < i; j++)
            {
                MmVmUnmapPage(virtual_address + (j * MM_VM_PAGE_SIZE));
            }

            __sync_lock_release(&s_vm_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }

            return status;
        }
    }

    __sync_lock_release(&s_vm_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmUnmapVirtualRegion(UInt64 virtual_address, UInt64 page_count)
{
    if (page_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if ((virtual_address % MM_VM_PAGE_SIZE) != 0)
    {
        return MM_STATUS_UNALIGNED_ADDRESS;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_vm_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    for (UInt64 i = 0; i < page_count; i++)
    {
        UInt64 current_virtual = virtual_address + (i * MM_VM_PAGE_SIZE);

        MmStatus status = MmVmUnmapPage(current_virtual);

        if (status != MM_STATUS_SUCCESS)
        {
            __sync_lock_release(&s_vm_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }
            return status;
        }
    }

    __sync_lock_release(&s_vm_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmMapPage(UInt64 virtual_address, UInt64 physical_address, UInt64 flags)
{
    return MmVmMapPageInSpace(s_vm_ledger.pml4_physical_address, virtual_address, physical_address, flags);
}

MmStatus MmVmUnmapPage(UInt64 virtual_address)
{
    return MmVmUnmapPageInSpace(s_vm_ledger.pml4_physical_address, virtual_address);
}

MmStatus MmVmMapPageInSpace(UInt64 pml4_phys, UInt64 virtual_address, UInt64 physical_address, UInt64 flags)
{
    if ((virtual_address % MM_VM_PAGE_SIZE) != 0 || (physical_address % MM_VM_PAGE_SIZE) != 0)
    {
        return MM_STATUS_UNALIGNED_ADDRESS;
    }

    if (pml4_phys == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt32 pml4_index = (UInt32)(virtual_address >> 39) & 0x1FFU;
    UInt32 pdpt_index = (UInt32)(virtual_address >> 30) & 0x1FFU;
    UInt32 pd_index   = (UInt32)(virtual_address >> 21) & 0x1FFU;
    UInt32 pt_index   = (UInt32)(virtual_address >> 12) & 0x1FFU;

    UInt64 directory_flags = MM_VM_FLAG_PRESENT | MM_VM_FLAG_WRITABLE | (flags & MM_VM_FLAG_USER);

    MmVmPageTable* pml4_table = 0;
    MmVmPageTable* pdpt_table = 0;
    MmVmPageTable* pd_table   = 0;
    MmVmPageTable* pt_table   = 0;

    MmStatus status = MmVmPhysicalToVirtual((void**)&pml4_table, pml4_phys);
    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    status = MmVmGetOrCreateTable(&pdpt_table, pml4_table, pml4_index, directory_flags);
    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    status = MmVmGetOrCreateTable(&pd_table, pdpt_table, pdpt_index, directory_flags);
    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    status = MmVmGetOrCreateTable(&pt_table, pd_table, pd_index, directory_flags);
    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    if ((pt_table->entries[pt_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        s_vm_ledger.total_mapped_pages++;
    }

    pt_table->entries[pt_index] = (physical_address & MM_VM_MASK_FRAME) | (flags & MM_VM_MASK_FLAGS) | MM_VM_FLAG_PRESENT;

    return MmVmInvalidatePage(virtual_address);
}

MmStatus MmVmUnmapPageInSpace(UInt64 pml4_phys, UInt64 virtual_address)
{
    if ((virtual_address % MM_VM_PAGE_SIZE) != 0)
    {
        return MM_STATUS_UNALIGNED_ADDRESS;
    }

    if (pml4_phys == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt32 pml4_index = (UInt32)(virtual_address >> 39) & 0x1FFU;
    UInt32 pdpt_index = (UInt32)(virtual_address >> 30) & 0x1FFU;
    UInt32 pd_index   = (UInt32)(virtual_address >> 21) & 0x1FFU;
    UInt32 pt_index   = (UInt32)(virtual_address >> 12) & 0x1FFU;

    MmVmPageTable* pml4_table = 0;
    MmVmPhysicalToVirtual((void**)&pml4_table, pml4_phys);
    if ((pml4_table->entries[pml4_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64         pdpt_phys  = pml4_table->entries[pml4_index] & MM_VM_MASK_FRAME;
    MmVmPageTable* pdpt_table = 0;
    MmVmPhysicalToVirtual((void**)&pdpt_table, pdpt_phys);
    if ((pdpt_table->entries[pdpt_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64         pd_phys  = pdpt_table->entries[pdpt_index] & MM_VM_MASK_FRAME;
    MmVmPageTable* pd_table = 0;
    MmVmPhysicalToVirtual((void**)&pd_table, pd_phys);
    if ((pd_table->entries[pd_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64         pt_phys  = pd_table->entries[pd_index] & MM_VM_MASK_FRAME;
    MmVmPageTable* pt_table = 0;
    MmVmPhysicalToVirtual((void**)&pt_table, pt_phys);
    if ((pt_table->entries[pt_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    pt_table->entries[pt_index] = 0ULL;

    if (s_vm_ledger.total_mapped_pages > 0)
    {
        s_vm_ledger.total_mapped_pages--;
    }

    UInt8 is_pt_empty = 0;
    MmVmIsTableEmpty(&is_pt_empty, pt_table);

    if (is_pt_empty != 0)
    {
        pd_table->entries[pd_index] = 0ULL;
        MmPmReleasePhysicalRegion(pt_phys, 1);

        if (s_vm_ledger.total_allocated_tables > 1)
        {
            s_vm_ledger.total_allocated_tables--;
        }

        UInt8 is_pd_empty = 0;
        MmVmIsTableEmpty(&is_pd_empty, pd_table);

        if (is_pd_empty != 0)
        {
            pdpt_table->entries[pdpt_index] = 0ULL;
            MmPmReleasePhysicalRegion(pd_phys, 1);

            if (s_vm_ledger.total_allocated_tables > 1)
            {
                s_vm_ledger.total_allocated_tables--;
            }

            UInt8 is_pdpt_empty = 0;
            MmVmIsTableEmpty(&is_pdpt_empty, pdpt_table);

            if (is_pdpt_empty != 0)
            {
                pml4_table->entries[pml4_index] = 0ULL;
                MmPmReleasePhysicalRegion(pdpt_phys, 1);

                if (s_vm_ledger.total_allocated_tables > 1)
                {
                    s_vm_ledger.total_allocated_tables--;
                }
            }
        }
    }

    return MmVmInvalidatePage(virtual_address);
}

MmStatus MmVmGetPhysicalAddress(UInt64* physical_address, UInt64 virtual_address)
{
    if (physical_address == 0 || s_vm_ledger.pml4_physical_address == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt32 pml4_index  = (UInt32)(virtual_address >> 39) & 0x1FFU;
    UInt32 pdpt_index  = (UInt32)(virtual_address >> 30) & 0x1FFU;
    UInt32 pd_index    = (UInt32)(virtual_address >> 21) & 0x1FFU;
    UInt32 pt_index    = (UInt32)(virtual_address >> 12) & 0x1FFU;
    UInt64 page_offset = virtual_address & 0xFFFULL;

    MmVmPageTable* pml4_table = 0;
    MmVmPhysicalToVirtual((void**)&pml4_table, s_vm_ledger.pml4_physical_address);
    if ((pml4_table->entries[pml4_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    MmVmPageTable* pdpt_table = 0;
    MmVmPhysicalToVirtual((void**)&pdpt_table, pml4_table->entries[pml4_index] & MM_VM_MASK_FRAME);
    if ((pdpt_table->entries[pdpt_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    MmVmPageTable* pd_table = 0;
    MmVmPhysicalToVirtual((void**)&pd_table, pdpt_table->entries[pdpt_index] & MM_VM_MASK_FRAME);
    if ((pd_table->entries[pd_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    MmVmPageTable* pt_table = 0;
    MmVmPhysicalToVirtual((void**)&pt_table, pd_table->entries[pd_index] & MM_VM_MASK_FRAME);
    if ((pt_table->entries[pt_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    *physical_address = (pt_table->entries[pt_index] & MM_VM_MASK_FRAME) | page_offset;

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmGetOrCreateTable(MmVmPageTable** next_table, MmVmPageTable* parent_table, UInt32 entry_index, UInt64 flags)
{
    if (next_table == 0 || parent_table == 0 || entry_index >= 512)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if ((parent_table->entries[entry_index] & MM_VM_FLAG_PRESENT) != 0)
    {
        parent_table->entries[entry_index] |= (flags & MM_VM_MASK_FLAGS);
        return MmVmPhysicalToVirtual((void**)next_table, parent_table->entries[entry_index] & MM_VM_MASK_FRAME);
    }

    UInt64 new_table_physical = MmPmAllocatePhysicalRegion(~0ULL, 1);
    if (new_table_physical == 0)
    {
        return MM_STATUS_OUT_OF_MEMORY;
    }

    MmVmPageTable* created_table = 0;
    MmVmPhysicalToVirtual((void**)&created_table, new_table_physical);

    for (UInt64 i = 0; i < 512; i++)
    {
        created_table->entries[i] = 0ULL;
    }

    parent_table->entries[entry_index] = (new_table_physical & MM_VM_MASK_FRAME) | (flags & MM_VM_MASK_FLAGS) | MM_VM_FLAG_PRESENT;
    s_vm_ledger.total_allocated_tables++;

    *next_table = created_table;

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmInvalidatePage(UInt64 virtual_address)
{
    __asm__ volatile("invlpg (%0)" : : "r"(virtual_address) : "memory");

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmPhysicalToVirtual(void** virtual_address, UInt64 physical_address)
{
    if (virtual_address == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if (s_vm_ledger.is_paging_active != 0 && physical_address < MM_VM_HHDM_BASE)
    {
        *virtual_address = (void*)(physical_address + MM_VM_HHDM_BASE);
    }
    else
    {
        *virtual_address = (void*)physical_address;
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmIsTableEmpty(UInt8* is_empty, MmVmPageTable* table)
{
    if (is_empty == 0 || table == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    for (UInt32 i = 0; i < 512U; i++)
    {
        if ((table->entries[i] & MM_VM_FLAG_PRESENT) != 0ULL)
        {
            *is_empty = 0;
            return MM_STATUS_SUCCESS;
        }
    }

    *is_empty = 1;

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmCreateAddressSpace(UInt64* page_table_address)
{
    if (page_table_address == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64 phys = MmPmAllocatePhysicalRegion(~0ULL, 1);
    if (phys == 0)
    {
        return MM_STATUS_OUT_OF_MEMORY;
    }

    MmVmPageTable* new_pml4 = 0;
    MmVmPhysicalToVirtual((void**)&new_pml4, phys);

    MmVmPageTable* master_pml4 = 0;
    MmVmPhysicalToVirtual((void**)&master_pml4, s_vm_ledger.pml4_physical_address);

    for (int i = 0; i < 256; i++)
    {
        new_pml4->entries[i] = 0ULL;
    }
    for (int i = 256; i < 512; i++)
    {
        new_pml4->entries[i] = master_pml4->entries[i];
    }

    *page_table_address = phys;
    return MM_STATUS_SUCCESS;
}

MmStatus MmVmDestroyAddressSpace(UInt64 page_table_address)
{
    if (page_table_address == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    MmVmPageTable* pml4 = 0;
    MmStatus status = MmVmPhysicalToVirtual((void**)&pml4, page_table_address);
    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    for (UInt32 i = 0; i < 256; i++)
    {
        if ((pml4->entries[i] & MM_VM_FLAG_PRESENT) != 0ULL)
        {
            UInt64 pdpt_phys = pml4->entries[i] & MM_VM_MASK_FRAME;
            MmVmPageTable* pdpt = 0;
            if (MmVmPhysicalToVirtual((void**)&pdpt, pdpt_phys) == MM_STATUS_SUCCESS)
            {
                for (UInt32 j = 0; j < 512; j++)
                {
                    if ((pdpt->entries[j] & MM_VM_FLAG_PRESENT) != 0ULL)
                    {
                        if ((pdpt->entries[j] & MM_VM_FLAG_HUGE_PAGE) != 0ULL)
                        {
                            UInt64 frame_phys = pdpt->entries[j] & MM_VM_MASK_FRAME;
                            MmPmReleasePhysicalRegion(frame_phys, 512ULL * 512ULL);
                        }
                        else
                        {
                            UInt64 pd_phys = pdpt->entries[j] & MM_VM_MASK_FRAME;
                            MmVmPageTable* pd = 0;
                            if (MmVmPhysicalToVirtual((void**)&pd, pd_phys) == MM_STATUS_SUCCESS)
                            {
                                for (UInt32 k = 0; k < 512; k++)
                                {
                                    if ((pd->entries[k] & MM_VM_FLAG_PRESENT) != 0ULL)
                                    {
                                        if ((pd->entries[k] & MM_VM_FLAG_HUGE_PAGE) != 0ULL)
                                        {
                                            UInt64 frame_phys = pd->entries[k] & MM_VM_MASK_FRAME;
                                            MmPmReleasePhysicalRegion(frame_phys, 512ULL);
                                        }
                                        else
                                        {
                                            UInt64 pt_phys = pd->entries[k] & MM_VM_MASK_FRAME;
                                            MmVmPageTable* pt = 0;
                                            if (MmVmPhysicalToVirtual((void**)&pt, pt_phys) == MM_STATUS_SUCCESS)
                                            {
                                                for (UInt32 l = 0; l < 512; l++)
                                                {
                                                    if ((pt->entries[l] & MM_VM_FLAG_PRESENT) != 0ULL)
                                                    {
                                                        UInt64 frame_phys = pt->entries[l] & MM_VM_MASK_FRAME;
                                                        MmPmReleasePhysicalRegion(frame_phys, 1);
                                                    }
                                                }
                                            }
                                            MmPmReleasePhysicalRegion(pt_phys, 1);
                                        }
                                    }
                                }
                            }
                            MmPmReleasePhysicalRegion(pd_phys, 1);
                        }
                    }
                }
            }
            MmPmReleasePhysicalRegion(pdpt_phys, 1);
            pml4->entries[i] = 0ULL;
        }
    }

    MmPmReleasePhysicalRegion(page_table_address, 1);
    return MM_STATUS_SUCCESS;
}

MmStatus MmVmMapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 physical_address, UInt64 page_count, UInt64 flags)
{
    if (page_table_address == 0 || page_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if ((virtual_address % MM_VM_PAGE_SIZE) != 0 || (physical_address % MM_VM_PAGE_SIZE) != 0)
    {
        return MM_STATUS_UNALIGNED_ADDRESS;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_vm_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    for (UInt64 i = 0; i < page_count; i++)
    {
        UInt64 current_virtual  = virtual_address + (i * MM_VM_PAGE_SIZE);
        UInt64 current_physical = physical_address + (i * MM_VM_PAGE_SIZE);

        MmStatus status = MmVmMapPageInSpace(page_table_address, current_virtual, current_physical, flags);

        if (status != MM_STATUS_SUCCESS)
        {
            for (UInt64 j = 0; j < i; j++)
            {
                MmVmUnmapPageInSpace(page_table_address, virtual_address + (j * MM_VM_PAGE_SIZE));
            }

            __sync_lock_release(&s_vm_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }

            return status;
        }
    }

    __sync_lock_release(&s_vm_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmUnmapVirtualRegionInSpace(UInt64 page_table_address, UInt64 virtual_address, UInt64 page_count)
{
    if (page_table_address == 0 || page_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if ((virtual_address % MM_VM_PAGE_SIZE) != 0)
    {
        return MM_STATUS_UNALIGNED_ADDRESS;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_vm_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    for (UInt64 i = 0; i < page_count; i++)
    {
        UInt64 current_virtual = virtual_address + (i * MM_VM_PAGE_SIZE);

        MmStatus status = MmVmUnmapPageInSpace(page_table_address, current_virtual);

        if (status != MM_STATUS_SUCCESS)
        {
            __sync_lock_release(&s_vm_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }

            return status;
        }
    }

    __sync_lock_release(&s_vm_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmVmGetPhysicalAddressInSpace(UInt64* physical_address, UInt64 page_table_address, UInt64 virtual_address)
{
    if (physical_address == 0 || page_table_address == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt32 pml4_index  = (UInt32)(virtual_address >> 39) & 0x1FFU;
    UInt32 pdpt_index  = (UInt32)(virtual_address >> 30) & 0x1FFU;
    UInt32 pd_index    = (UInt32)(virtual_address >> 21) & 0x1FFU;
    UInt32 pt_index    = (UInt32)(virtual_address >> 12) & 0x1FFU;
    UInt64 page_offset = virtual_address & 0xFFFULL;

    MmVmPageTable* pml4_table = 0;
    MmStatus status = MmVmPhysicalToVirtual((void**)&pml4_table, page_table_address);
    if (status != MM_STATUS_SUCCESS || (pml4_table->entries[pml4_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    MmVmPageTable* pdpt_table = 0;
    status = MmVmPhysicalToVirtual((void**)&pdpt_table, pml4_table->entries[pml4_index] & MM_VM_MASK_FRAME);
    if (status != MM_STATUS_SUCCESS || (pdpt_table->entries[pdpt_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    MmVmPageTable* pd_table = 0;
    status = MmVmPhysicalToVirtual((void**)&pd_table, pdpt_table->entries[pdpt_index] & MM_VM_MASK_FRAME);
    if (status != MM_STATUS_SUCCESS || (pd_table->entries[pd_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    MmVmPageTable* pt_table = 0;
    status = MmVmPhysicalToVirtual((void**)&pt_table, pd_table->entries[pd_index] & MM_VM_MASK_FRAME);
    if (status != MM_STATUS_SUCCESS || (pt_table->entries[pt_index] & MM_VM_FLAG_PRESENT) == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    *physical_address = (pt_table->entries[pt_index] & MM_VM_MASK_FRAME) | page_offset;

    return MM_STATUS_SUCCESS;
}
