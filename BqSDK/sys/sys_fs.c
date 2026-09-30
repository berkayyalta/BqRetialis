// Made by Berkay

#include "sys_fs.h"

BqStatus BqRegisterDriver(const FsDriver* driver, UInt32* driver_id)
{
    if (driver == NULL || driver_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScRegisterDriverForm form;
    form.driver    = *driver;
    form.driver_id = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_REGISTER_DRIVER, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *driver_id = form.driver_id;
    }

    return status;
}

BqStatus BqUnregisterDriver(UInt32 driver_id)
{
    AbiScUnregisterDriverForm form;
    form.driver_id = driver_id;
    return BqSyscall(ABI_SC_FS_UNREGISTER_DRIVER, &form);
}

BqStatus BqGetDriver(UInt32 driver_id, FsDriver* driver)
{
    if (driver == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetDriverForm form;
    form.driver_id = driver_id;
    BqStatus status = BqSyscall(ABI_SC_FS_GET_DRIVER, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *driver = form.driver;
    }

    return status;
}

BqStatus BqFindDriverByType(FsDriverType type, FsDriver* driver)
{
    if (driver == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindDriverByTypeForm form;
    form.type = type;
    BqStatus status = BqSyscall(ABI_SC_FS_FIND_DRIVER_BY_TYPE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *driver = form.driver;
    }

    return status;
}

BqStatus BqDriverRead(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read)
{
    if (buffer == NULL || bytes_read == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScDriverReadForm form;
    form.driver_id  = driver_id;
    form.device_id  = device_id;
    form.offset     = offset;
    form.buffer     = buffer;
    form.byte_count = byte_count;
    form.bytes_read = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_DRIVER_READ, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *bytes_read = form.bytes_read;
    }

    return status;
}

BqStatus BqDriverWrite(UInt32 driver_id, UInt32 device_id, UInt64 offset, const void* buffer, UInt64 byte_count, UInt64* bytes_written)
{
    if (buffer == NULL || bytes_written == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScDriverWriteForm form;
    form.driver_id     = driver_id;
    form.device_id     = device_id;
    form.offset        = offset;
    form.buffer        = (void*)buffer;
    form.byte_count    = byte_count;
    form.bytes_written = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_DRIVER_WRITE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *bytes_written = form.bytes_written;
    }

    return status;
}

BqStatus BqDriverControl(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument)
{
    AbiScDriverControlForm form;
    form.driver_id    = driver_id;
    form.device_id    = device_id;
    form.control_code = control_code;
    form.argument     = argument;
    return BqSyscall(ABI_SC_FS_DRIVER_CONTROL, &form);
}

BqStatus BqMountVolume(UInt32 driver_id, FsVolumeType type, const char* mount_path, UInt32* volume_id)
{
    if (mount_path == NULL || volume_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScMountVolumeForm form;
    form.driver_id  = driver_id;
    form.type       = type;
    form.mount_path = mount_path;
    form.volume_id  = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_MOUNT_VOLUME, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *volume_id = form.volume_id;
    }

    return status;
}

BqStatus BqUnmountVolume(UInt32 volume_id)
{
    AbiScUnmountVolumeForm form;
    form.volume_id = volume_id;
    return BqSyscall(ABI_SC_FS_UNMOUNT_VOLUME, &form);
}

BqStatus BqGetVolume(UInt32 volume_id, FsVolume* volume)
{
    if (volume == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetVolumeForm form;
    form.volume_id = volume_id;
    BqStatus status = BqSyscall(ABI_SC_FS_GET_VOLUME, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *volume = form.volume;
    }

    return status;
}

BqStatus BqFindVolumeByPath(const char* path, FsVolume* volume)
{
    if (path == NULL || volume == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScFindVolumeByPathForm form;
    form.path = path;
    BqStatus status = BqSyscall(ABI_SC_FS_FIND_VOLUME_BY_PATH, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *volume = form.volume;
    }

    return status;
}

BqStatus BqCreateNode(UInt32 parent_id, const char* name, FsNodeType type, UInt32 flags, UInt32* node_id)
{
    if (name == NULL || node_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScCreateNodeForm form;
    form.parent_id = parent_id;
    form.name      = name;
    form.type      = type;
    form.flags     = flags;
    form.node_id   = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_CREATE_NODE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *node_id = form.node_id;
    }

    return status;
}

BqStatus BqDestroyNode(UInt32 node_id)
{
    AbiScDestroyNodeForm form;
    form.node_id = node_id;
    return BqSyscall(ABI_SC_FS_DESTROY_NODE, &form);
}

BqStatus BqGetNode(UInt32 node_id, FsNode* node)
{
    if (node == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetNodeForm form;
    form.node_id = node_id;
    BqStatus status = BqSyscall(ABI_SC_FS_GET_NODE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *node = form.node;
    }

    return status;
}

BqStatus BqResolvePath(const char* path, UInt32* node_id)
{
    if (path == NULL || node_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScResolvePathForm form;
    form.path    = path;
    form.node_id = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_RESOLVE_PATH, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *node_id = form.node_id;
    }

    return status;
}

BqStatus BqGetNodeByPath(const char* path, FsNode* node)
{
    if (path == NULL || node == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetNodeByPathForm form;
    form.path = path;
    BqStatus status = BqSyscall(ABI_SC_FS_GET_NODE_BY_PATH, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *node = form.node;
    }

    return status;
}

BqStatus BqBindNodeBuffer(UInt32 node_id, void* buffer, UInt64 size)
{
    AbiScBindNodeBufferForm form;
    form.node_id = node_id;
    form.buffer  = buffer;
    form.size    = size;
    return BqSyscall(ABI_SC_FS_BIND_NODE_BUFFER, &form);
}

BqStatus BqReadNodeBuffer(UInt32 node_id, void* buffer, UInt64 buffer_capacity, UInt64* bytes_read)
{
    if (buffer == NULL || bytes_read == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadNodeBufferForm form;
    form.node_id         = node_id;
    form.buffer          = buffer;
    form.buffer_capacity = buffer_capacity;
    form.bytes_read      = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_READ_NODE_BUFFER, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *bytes_read = form.bytes_read;
    }

    return status;
}

BqStatus BqWriteNodeBuffer(UInt32 node_id, const void* buffer, UInt64 size)
{
    if (buffer == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScWriteNodeBufferForm form;
    form.node_id = node_id;
    form.buffer  = (void*)buffer;
    form.size    = size;
    return BqSyscall(ABI_SC_FS_WRITE_NODE_BUFFER, &form);
}

BqStatus BqOpenFile(const char* path, FsFileMode mode, UInt32* file_id)
{
    if (path == NULL || file_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScOpenFileForm form;
    form.path    = path;
    form.mode    = mode;
    form.file_id = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_OPEN_FILE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *file_id = form.file_id;
    }

    return status;
}

BqStatus BqOpenFileByNode(UInt32 node_id, FsFileMode mode, UInt32* file_id)
{
    if (file_id == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScOpenFileByNodeForm form;
    form.node_id = node_id;
    form.mode    = mode;
    form.file_id = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_OPEN_FILE_BY_NODE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *file_id = form.file_id;
    }

    return status;
}

BqStatus BqCloseFile(UInt32 file_id)
{
    AbiScCloseFileForm form;
    form.file_id = file_id;
    return BqSyscall(ABI_SC_FS_CLOSE_FILE, &form);
}

BqStatus BqReadFile(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_read)
{
    if (buffer == NULL || bytes_read == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScReadFileForm form;
    form.file_id    = file_id;
    form.buffer     = buffer;
    form.byte_count = byte_count;
    form.bytes_read = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_READ_FILE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *bytes_read = form.bytes_read;
    }

    return status;
}

BqStatus BqWriteFile(UInt32 file_id, const void* buffer, UInt64 byte_count, UInt64* bytes_written)
{
    if (buffer == NULL || bytes_written == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScWriteFileForm form;
    form.file_id       = file_id;
    form.buffer        = (void*)buffer;
    form.byte_count    = byte_count;
    form.bytes_written = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_WRITE_FILE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *bytes_written = form.bytes_written;
    }

    return status;
}

BqStatus BqSeekFile(UInt32 file_id, Int64 offset, FsFileSeek origin, UInt64* new_offset)
{
    AbiScSeekFileForm form;
    form.file_id    = file_id;
    form.offset     = offset;
    form.origin     = origin;
    form.new_offset = 0;
    BqStatus status = BqSyscall(ABI_SC_FS_SEEK_FILE, &form);
    if (status == BQ_STATUS_SUCCESS && new_offset != NULL)
    {
        *new_offset = form.new_offset;
    }

    return status;
}

BqStatus BqGetFile(UInt32 file_id, FsFile* file)
{
    if (file == NULL)
    {
        return BQ_STATUS_INVALID_ARGUMENT;
    }

    AbiScGetFileForm form;
    form.file_id = file_id;
    BqStatus status = BqSyscall(ABI_SC_FS_GET_FILE, &form);
    if (status == BQ_STATUS_SUCCESS)
    {
        *file = form.file;
    }

    return status;
}
