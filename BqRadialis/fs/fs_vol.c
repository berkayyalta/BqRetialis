// Made by Berkay

#include "fs_private.h"

static FsVolLedger s_vol_ledger;

FsStatus FsVolInit(void)
{
    UInt8* raw = (UInt8*)&s_vol_ledger;
    for (UInt64 i = 0; i < sizeof(FsVolLedger); i++)
    {
        raw[i] = 0;
    }

    PsStatus ps_status = PsSpinlockCreate(&s_vol_ledger.lock_id);
    if (ps_status != PS_STATUS_SUCCESS)
    {
        return FS_STATUS_INTERNAL_ERROR;
    }

    s_vol_ledger.volumes[1].id           = 1;
    s_vol_ledger.volumes[1].driver_id    = 1;
    s_vol_ledger.volumes[1].root_node_id = 1;
    s_vol_ledger.volumes[1].type         = FS_VOL_TYPE_RAMFS;
    s_vol_ledger.volumes[1].state        = FS_VOL_STATE_MOUNTED;
    s_vol_ledger.volumes[1].total_size   = 64ULL * 1024ULL * 1024ULL;
    s_vol_ledger.volumes[1].used_size    = 0;
    s_vol_ledger.volumes[1].is_allocated = 1;

    FsKitStringCopy((char*)s_vol_ledger.volumes[1].mount_path, "\\", FS_VOL_PATH_CAPACITY);

    s_vol_ledger.volume_count   = 1;
    s_vol_ledger.is_initialized = 1;

    return FS_STATUS_SUCCESS;
}

