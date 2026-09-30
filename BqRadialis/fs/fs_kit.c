// Made by Berkay

#include "fs_private.h"

FsStatus FsKitCopyMemory(void* destination, const void* source, UInt64 byte_count)
{
    if (destination == NULL || source == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    UInt8* dest = (UInt8*)destination;
    const UInt8* src = (const UInt8*)source;

    for (UInt64 i = 0; i < byte_count; i++)
    {
        dest[i] = src[i];
    }

    return FS_STATUS_SUCCESS;
}

FsStatus FsKitZeroMemory(void* destination, UInt64 byte_count)
{
    if (destination == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    UInt8* dest = (UInt8*)destination;

    for (UInt64 i = 0; i < byte_count; i++)
    {
        dest[i] = 0;
    }

    return FS_STATUS_SUCCESS;
}

FsStatus FsKitCompareMemory(UInt8* is_equal, const void* first, const void* second, UInt64 byte_count)
{
    if (is_equal == NULL || first == NULL || second == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    *is_equal = 1;

    const UInt8* a = (const UInt8*)first;
    const UInt8* b = (const UInt8*)second;

    for (UInt64 i = 0; i < byte_count; i++)
    {
        if (a[i] != b[i])
        {
            *is_equal = 0;
            break;
        }
    }

    return FS_STATUS_SUCCESS;
}

FsStatus FsKitStringLength(UInt64* length, const char* string)
{
    if (length == NULL || string == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    UInt64 len = 0;
    while (string[len] != '\0')
    {
        len++;
    }

    *length = len;

    return FS_STATUS_SUCCESS;
}

FsStatus FsKitStringCopy(char* destination, const char* source, UInt64 capacity)
{
    if (destination == NULL || source == NULL || capacity == 0)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    UInt64 i = 0;
    while (i < capacity - 1 && source[i] != '\0')
    {
        destination[i] = source[i];
        i++;
    }

    destination[i] = '\0';

    return FS_STATUS_SUCCESS;
}

FsStatus FsKitStringCompare(Int32* difference, const char* first, const char* second)
{
    if (difference == NULL || first == NULL || second == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    UInt64 i = 0;
    while (first[i] != '\0' && second[i] != '\0' && first[i] == second[i])
    {
        i++;
    }

    *difference = (Int32)((UInt8)first[i] - (UInt8)second[i]);

    return FS_STATUS_SUCCESS;
}

FsStatus FsKitExtractNextToken(const char* path, UInt64* current_index, char* token_buffer, UInt64 token_capacity)
{
    if (path == NULL || current_index == NULL || token_buffer == NULL || token_capacity == 0)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    UInt64 index = *current_index;

    while (path[index] == '\\')
    {
        index++;
    }

    if (path[index] == '\0')
    {
        return FS_STATUS_END_OF_FILE;
    }

    UInt64 token_len = 0;
    while (path[index] != '\0' && path[index] != '\\')
    {
        if (token_len < token_capacity - 1)
        {
            token_buffer[token_len] = path[index];
            token_len++;
        }
        index++;
    }

    token_buffer[token_len] = '\0';
    *current_index          = index;

    return FS_STATUS_SUCCESS;
}

FsStatus FsKitDriverToPublic(FsDriver* public_driver, FsDrvDriver* private_driver)
{
    if (public_driver == NULL || private_driver == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    public_driver->id      = private_driver->id;
    public_driver->type    = (FsDriverType)private_driver->type;
    public_driver->state   = (FsDriverState)private_driver->state;
    public_driver->read    = (FsDriverReadOperation)private_driver->read;
    public_driver->write   = (FsDriverWriteOperation)private_driver->write;
    public_driver->control = (FsDriverControlOperation)private_driver->control;

    return FsKitStringCopy((char*)public_driver->name, (const char*)private_driver->name, FS_DRIVER_NAME_CAPACITY);
}

FsStatus FsKitVolumeToPublic(FsVolume* public_volume, FsVolVolume* private_volume)
{
    if (public_volume == NULL || private_volume == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    public_volume->id           = private_volume->id;
    public_volume->driver_id    = private_volume->driver_id;
    public_volume->root_node_id = private_volume->root_node_id;
    public_volume->type         = (FsVolumeType)private_volume->type;
    public_volume->state        = (FsVolumeState)private_volume->state;
    public_volume->total_size   = private_volume->total_size;
    public_volume->used_size    = private_volume->used_size;

    return FsKitStringCopy((char*)public_volume->mount_path, (const char*)private_volume->mount_path, FS_VOLUME_PATH_CAPACITY);
}

FsStatus FsKitNodeToPublic(FsNode* public_node, FsNodNode* private_node)
{
    if (public_node == NULL || private_node == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    public_node->id           = private_node->id;
    public_node->volume_id    = private_node->volume_id;
    public_node->parent_id    = private_node->parent_id;
    public_node->driver_id    = private_node->driver_id;
    public_node->device_id    = private_node->device_id;
    public_node->type         = (FsNodeType)private_node->type;
    public_node->flags        = private_node->flags;
    public_node->size         = private_node->size;
    public_node->capacity     = private_node->capacity;
    public_node->data_address = private_node->data_address;

    return FsKitStringCopy((char*)public_node->name, (const char*)private_node->name, FS_NODE_NAME_CAPACITY);
}

FsStatus FsKitFileToPublic(FsFile* public_file, FsFilFile* private_file)
{
    if (public_file == NULL || private_file == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    public_file->id         = private_file->id;
    public_file->node_id    = private_file->node_id;
    public_file->volume_id  = private_file->volume_id;
    public_file->process_id = private_file->process_id;
    public_file->mode       = (FsFileMode)private_file->mode;
    public_file->state      = (FsFileState)private_file->state;
    public_file->offset     = private_file->offset;
    public_file->size       = private_file->size;

    return FS_STATUS_SUCCESS;
}
