// Made by Berkay

#include "fs_private.h"

static FsFilLedger s_fil_ledger;

FsStatus FsFilInit(void)
{
    UInt8* raw = (UInt8*)&s_fil_ledger;
    for (UInt64 i = 0; i < sizeof(FsFilLedger); i++)
    {
        raw[i] = 0;
    }

    PsStatus ps_status = PsSpinlockCreate(&s_fil_ledger.lock_id);
    if (ps_status != PS_STATUS_SUCCESS)
    {
        return FS_STATUS_INTERNAL_ERROR;
    }

    s_fil_ledger.file_count     = 0;
    s_fil_ledger.is_initialized = 1;

    return FS_STATUS_SUCCESS;
}

FsStatus FsFilOpen(UInt32* file_id, const char* path, FsFilMode mode)
{
    if (file_id == NULL || path == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    UInt32 node_id = 0;
    FsStatus status = FsNodResolvePath(&node_id, path);

    if (status != FS_STATUS_SUCCESS)
    {
        if ((mode & FS_FIL_MODE_CREATE) != 0)
        {
            UInt64 length = 0;
            FsKitStringLength(&length, path);

            Int64 last_slash = -1;
            for (Int64 i = (Int64)length - 1; i >= 0; i--)
            {
                if (path[i] == '\\')
                {
                    last_slash = i;
                    break;
                }
            }

            UInt32 parent_id = 1;
            const char* filename = path;

            if (last_slash > 0)
            {
                char parent_path[FS_VOL_PATH_CAPACITY];
                for (Int64 i = 0; i < last_slash; i++)
                {
                    parent_path[i] = path[i];
                }
                parent_path[last_slash] = '\0';

                FsStatus parent_status = FsNodResolvePath(&parent_id, parent_path);
                if (parent_status != FS_STATUS_SUCCESS)
                {
                    return parent_status;
                }

                filename = &path[last_slash + 1];
            }
            else if (last_slash == 0)
            {
                parent_id = 1;
                filename = &path[1];
            }

            FsStatus create_status = FsNodCreate(&node_id, parent_id, filename, FS_NOD_TYPE_FILE, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE);
            if (create_status != FS_STATUS_SUCCESS)
            {
                return create_status;
            }
        }
        else
        {
            return status;
        }
    }

    return FsFilOpenByNode(file_id, node_id, mode);
}

FsStatus FsFilOpenByNode(UInt32* file_id, UInt32 node_id, FsFilMode mode)
{
    if (file_id == NULL || node_id == 0 || node_id >= FS_NOD_MAX_NODES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    FsNodNode* node = NULL;
    FsStatus status = FsNodGet(&node, node_id);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    UInt32 process_id = 0;
    PsProcessGetCurrentId(&process_id);

    PsSpinlockAcquire(s_fil_ledger.lock_id);

    UInt32 slot = 0;
    for (UInt32 i = 1; i < FS_FIL_MAX_FILES; i++)
    {
        if (s_fil_ledger.files[i].is_allocated == 0)
        {
            slot = i;
            break;
        }
    }

    if (slot == 0)
    {
        PsSpinlockRelease(s_fil_ledger.lock_id);
        return FS_STATUS_OUT_OF_RESOURCES;
    }

    s_fil_ledger.files[slot].id          = slot;
    s_fil_ledger.files[slot].node_id      = node_id;
    s_fil_ledger.files[slot].volume_id    = node->volume_id;
    s_fil_ledger.files[slot].process_id   = process_id;
    s_fil_ledger.files[slot].mode        = mode;
    s_fil_ledger.files[slot].state       = FS_FIL_STATE_OPEN;
    s_fil_ledger.files[slot].offset      = ((mode & FS_FIL_MODE_APPEND) != 0) ? node->size : 0;

    if ((mode & FS_FIL_MODE_TRUNCATE) != 0)
    {
        node->size = 0;
    }

    s_fil_ledger.files[slot].size        = node->size;
    s_fil_ledger.files[slot].is_allocated = 1;

    s_fil_ledger.file_count++;

    *file_id = slot;

    PsSpinlockRelease(s_fil_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsFilClose(UInt32 file_id)
{
    if (file_id == 0 || file_id >= FS_FIL_MAX_FILES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_fil_ledger.lock_id);

    if (s_fil_ledger.files[file_id].is_allocated == 0)
    {
        PsSpinlockRelease(s_fil_ledger.lock_id);
        return FS_STATUS_NOT_FOUND;
    }

    s_fil_ledger.files[file_id].is_allocated = 0;
    s_fil_ledger.files[file_id].state       = FS_FIL_STATE_CLOSED;
    s_fil_ledger.file_count--;

    PsSpinlockRelease(s_fil_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsFilRead(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_read)
{
    if (file_id == 0 || file_id >= FS_FIL_MAX_FILES || buffer == NULL || bytes_read == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_fil_ledger.files[file_id].is_allocated == 0 || s_fil_ledger.files[file_id].state != FS_FIL_STATE_OPEN)
    {
        return FS_STATUS_NOT_FOUND;
    }

    FsFilFile* file = &s_fil_ledger.files[file_id];

    FsNodNode* node = NULL;
    FsStatus node_status = FsNodGet(&node, file->node_id);
    if (node_status != FS_STATUS_SUCCESS)
    {
        return node_status;
    }

    if (node->type == FS_NOD_TYPE_DEVICE)
    {
        file->offset = 0;
    }

    if (file->offset >= node->size)
    {
        *bytes_read = 0;
        return FS_STATUS_END_OF_FILE;
    }

    UInt64 to_read = node->size - file->offset;
    if (to_read > byte_count)
    {
        to_read = byte_count;
    }

    if (to_read > 0 && node->data_address != 0)
    {
        FsKitCopyMemory(buffer, (const void*)(node->data_address + file->offset), to_read);
    }

    file->offset += to_read;
    *bytes_read   = to_read;

    return FS_STATUS_SUCCESS;
}

FsStatus FsFilWrite(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_written)
{
    if (file_id == 0 || file_id >= FS_FIL_MAX_FILES || buffer == NULL || bytes_written == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_fil_ledger.files[file_id].is_allocated == 0 || s_fil_ledger.files[file_id].state != FS_FIL_STATE_OPEN)
    {
        return FS_STATUS_NOT_FOUND;
    }

    FsFilFile* file = &s_fil_ledger.files[file_id];

    if ((file->mode & (FS_FIL_MODE_WRITE | FS_FIL_MODE_APPEND)) == 0)
    {
        return FS_STATUS_ACCESS_DENIED;
    }

    FsNodNode* node = NULL;
    FsStatus node_status = FsNodGet(&node, file->node_id);
    if (node_status != FS_STATUS_SUCCESS)
    {
        return node_status;
    }

    if (node->type == FS_NOD_TYPE_DEVICE)
    {
        file->offset = 0;
    }
    else if ((file->mode & FS_FIL_MODE_APPEND) != 0)
    {
        file->offset = node->size;
    }

    UInt64 required_capacity = file->offset + byte_count;
    if (required_capacity > node->capacity || node->is_buffer_owned == 0)
    {
        UInt64 new_capacity = (node->capacity > 0) ? (node->capacity * 2) : FS_NOD_DEFAULT_BUFFER_CAPACITY;
        if (new_capacity < required_capacity)
        {
            new_capacity = required_capacity + FS_NOD_DEFAULT_BUFFER_CAPACITY;
        }

        FsStatus allocate_status = FsNodAllocateBuffer(node->id, new_capacity);
        if (allocate_status != FS_STATUS_SUCCESS)
        {
            return allocate_status;
        }
    }

    FsKitCopyMemory((void*)(node->data_address + file->offset), buffer, byte_count);

    file->offset += byte_count;
    if (file->offset > node->size)
    {
        node->size = file->offset;
    }

    file->size     = node->size;
    *bytes_written = byte_count;

    return FS_STATUS_SUCCESS;
}

FsStatus FsFilSeekFile(UInt32 file_id, Int64 offset, FsFilSeek origin, UInt64* new_offset)
{
    if (file_id == 0 || file_id >= FS_FIL_MAX_FILES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_fil_ledger.files[file_id].is_allocated == 0 || s_fil_ledger.files[file_id].state != FS_FIL_STATE_OPEN)
    {
        return FS_STATUS_NOT_FOUND;
    }

    FsFilFile* file = &s_fil_ledger.files[file_id];
    Int64 target_offset = 0;

    if (origin == FS_FIL_SEEK_SET)
    {
        target_offset = offset;
    }
    else if (origin == FS_FIL_SEEK_CURRENT)
    {
        target_offset = (Int64)file->offset + offset;
    }
    else if (origin == FS_FIL_SEEK_END)
    {
        target_offset = (Int64)file->size + offset;
    }
    else
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (target_offset < 0)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    file->offset = (UInt64)target_offset;

    if (new_offset != NULL)
    {
        *new_offset = file->offset;
    }

    return FS_STATUS_SUCCESS;
}

FsStatus FsFilGet(FsFilFile** file, UInt32 file_id)
{
    if (file == NULL || file_id == 0 || file_id >= FS_FIL_MAX_FILES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_fil_ledger.files[file_id].is_allocated == 0)
    {
        return FS_STATUS_NOT_FOUND;
    }

    *file = &s_fil_ledger.files[file_id];

    return FS_STATUS_SUCCESS;
}

FsStatus FsFilGetPublic(FsFile* file, UInt32 file_id)
{
    if (file == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    FsFilFile* internal_file = NULL;
    FsStatus status = FsFilGet(&internal_file, file_id);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    return FsKitFileToPublic(file, internal_file);
}