FsStatus FsVolMount(UInt32* volume_id, UInt32 driver_id, FsVolType type, const char* mount_path)
{
    if (volume_id == NULL || mount_path == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_vol_ledger.lock_id);

    for (UInt32 i = 1; i < FS_VOL_MAX_VOLUMES; i++)
    {
        if (s_vol_ledger.volumes[i].is_allocated != 0)
        {
            Int32 difference = 0;
            FsKitStringCompare(&difference, (const char*)s_vol_ledger.volumes[i].mount_path, mount_path);
            if (difference == 0)
            {
                PsSpinlockRelease(s_vol_ledger.lock_id);
                return FS_STATUS_ALREADY_EXISTS;
            }
        }
    }

    UInt32 slot = 0;
    for (UInt32 i = 2; i < FS_VOL_MAX_VOLUMES; i++)
    {
        if (s_vol_ledger.volumes[i].is_allocated == 0)
        {
            slot = i;
            break;
        }
    }

    if (slot == 0)
    {
        PsSpinlockRelease(s_vol_ledger.lock_id);
        return FS_STATUS_OUT_OF_RESOURCES;
    }

    s_vol_ledger.volumes[slot].id           = slot;
    s_vol_ledger.volumes[slot].driver_id    = driver_id;
    s_vol_ledger.volumes[slot].root_node_id = 0;
    s_vol_ledger.volumes[slot].type         = type;
    s_vol_ledger.volumes[slot].state        = FS_VOL_STATE_MOUNTED;
    s_vol_ledger.volumes[slot].total_size   = 64ULL * 1024ULL * 1024ULL;
    s_vol_ledger.volumes[slot].used_size    = 0;
    s_vol_ledger.volumes[slot].is_allocated = 1;

    FsKitStringCopy((char*)s_vol_ledger.volumes[slot].mount_path, mount_path, FS_VOL_PATH_CAPACITY);

    s_vol_ledger.volume_count++;

    *volume_id = slot;

    PsSpinlockRelease(s_vol_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsVolUnmount(UInt32 volume_id)
{
    if (volume_id <= 1 || volume_id >= FS_VOL_MAX_VOLUMES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_vol_ledger.lock_id);

    if (s_vol_ledger.volumes[volume_id].is_allocated == 0)
    {
        PsSpinlockRelease(s_vol_ledger.lock_id);
        return FS_STATUS_NOT_FOUND;
    }

    s_vol_ledger.volumes[volume_id].is_allocated = 0;
    s_vol_ledger.volumes[volume_id].state        = FS_VOL_STATE_UNMOUNTED;
    s_vol_ledger.volume_count--;

    PsSpinlockRelease(s_vol_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsVolGet(FsVolVolume** volume, UInt32 volume_id)
{
    if (volume == NULL || volume_id == 0 || volume_id >= FS_VOL_MAX_VOLUMES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_vol_ledger.volumes[volume_id].is_allocated == 0)
    {
        return FS_STATUS_NOT_FOUND;
    }

    *volume = &s_vol_ledger.volumes[volume_id];

    return FS_STATUS_SUCCESS;
}

FsStatus FsVolGetPublic(FsVolume* volume, UInt32 volume_id)
{
    if (volume == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    FsVolVolume* internal_volume = NULL;
    FsStatus status = FsVolGet(&internal_volume, volume_id);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    return FsKitVolumeToPublic(volume, internal_volume);
}

FsStatus FsVolFindByPath(FsVolVolume** volume, const char* path)
{
    if (volume == NULL || path == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_vol_ledger.lock_id);

    UInt32 best_id = 0;
    UInt64 best_length = 0;

    for (UInt32 i = 1; i < FS_VOL_MAX_VOLUMES; i++)
    {
        if (s_vol_ledger.volumes[i].is_allocated != 0)
        {
            UInt64 mount_len = 0;
            FsKitStringLength(&mount_len, (const char*)s_vol_ledger.volumes[i].mount_path);

            UInt8 matches = 1;
            for (UInt64 j = 0; j < mount_len; j++)
            {
                if (path[j] == '\0' || path[j] != (char)s_vol_ledger.volumes[i].mount_path[j])
                {
                    matches = 0;
                    break;
                }
            }

            if (matches != 0)
            {
                if (mount_len == 1 && s_vol_ledger.volumes[i].mount_path[0] == '\\')
                {
                    if (best_id == 0)
                    {
                        best_id     = i;
                        best_length = 1;
                    }
                }
                else if (path[mount_len] == '\\' || path[mount_len] == '\0')
                {
                    if (mount_len > best_length)
                    {
                        best_id     = i;
                        best_length = mount_len;
                    }
                }
            }
        }
    }

    if (best_id != 0)
    {
        *volume = &s_vol_ledger.volumes[best_id];
        PsSpinlockRelease(s_vol_ledger.lock_id);
        return FS_STATUS_SUCCESS;
    }

    PsSpinlockRelease(s_vol_ledger.lock_id);

    return FS_STATUS_NOT_FOUND;
}

FsStatus FsVolFindByPathPublic(FsVolume* volume, const char* path)
{
    if (volume == NULL || path == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    FsVolVolume* internal_volume = NULL;
    FsStatus status = FsVolFindByPath(&internal_volume, path);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    return FsKitVolumeToPublic(volume, internal_volume);
}

FsStatus FsVolSetRootNode(UInt32 volume_id, UInt32 root_node_id)
{
    if (volume_id == 0 || volume_id >= FS_VOL_MAX_VOLUMES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_vol_ledger.lock_id);

    if (s_vol_ledger.volumes[volume_id].is_allocated == 0)
    {
        PsSpinlockRelease(s_vol_ledger.lock_id);
        return FS_STATUS_NOT_FOUND;
    }

    s_vol_ledger.volumes[volume_id].root_node_id = root_node_id;

    PsSpinlockRelease(s_vol_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsVolUpdateUsage(UInt32 volume_id, Int64 delta_bytes)
{
    if (volume_id == 0 || volume_id >= FS_VOL_MAX_VOLUMES)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_vol_ledger.lock_id);

    if (s_vol_ledger.volumes[volume_id].is_allocated == 0)
    {
        PsSpinlockRelease(s_vol_ledger.lock_id);
        return FS_STATUS_NOT_FOUND;
    }

    if (delta_bytes >= 0)
    {
        s_vol_ledger.volumes[volume_id].used_size += (UInt64)delta_bytes;
    }
    else
    {
        UInt64 reduction = (UInt64)(-delta_bytes);
        if (s_vol_ledger.volumes[volume_id].used_size >= reduction)
        {
            s_vol_ledger.volumes[volume_id].used_size -= reduction;
        }
        else
        {
            s_vol_ledger.volumes[volume_id].used_size = 0;
        }
    }

    PsSpinlockRelease(s_vol_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}
