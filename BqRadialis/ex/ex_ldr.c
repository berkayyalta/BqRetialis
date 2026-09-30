// Made by Berkay

#include "ex_private.h"

static ExLdrLedger s_ldr_ledger;

ExStatus ExLdrInit(void)
{
    UInt8* raw = (UInt8*)&s_ldr_ledger;
    for (UInt64 i = 0; i < sizeof(ExLdrLedger); i++)
    {
        raw[i] = 0;
    }

    s_ldr_ledger.is_initialized = 1;

    return EX_STATUS_SUCCESS;
}

ExStatus ExLdrLoadExecutable(UInt32* process_id, const char* path, UInt32 is_driver)
{
    if (process_id == NULL || path == NULL)
    {
        return EX_STATUS_INVALID_ARGUMENT;
    }

    UInt32 file_id = 0;
    FsStatus fs_status = FsOpenFile(&file_id, path, FS_FILE_MODE_READ);
    if (fs_status != FS_STATUS_SUCCESS)
    {
        return EX_STATUS_NOT_FOUND;
    }

    ExLdrElfHeader elf_header;
    UInt64 bytes_read = 0;
    fs_status = FsReadFile(file_id, &elf_header, sizeof(ExLdrElfHeader), &bytes_read);
    if (fs_status != FS_STATUS_SUCCESS || bytes_read != sizeof(ExLdrElfHeader))
    {
        FsCloseFile(file_id);
        return EX_STATUS_INTERNAL_ERROR;
    }

    if (elf_header.ident[0] != 0x7F || elf_header.ident[1] != 'E' ||
        elf_header.ident[2] != 'L'  || elf_header.ident[3] != 'F' ||
        elf_header.ident[4] != 2    || elf_header.machine != 0x3E)
    {
        FsCloseFile(file_id);
        return EX_STATUS_INVALID_ARGUMENT;
    }

    UInt32 pid = 0;
    PsStatus ps_status = PsProcessCreate(&pid, PS_PROCESS_PRIVILEGE_USER, PS_PROCESS_PRIORITY_NORMAL);
    if (ps_status != PS_STATUS_SUCCESS)
    {
        FsCloseFile(file_id);
        return EX_STATUS_OUT_OF_MEMORY;
    }

    PsProcess process;
    ps_status = PsProcessGet(&process, pid);
    if (ps_status != PS_STATUS_SUCCESS || process.page_table_address == 0)
    {
        PsProcessDestroy(pid);
        FsCloseFile(file_id);
        return EX_STATUS_INTERNAL_ERROR;
    }

    UInt64 page_table = process.page_table_address;

    for (UInt16 i = 0; i < elf_header.phnum; i++)
    {
        UInt64 ph_offset = elf_header.phoff + ((UInt64)i * (UInt64)elf_header.phentsize);
        UInt64 new_offset = 0;
        FsSeekFile(file_id, (Int64)ph_offset, FS_FILE_SEEK_SET, &new_offset);

        ExLdrElfProgramHeader ph;
        fs_status = FsReadFile(file_id, &ph, sizeof(ExLdrElfProgramHeader), &bytes_read);
        if (fs_status != FS_STATUS_SUCCESS || bytes_read != sizeof(ExLdrElfProgramHeader))
        {
            continue;
        }

        if (ph.type != EX_LDR_SEGMENT_LOAD || ph.memsz == 0)
        {
            continue;
        }

        UInt64 vaddr_start = ph.vaddr & ~0xFFFULL;
        UInt64 vaddr_end   = (ph.vaddr + ph.memsz + 0xFFFULL) & ~0xFFFULL;
        UInt64 page_count  = (vaddr_end - vaddr_start) / 4096ULL;

        UInt64 phys_base = MmAllocatePhysicalRegion(~0ULL, page_count);
        if (phys_base == 0)
        {
            PsProcessDestroy(pid);
            FsCloseFile(file_id);
            return EX_STATUS_OUT_OF_MEMORY;
        }

        UInt8* dest = (UInt8*)(EX_LDR_HHDM_BASE + phys_base);
        for (UInt64 b = 0; b < page_count * 4096ULL; b++)
        {
            dest[b] = 0;
        }

        if (ph.filesz > 0)
        {
            FsSeekFile(file_id, (Int64)ph.offset, FS_FILE_SEEK_SET, &new_offset);
            UInt64 offset_in_page = ph.vaddr - vaddr_start;
            FsReadFile(file_id, dest + offset_in_page, ph.filesz, &bytes_read);
        }

        UInt64 map_flags = MM_VIRTUAL_MEMORY_FLAG_PRESENT | MM_VIRTUAL_MEMORY_FLAG_USER | MM_VIRTUAL_MEMORY_FLAG_WRITABLE;
        MmStatus mm_status = MmMapVirtualRegionInSpace(page_table, vaddr_start, phys_base, page_count, map_flags);
        if (mm_status != MM_STATUS_SUCCESS)
        {
            PsProcessDestroy(pid);
            FsCloseFile(file_id);
            return EX_STATUS_INTERNAL_ERROR;
        }
    }

    FsCloseFile(file_id);

    UInt64 stack_pages = EX_LDR_USER_STACK_PAGES;
    UInt64 stack_phys  = MmAllocatePhysicalRegion(~0ULL, stack_pages);
    if (stack_phys == 0)
    {
        PsProcessDestroy(pid);
        return EX_STATUS_OUT_OF_MEMORY;
    }

    UInt8* stack_dest = (UInt8*)(EX_LDR_HHDM_BASE + stack_phys);
    for (UInt64 b = 0; b < stack_pages * 4096ULL; b++)
    {
        stack_dest[b] = 0;
    }

    UInt64 stack_flags = MM_VIRTUAL_MEMORY_FLAG_PRESENT | MM_VIRTUAL_MEMORY_FLAG_USER | MM_VIRTUAL_MEMORY_FLAG_WRITABLE;
    MmStatus mm_status = MmMapVirtualRegionInSpace(page_table, EX_LDR_USER_STACK_BASE, stack_phys, stack_pages, stack_flags);
    if (mm_status != MM_STATUS_SUCCESS)
    {
        PsProcessDestroy(pid);
        return EX_STATUS_INTERNAL_ERROR;
    }

    UInt64 user_stack_top = EX_LDR_USER_STACK_BASE + (stack_pages * 4096ULL) - 8ULL;

    UInt32 thread_id = 0;
    ps_status = PsThreadCreate(&thread_id, pid, (PsThreadEntry)elf_header.entry, NULL, PS_THREAD_PRIORITY_NORMAL);
    if (ps_status != PS_STATUS_SUCCESS)
    {
        PsProcessDestroy(pid);
        return EX_STATUS_INTERNAL_ERROR;
    }

    PsContext thr_context;
    PsThreadGetContext(&thr_context, thread_id);
    thr_context.rip = elf_header.entry;
    thr_context.rsp = user_stack_top;
    PsThreadSetContext(thread_id, &thr_context);

    if (is_driver != 0 && pid < EX_LDR_MAX_PROCESSES)
    {
        s_ldr_ledger.is_driver_process[pid] = 1;
    }

    s_ldr_ledger.total_loaded++;
    *process_id = pid;

    return EX_STATUS_SUCCESS;
}

ExStatus ExLdrLaunchDriver(UInt32* process_id, const char* path)
{
    return ExLdrLoadExecutable(process_id, path, 1);
}

ExStatus ExLdrLaunchService(UInt32* process_id, const char* path)
{
    return ExLdrLoadExecutable(process_id, path, 0);
}

ExStatus ExLdrStartup(void)
{
    UInt32 pid = 0;

    ExLdrLaunchDriver(&pid, "\\System\\Drivers\\GopDisplay.elf");
    ExLdrLaunchDriver(&pid, "\\System\\Drivers\\Ps2Keyboard.elf");
    ExLdrLaunchDriver(&pid, "\\System\\Drivers\\Ps2Mouse.elf");
    ExLdrLaunchDriver(&pid, "\\System\\Drivers\\AtaStorage.elf");

    ExLdrLaunchService(&pid, "\\System\\Services\\Desktop.elf");

    return EX_STATUS_SUCCESS;
}

UInt8 ExLdrIsDriverProcess(UInt32 process_id)
{
    if (process_id >= EX_LDR_MAX_PROCESSES)
    {
        return 0;
    }

    return s_ldr_ledger.is_driver_process[process_id];
}
