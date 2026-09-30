// Made by Berkay

#ifndef MM_HP_H
#define MM_HP_H

#include "../bk/bk_types.h"

#define MM_HP_ALIGNMENT          16ULL
#define MM_HP_INITIAL_PAGE_COUNT 16ULL
#define MM_HP_MINIMUM_BLOCK_SIZE 16ULL

struct MmHpBlock
{
    UInt64 size;
    UInt64 is_free;
    struct MmHpBlock* next;
    struct MmHpBlock* prev;
};

struct MmHpLedger
{
    struct MmHpBlock* head_block;
    UInt64            total_bytes;
    UInt64            used_bytes;
    UInt64            free_bytes;
    UInt64            block_count;
    volatile UInt32   spin_lock;
};

#endif
