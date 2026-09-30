// Made by Berkay

#include "fs_private.h"

static FsNodLedger s_nod_ledger;

FsStatus FsNodInit(void)
{
    UInt8* raw = (UInt8*)&s_nod_ledger;
    for (UInt64 i = 0; i < sizeof(FsNodLedger); i++)
    {
        raw[i] = 0;
    }

    PsStatus ps_status = PsSpinlockCreate(&s_nod_ledger.lock_id);
    if (ps_status != PS_STATUS_SUCCESS)
    {
        return FS_STATUS_INTERNAL_ERROR;
    }

    s_nod_ledger.nodes[1].id            = 1;
    s_nod_ledger.nodes[1].volume_id      = 1;
    s_nod_ledger.nodes[1].parent_id      = 1;
    s_nod_ledger.nodes[1].first_child_id  = 0;
    s_nod_ledger.nodes[1].next_sibling_id = 0;
    s_nod_ledger.nodes[1].driver_id      = 1;
    s_nod_ledger.nodes[1].device_id      = 0;
    s_nod_ledger.nodes[1].type          = FS_NOD_TYPE_DIRECTORY;
    s_nod_ledger.nodes[1].flags         = FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM;
    s_nod_ledger.nodes[1].size          = 0;
    s_nod_ledger.nodes[1].capacity      = 0;
    s_nod_ledger.nodes[1].data_address   = 0;
    s_nod_ledger.nodes[1].is_buffer_owned = 0;
    s_nod_ledger.nodes[1].is_allocated   = 1;

    FsKitStringCopy((char*)s_nod_ledger.nodes[1].name, "\\", FS_NOD_NAME_CAPACITY);

    s_nod_ledger.node_count     = 1;
    s_nod_ledger.is_initialized = 1;

    FsVolSetRootNode(1, 1);

    UInt32 system_id = 0;
    FsNodCreate(&system_id, 1, "System", FS_NOD_TYPE_DIRECTORY, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM);

    UInt32 drivers_id = 0;
    FsNodCreate(&drivers_id, system_id, "Drivers", FS_NOD_TYPE_DIRECTORY, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM);

    UInt32 services_id = 0;
    FsNodCreate(&services_id, system_id, "Services", FS_NOD_TYPE_DIRECTORY, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM);

    UInt32 devices_id = 0;
    FsNodCreate(&devices_id, 1, "Devices", FS_NOD_TYPE_DIRECTORY, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM);

    UInt32 keyboard_id = 0;
    FsNodCreate(&keyboard_id, devices_id, "Keyboard", FS_NOD_TYPE_DIRECTORY, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM);

    UInt32 mouse_id = 0;
    FsNodCreate(&mouse_id, devices_id, "Mouse", FS_NOD_TYPE_DIRECTORY, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM);

    UInt32 display_id = 0;
    FsNodCreate(&display_id, devices_id, "Display", FS_NOD_TYPE_DIRECTORY, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM);

    UInt32 storage_id = 0;
    FsNodCreate(&storage_id, devices_id, "Storage", FS_NOD_TYPE_DIRECTORY, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM);

    BkBootInfo boot_info = BkGetBootInfo();
    for (UInt32 i = 0; i < boot_info.module_count && i < 16; i++)
    {
        if (boot_info.modules[i].physical_base == 0 || boot_info.modules[i].size == 0 || boot_info.modules[i].path[0] == '\0')
        {
            continue;
        }

        UInt32 current_id = 1;
        UInt64 index = 0;
        char token[FS_NOD_NAME_CAPACITY];
        const char* path = boot_info.modules[i].path;

        for (;;)
        {
            FsStatus token_status = FsKitExtractNextToken(path, &index, token, FS_NOD_NAME_CAPACITY);
            if (token_status != FS_STATUS_SUCCESS)
            {
                break;
            }

            UInt64 next_index = index;
            char next_token[FS_NOD_NAME_CAPACITY];
            FsStatus next_status = FsKitExtractNextToken(path, &next_index, next_token, FS_NOD_NAME_CAPACITY);

            if (next_status == FS_STATUS_END_OF_FILE)
            {
                UInt32 mod_node_id = 0;
                FsStatus create_status = FsNodCreate(&mod_node_id, current_id, token, FS_NOD_TYPE_FILE, FS_NOD_FLAG_READ);
                if (create_status == FS_STATUS_SUCCESS)
                {
                    FsNodBindBuffer(mod_node_id, (void*)(0xFFFF800000000000ULL + boot_info.modules[i].physical_base), boot_info.modules[i].size);
                }
                break;
            }
            else
            {
                UInt32 child_id = 0;
                FsStatus child_status = FsNodFindChild(&child_id, current_id, token);
                if (child_status != FS_STATUS_SUCCESS)
                {
                    FsStatus dir_status = FsNodCreate(&child_id, current_id, token, FS_NOD_TYPE_DIRECTORY, FS_NOD_FLAG_READ | FS_NOD_FLAG_WRITE | FS_NOD_FLAG_SYSTEM);
                    if (dir_status != FS_STATUS_SUCCESS)
                    {
                        break;
                    }
                }
                current_id = child_id;
            }
        }
    }

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodCreate(UInt32* node_id, UInt32 parent_id, const char* name, FsNodType type, UInt32 flags)
{
    if (node_id == NULL || name == NULL || parent_id == 0 || parent_id >= FS_NOD_MAX_NODES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_nod_ledger.nodes[parent_id].is_allocated == 0 || s_nod_ledger.nodes[parent_id].type != FS_NOD_TYPE_DIRECTORY)
    {
        return FS_STATUS_NOT_FOUND;
    }

    UInt32 existing_child = 0;
    if (FsNodFindChild(&existing_child, parent_id, name) == FS_STATUS_SUCCESS)
    {
        return FS_STATUS_ALREADY_EXISTS;
    }

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    UInt32 slot = 0;
    for (UInt32 i = 2; i < FS_NOD_MAX_NODES; i++)
    {
        if (s_nod_ledger.nodes[i].is_allocated == 0)
        {
            slot = i;
            break;
        }
    }

    if (slot == 0)
    {
        PsSpinlockRelease(s_nod_ledger.lock_id);
        return FS_STATUS_OUT_OF_RESOURCES;
    }

    s_nod_ledger.nodes[slot].id            = slot;
    s_nod_ledger.nodes[slot].volume_id      = s_nod_ledger.nodes[parent_id].volume_id;
    s_nod_ledger.nodes[slot].parent_id      = parent_id;
    s_nod_ledger.nodes[slot].first_child_id  = 0;
    s_nod_ledger.nodes[slot].next_sibling_id = 0;
    s_nod_ledger.nodes[slot].driver_id      = s_nod_ledger.nodes[parent_id].driver_id;
    s_nod_ledger.nodes[slot].device_id      = 0;
    s_nod_ledger.nodes[slot].type          = type;
    s_nod_ledger.nodes[slot].flags         = flags;
    s_nod_ledger.nodes[slot].size          = 0;
    s_nod_ledger.nodes[slot].capacity      = 0;
    s_nod_ledger.nodes[slot].data_address   = 0;
    s_nod_ledger.nodes[slot].is_buffer_owned = 0;
    s_nod_ledger.nodes[slot].is_allocated   = 1;

    FsKitStringCopy((char*)s_nod_ledger.nodes[slot].name, name, FS_NOD_NAME_CAPACITY);

    s_nod_ledger.node_count++;

    *node_id = slot;

    PsSpinlockRelease(s_nod_ledger.lock_id);

    FsNodAddChild(parent_id, slot);

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodDestroy(UInt32 node_id)
{
    if (node_id <= 1 || node_id >= FS_NOD_MAX_NODES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_nod_ledger.nodes[node_id].is_allocated == 0)
    {
        return FS_STATUS_NOT_FOUND;
    }

    if (s_nod_ledger.nodes[node_id].first_child_id != 0)
    {
        return FS_STATUS_BUSY;
    }

    FsNodRemoveChild(s_nod_ledger.nodes[node_id].parent_id, node_id);
    FsNodFreeBuffer(node_id);

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    s_nod_ledger.nodes[node_id].is_allocated = 0;
    s_nod_ledger.node_count--;

    PsSpinlockRelease(s_nod_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodGet(FsNodNode** node, UInt32 node_id)
{
    if (node == NULL || node_id == 0 || node_id >= FS_NOD_MAX_NODES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_nod_ledger.nodes[node_id].is_allocated == 0)
    {
        return FS_STATUS_NOT_FOUND;
    }

    *node = &s_nod_ledger.nodes[node_id];

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodGetPublic(FsNode* node, UInt32 node_id)
{
    if (node == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    FsNodNode* internal_node = NULL;
    FsStatus status = FsNodGet(&internal_node, node_id);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    return FsKitNodeToPublic(node, internal_node);
}

FsStatus FsNodResolvePath(UInt32* node_id, const char* path)
{
    if (node_id == NULL || path == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (path[0] == '\0')
    {
        return FS_STATUS_INVALID_PATH;
    }

    if (path[0] == '\\' && path[1] == '\0')
    {
        *node_id = 1;
        return FS_STATUS_SUCCESS;
    }

    UInt32 current_id = 1;
    UInt64 index = 0;
    char token[FS_NOD_NAME_CAPACITY];

    for (;;)
    {
        FsStatus token_status = FsKitExtractNextToken(path, &index, token, FS_NOD_NAME_CAPACITY);
        if (token_status == FS_STATUS_END_OF_FILE)
        {
            break;
        }

        if (token_status != FS_STATUS_SUCCESS)
        {
            return token_status;
        }

        UInt32 child_id = 0;
        FsStatus child_status = FsNodFindChild(&child_id, current_id, token);
        if (child_status != FS_STATUS_SUCCESS)
        {
            return FS_STATUS_NOT_FOUND;
        }

        current_id = child_id;
    }

    *node_id = current_id;

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodGetByPathPublic(FsNode* node, const char* path)
{
    if (node == NULL || path == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    UInt32 node_id = 0;
    FsStatus status = FsNodResolvePath(&node_id, path);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    return FsNodGetPublic(node, node_id);
}

FsStatus FsNodBindBuffer(UInt32 node_id, void* buffer, UInt64 size)
{
    if (node_id == 0 || node_id >= FS_NOD_MAX_NODES || buffer == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_nod_ledger.nodes[node_id].is_allocated == 0)
    {
        return FS_STATUS_NOT_FOUND;
    }

    if (s_nod_ledger.nodes[node_id].is_buffer_owned != 0 && s_nod_ledger.nodes[node_id].data_address != 0)
    {
        MmReleaseHeapBlock((void*)s_nod_ledger.nodes[node_id].data_address);
    }

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    s_nod_ledger.nodes[node_id].data_address   = (UInt64)buffer;
    s_nod_ledger.nodes[node_id].size          = size;
    s_nod_ledger.nodes[node_id].capacity      = size;
    s_nod_ledger.nodes[node_id].is_buffer_owned = 0;

    PsSpinlockRelease(s_nod_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodReadBuffer(UInt32 node_id, void* buffer, UInt64 buffer_capacity, UInt64* bytes_read)
{
    if (node_id == 0 || node_id >= FS_NOD_MAX_NODES || buffer == NULL || bytes_read == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_nod_ledger.nodes[node_id].is_allocated == 0)
    {
        return FS_STATUS_NOT_FOUND;
    }

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    UInt64 to_read = s_nod_ledger.nodes[node_id].size;
    if (to_read > buffer_capacity)
    {
        to_read = buffer_capacity;
    }

    if (to_read > 0 && s_nod_ledger.nodes[node_id].data_address != 0)
    {
        FsKitCopyMemory(buffer, (const void*)s_nod_ledger.nodes[node_id].data_address, to_read);
    }

    *bytes_read = to_read;

    PsSpinlockRelease(s_nod_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodWriteBuffer(UInt32 node_id, void* buffer, UInt64 size)
{
    if (node_id == 0 || node_id >= FS_NOD_MAX_NODES || buffer == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_nod_ledger.nodes[node_id].is_allocated == 0)
    {
        return FS_STATUS_NOT_FOUND;
    }

    if (s_nod_ledger.nodes[node_id].capacity < size || s_nod_ledger.nodes[node_id].is_buffer_owned == 0)
    {
        FsStatus status = FsNodAllocateBuffer(node_id, size);
        if (status != FS_STATUS_SUCCESS)
        {
            return status;
        }
    }

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    FsKitCopyMemory((void*)s_nod_ledger.nodes[node_id].data_address, buffer, size);
    s_nod_ledger.nodes[node_id].size = size;

    PsSpinlockRelease(s_nod_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodFindChild(UInt32* child_id, UInt32 parent_id, const char* name)
{
    if (child_id == NULL || name == NULL || parent_id == 0 || parent_id >= FS_NOD_MAX_NODES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    UInt32 current = s_nod_ledger.nodes[parent_id].first_child_id;

    while (current != 0)
    {
        if (s_nod_ledger.nodes[current].is_allocated != 0)
        {
            Int32 difference = 0;
            FsKitStringCompare(&difference, (const char*)s_nod_ledger.nodes[current].name, name);
            if (difference == 0)
            {
                *child_id = current;
                PsSpinlockRelease(s_nod_ledger.lock_id);
                return FS_STATUS_SUCCESS;
            }
        }

        current = s_nod_ledger.nodes[current].next_sibling_id;
    }

    PsSpinlockRelease(s_nod_ledger.lock_id);

    return FS_STATUS_NOT_FOUND;
}

FsStatus FsNodAddChild(UInt32 parent_id, UInt32 child_id)
{
    if (parent_id == 0 || parent_id >= FS_NOD_MAX_NODES || child_id == 0 || child_id >= FS_NOD_MAX_NODES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    s_nod_ledger.nodes[child_id].next_sibling_id = s_nod_ledger.nodes[parent_id].first_child_id;
    s_nod_ledger.nodes[parent_id].first_child_id = child_id;

    PsSpinlockRelease(s_nod_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodRemoveChild(UInt32 parent_id, UInt32 child_id)
{
    if (parent_id == 0 || parent_id >= FS_NOD_MAX_NODES || child_id == 0 || child_id >= FS_NOD_MAX_NODES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    UInt32 current = s_nod_ledger.nodes[parent_id].first_child_id;
    UInt32 previous = 0;

    while (current != 0)
    {
        if (current == child_id)
        {
            if (previous == 0)
            {
                s_nod_ledger.nodes[parent_id].first_child_id = s_nod_ledger.nodes[current].next_sibling_id;
            }
            else
            {
                s_nod_ledger.nodes[previous].next_sibling_id = s_nod_ledger.nodes[current].next_sibling_id;
            }

            s_nod_ledger.nodes[current].next_sibling_id = 0;

            PsSpinlockRelease(s_nod_ledger.lock_id);
            return FS_STATUS_SUCCESS;
        }

        previous = current;
        current = s_nod_ledger.nodes[current].next_sibling_id;
    }

    PsSpinlockRelease(s_nod_ledger.lock_id);

    return FS_STATUS_NOT_FOUND;
}

FsStatus FsNodAllocateBuffer(UInt32 node_id, UInt64 capacity)
{
    if (node_id == 0 || node_id >= FS_NOD_MAX_NODES || capacity == 0)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_nod_ledger.nodes[node_id].is_allocated == 0)
    {
        return FS_STATUS_NOT_FOUND;
    }

    void* new_buffer = NULL;
    MmStatus mm_status = MmAllocateHeapBlock(&new_buffer, capacity);
    if (mm_status != MM_STATUS_SUCCESS)
    {
        return FS_STATUS_OUT_OF_MEMORY;
    }

    if (s_nod_ledger.nodes[node_id].is_buffer_owned != 0 && s_nod_ledger.nodes[node_id].data_address != 0)
    {
        UInt64 copy_count = s_nod_ledger.nodes[node_id].size;
        if (copy_count > capacity)
        {
            copy_count = capacity;
        }

        if (copy_count > 0)
        {
            FsKitCopyMemory(new_buffer, (const void*)s_nod_ledger.nodes[node_id].data_address, copy_count);
        }

        MmReleaseHeapBlock((void*)s_nod_ledger.nodes[node_id].data_address);
    }

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    s_nod_ledger.nodes[node_id].data_address   = (UInt64)new_buffer;
    s_nod_ledger.nodes[node_id].capacity      = capacity;
    s_nod_ledger.nodes[node_id].is_buffer_owned = 1;

    PsSpinlockRelease(s_nod_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsNodFreeBuffer(UInt32 node_id)
{
    if (node_id == 0 || node_id >= FS_NOD_MAX_NODES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_nod_ledger.lock_id);

    if (s_nod_ledger.nodes[node_id].is_buffer_owned != 0 && s_nod_ledger.nodes[node_id].data_address != 0)
    {
        MmReleaseHeapBlock((void*)s_nod_ledger.nodes[node_id].data_address);
    }

    s_nod_ledger.nodes[node_id].data_address   = 0;
    s_nod_ledger.nodes[node_id].size          = 0;
    s_nod_ledger.nodes[node_id].capacity      = 0;
    s_nod_ledger.nodes[node_id].is_buffer_owned = 0;

    PsSpinlockRelease(s_nod_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}
