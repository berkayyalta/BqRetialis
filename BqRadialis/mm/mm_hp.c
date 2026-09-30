// Made by Berkay

#include "mm_private.h"

static MmHpLedger s_hp_ledger;

MmStatus MmHpLedgerAllocateSpace(void)
{
    UInt64 virtual_address = 0;
    UInt64 default_flags   = MM_VM_FLAG_PRESENT | MM_VM_FLAG_WRITABLE;

    MmStatus status = MmVmAllocateVirtualRegion(&virtual_address, MM_HP_INITIAL_PAGE_COUNT, default_flags);

    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    s_hp_ledger.head_block  = (MmHpBlock*)virtual_address;
    s_hp_ledger.total_bytes = MM_HP_INITIAL_PAGE_COUNT * MM_VM_PAGE_SIZE;
    s_hp_ledger.used_bytes  = 0;
    s_hp_ledger.free_bytes  = 0;
    s_hp_ledger.block_count = 0;
    s_hp_ledger.spin_lock   = 0;

    return MM_STATUS_SUCCESS;
}

MmStatus MmHpLedgerPopulate(void)
{
    if (s_hp_ledger.head_block == 0 || s_hp_ledger.total_bytes <= sizeof(MmHpBlock))
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64 usable_bytes = s_hp_ledger.total_bytes - sizeof(MmHpBlock);

    s_hp_ledger.head_block->size    = usable_bytes;
    s_hp_ledger.head_block->is_free = 1;
    s_hp_ledger.head_block->next    = 0;
    s_hp_ledger.head_block->prev    = 0;

    s_hp_ledger.used_bytes  = 0;
    s_hp_ledger.free_bytes  = usable_bytes;
    s_hp_ledger.block_count = 1;

    return MM_STATUS_SUCCESS;
}

MmStatus MmHpLedgerLoad(void)
{
    MmStatus status = MmHpLedgerAllocateSpace();

    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    return MmHpLedgerPopulate();
}

