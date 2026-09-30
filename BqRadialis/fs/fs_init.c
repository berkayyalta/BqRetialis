// Made by Berkay

#include "fs_private.h"

void FsLoad(void)
{
}

FsStatus FsInit(void)
{
    FsStatus status = FsDrvInit();
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    status = FsVolInit();
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    status = FsNodInit();
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    status = FsFilInit();
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    return FS_STATUS_SUCCESS;
}

FsStatus FsRegisterDriver(UInt32* driver_id, FsDriver* driver)
{
    return FsDrvRegister(driver_id, driver);
}

FsStatus FsUnregisterDriver(UInt32 driver_id)
{
    return FsDrvUnregister(driver_id);
}

FsStatus FsGetDriver(FsDriver* driver, UInt32 driver_id)
{
    return FsDrvGetPublic(driver, driver_id);
}

FsStatus FsFindDriverByType(FsDriver* driver, FsDriverType type)
{
    return FsDrvFindByTypePublic(driver, (FsDrvType)type);
}

FsStatus FsDriverRead(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read)
{
    return FsDrvInvokeRead(driver_id, device_id, offset, buffer, byte_count, bytes_read);
}

FsStatus FsDriverWrite(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written)
{
    return FsDrvInvokeWrite(driver_id, device_id, offset, buffer, byte_count, bytes_written);
}

FsStatus FsDriverControl(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument)
{
    return FsDrvInvokeControl(driver_id, device_id, control_code, argument);
}

FsStatus FsMountVolume(UInt32* volume_id, UInt32 driver_id, FsVolumeType type, const char* mount_path)
{
    return FsVolMount(volume_id, driver_id, (FsVolType)type, mount_path);
}

FsStatus FsUnmountVolume(UInt32 volume_id)
{
    return FsVolUnmount(volume_id);
}

FsStatus FsGetVolume(FsVolume* volume, UInt32 volume_id)
{
    return FsVolGetPublic(volume, volume_id);
}

FsStatus FsFindVolumeByPath(FsVolume* volume, const char* path)
{
    return FsVolFindByPathPublic(volume, path);
}

FsStatus FsCreateNode(UInt32* node_id, UInt32 parent_id, const char* name, FsNodeType type, UInt32 flags)
{
    return FsNodCreate(node_id, parent_id, name, (FsNodType)type, flags);
}

FsStatus FsDestroyNode(UInt32 node_id)
{
    return FsNodDestroy(node_id);
}

FsStatus FsGetNode(FsNode* node, UInt32 node_id)
{
    return FsNodGetPublic(node, node_id);
}

FsStatus FsResolvePath(UInt32* node_id, const char* path)
{
    return FsNodResolvePath(node_id, path);
}

FsStatus FsGetNodeByPath(FsNode* node, const char* path)
{
    return FsNodGetByPathPublic(node, path);
}

FsStatus FsBindNodeBuffer(UInt32 node_id, void* buffer, UInt64 size)
{
    return FsNodBindBuffer(node_id, buffer, size);
}

FsStatus FsReadNodeBuffer(UInt32 node_id, void* buffer, UInt64 buffer_capacity, UInt64* bytes_read)
{
    return FsNodReadBuffer(node_id, buffer, buffer_capacity, bytes_read);
}

FsStatus FsWriteNodeBuffer(UInt32 node_id, void* buffer, UInt64 size)
{
    return FsNodWriteBuffer(node_id, buffer, size);
}

FsStatus FsOpenFile(UInt32* file_id, const char* path, FsFileMode mode)
{
    return FsFilOpen(file_id, path, (FsFilMode)mode);
}

FsStatus FsOpenFileByNode(UInt32* file_id, UInt32 node_id, FsFileMode mode)
{
    return FsFilOpenByNode(file_id, node_id, (FsFilMode)mode);
}

FsStatus FsCloseFile(UInt32 file_id)
{
    return FsFilClose(file_id);
}

FsStatus FsReadFile(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_read)
{
    return FsFilRead(file_id, buffer, byte_count, bytes_read);
}

FsStatus FsWriteFile(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_written)
{
    return FsFilWrite(file_id, buffer, byte_count, bytes_written);
}

FsStatus FsSeekFile(UInt32 file_id, Int64 offset, FsFileSeek origin, UInt64* new_offset)
{
    return FsFilSeekFile(file_id, offset, (FsFilSeek)origin, new_offset);
}

FsStatus FsGetFile(FsFile* file, UInt32 file_id)
{
    return FsFilGetPublic(file, file_id);
}
