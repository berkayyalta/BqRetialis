// Made by Berkay

#ifndef MM_PM_H
#define MM_PM_H

#include "../bk/bk_types.h"

#define MM_PM_PAGE_SIZE 4096ULL

struct MmPmLedger
{
    UInt8*          bitmap;
    UInt64          total_pages;
    UInt64          free_pages;
    UInt64          last_scanned_page;
    volatile UInt32 spin_lock;
};

#endif