MmStatus MmHpAllocateBlock(void** heap_address, UInt64 byte_count)
{
    if (heap_address == 0 || byte_count == 0 || s_hp_ledger.head_block == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_hp_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    UInt64 aligned_size = ((byte_count + (MM_HP_ALIGNMENT - 1)) / MM_HP_ALIGNMENT) * MM_HP_ALIGNMENT;

    MmHpBlock* current = s_hp_ledger.head_block;

    while (current != 0)
    {
        if (current->is_free != 0 && current->size >= aligned_size)
        {
            break;
        }

        current = current->next;
    }

    if (current == 0)
    {
        MmStatus status = MmHpExpandSpace(aligned_size);

        if (status != MM_STATUS_SUCCESS)
        {
            __sync_lock_release(&s_hp_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }
            return status;
        }

        current = s_hp_ledger.head_block;

        while (current != 0)
        {
            if (current->is_free != 0 && current->size >= aligned_size)
            {
                break;
            }

            current = current->next;
        }

        if (current == 0)
        {
            __sync_lock_release(&s_hp_ledger.spin_lock);
            if ((rflags & 0x0200ULL) != 0ULL)
            {
                __asm__ volatile ("sti" : : : "memory");
            }
            return MM_STATUS_OUT_OF_MEMORY;
        }
    }

    MmHpSplitBlock(current, aligned_size);

    current->is_free = 0;
    s_hp_ledger.used_bytes += current->size;
    s_hp_ledger.free_bytes -= current->size;

    *heap_address = (void*)((UInt8*)current + sizeof(MmHpBlock));

    __sync_lock_release(&s_hp_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return MM_STATUS_SUCCESS;
}

MmStatus MmHpReallocateBlock(void** new_heap_address, void* old_heap_address, UInt64 byte_count)
{
    if (new_heap_address == 0 || byte_count == 0 || s_hp_ledger.head_block == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if (old_heap_address == 0)
    {
        return MmHpAllocateBlock(new_heap_address, byte_count);
    }

    if ((UInt64)old_heap_address < sizeof(MmHpBlock) || ((UInt64)old_heap_address % MM_HP_ALIGNMENT) != 0)
    {
        return MM_STATUS_UNALIGNED_ADDRESS;
    }

    MmHpBlock* old_block = (MmHpBlock*)((UInt8*)old_heap_address - sizeof(MmHpBlock));

    if (old_block->is_free != 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64 aligned_size = ((byte_count + (MM_HP_ALIGNMENT - 1)) / MM_HP_ALIGNMENT) * MM_HP_ALIGNMENT;

    if (old_block->size >= aligned_size)
    {
        *new_heap_address = old_heap_address;
        return MM_STATUS_SUCCESS;
    }

    void* allocated_address = 0;
    MmStatus status = MmHpAllocateBlock(&allocated_address, aligned_size);

    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    UInt8* dst = (UInt8*)allocated_address;
    UInt8* src = (UInt8*)old_heap_address;

    for (UInt64 i = 0; i < old_block->size; i++)
    {
        dst[i] = src[i];
    }

    MmHpReleaseBlock(old_heap_address);

    *new_heap_address = allocated_address;

    return MM_STATUS_SUCCESS;
}

MmStatus MmHpReleaseBlock(void* heap_address)
{
    if (heap_address == 0 || s_hp_ledger.head_block == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if ((UInt64)heap_address < sizeof(MmHpBlock) || ((UInt64)heap_address % MM_HP_ALIGNMENT) != 0)
    {
        return MM_STATUS_UNALIGNED_ADDRESS;
    }

    UInt64 rflags = 0;
    __asm__ volatile ("pushfq; popq %0; cli" : "=r"(rflags) : : "memory");

    while (__sync_lock_test_and_set(&s_hp_ledger.spin_lock, 1U) != 0U)
    {
        __asm__ volatile ("pause");
    }

    MmHpBlock* block = (MmHpBlock*)((UInt8*)heap_address - sizeof(MmHpBlock));

    if (block->is_free != 0)
    {
        __sync_lock_release(&s_hp_ledger.spin_lock);
        if ((rflags & 0x0200ULL) != 0ULL)
        {
            __asm__ volatile ("sti" : : : "memory");
        }
        return MM_STATUS_INVALID_ARGUMENT;
    }

    block->is_free = 1;
    s_hp_ledger.used_bytes -= block->size;
    s_hp_ledger.free_bytes += block->size;

    MmStatus status = MmHpMergeBlock(block);

    __sync_lock_release(&s_hp_ledger.spin_lock);
    if ((rflags & 0x0200ULL) != 0ULL)
    {
        __asm__ volatile ("sti" : : : "memory");
    }

    return status;
}

MmStatus MmHpExpandSpace(UInt64 byte_count)
{
    if (byte_count == 0 || s_hp_ledger.head_block == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64 required_bytes = byte_count + sizeof(MmHpBlock);
    UInt64 page_count     = (required_bytes + (MM_VM_PAGE_SIZE - 1)) / MM_VM_PAGE_SIZE;

    if (page_count < MM_HP_INITIAL_PAGE_COUNT)
    {
        page_count = MM_HP_INITIAL_PAGE_COUNT;
    }

    UInt64 virtual_address = 0;
    UInt64 default_flags   = MM_VM_FLAG_PRESENT | MM_VM_FLAG_WRITABLE;

    MmStatus status = MmVmAllocateVirtualRegion(&virtual_address, page_count, default_flags);

    if (status != MM_STATUS_SUCCESS)
    {
        return status;
    }

    UInt64 region_bytes = page_count * MM_VM_PAGE_SIZE;
    UInt64 usable_bytes = region_bytes - sizeof(MmHpBlock);

    MmHpBlock* new_block = (MmHpBlock*)virtual_address;
    new_block->size      = usable_bytes;
    new_block->is_free   = 1;
    new_block->next      = 0;
    new_block->prev      = 0;

    MmHpBlock* tail = s_hp_ledger.head_block;
    while (tail->next != 0)
    {
        tail = tail->next;
    }

    tail->next      = new_block;
    new_block->prev = tail;

    s_hp_ledger.total_bytes += region_bytes;
    s_hp_ledger.free_bytes  += usable_bytes;
    s_hp_ledger.block_count++;

    return MmHpMergeBlock(new_block);
}

MmStatus MmHpSplitBlock(MmHpBlock* block, UInt64 byte_count)
{
    if (block == 0 || byte_count == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    UInt64 aligned_size  = ((byte_count + (MM_HP_ALIGNMENT - 1)) / MM_HP_ALIGNMENT) * MM_HP_ALIGNMENT;
    UInt64 minimum_split = aligned_size + sizeof(MmHpBlock) + MM_HP_MINIMUM_BLOCK_SIZE;

    if (block->size < minimum_split)
    {
        return MM_STATUS_SUCCESS;
    }

    MmHpBlock* remainder_block = (MmHpBlock*)((UInt8*)block + sizeof(MmHpBlock) + aligned_size);
    remainder_block->size      = block->size - aligned_size - sizeof(MmHpBlock);
    remainder_block->is_free   = 1;
    remainder_block->next      = block->next;
    remainder_block->prev      = block;

    if (block->next != 0)
    {
        block->next->prev = remainder_block;
    }

    block->size = aligned_size;
    block->next = remainder_block;

    s_hp_ledger.free_bytes -= sizeof(MmHpBlock);
    s_hp_ledger.block_count++;

    return MM_STATUS_SUCCESS;
}

MmStatus MmHpMergeBlock(MmHpBlock* block)
{
    if (block == 0 || block->is_free == 0)
    {
        return MM_STATUS_INVALID_ARGUMENT;
    }

    if (block->next != 0 && block->next->is_free != 0)
    {
        UInt8* expected_next = (UInt8*)block + sizeof(MmHpBlock) + block->size;

        if ((UInt8*)block->next == expected_next)
        {
            MmHpBlock* next_block = block->next;

            block->size += sizeof(MmHpBlock) + next_block->size;
            block->next  = next_block->next;

            if (next_block->next != 0)
            {
                next_block->next->prev = block;
            }

            s_hp_ledger.free_bytes += sizeof(MmHpBlock);
            s_hp_ledger.block_count--;
        }
    }

    if (block->prev != 0 && block->prev->is_free != 0)
    {
        UInt8* expected_curr = (UInt8*)block->prev + sizeof(MmHpBlock) + block->prev->size;

        if ((UInt8*)block == expected_curr)
        {
            MmHpBlock* prev_block = block->prev;

            prev_block->size += sizeof(MmHpBlock) + block->size;
            prev_block->next  = block->next;

            if (block->next != 0)
            {
                block->next->prev = prev_block;
            }

            s_hp_ledger.free_bytes += sizeof(MmHpBlock);
            s_hp_ledger.block_count--;
        }
    }

    return MM_STATUS_SUCCESS;
}
