// Made by Berkay

#ifndef ABI_SC_FS_H
#define ABI_SC_FS_H

#include "abi_sc_status.h"
#include "../fs/fs_public.h"

struct AbiScRegisterDriverForm
{
    UInt32   driver_id;
    FsDriver driver;
};
typedef struct AbiScRegisterDriverForm AbiScRegisterDriverForm;
AbiScStatus AbiScRegisterDriver(AbiScRegisterDriverForm* form);

struct AbiScUnregisterDriverForm
{
    UInt32 driver_id;
};
typedef struct AbiScUnregisterDriverForm AbiScUnregisterDriverForm;
AbiScStatus AbiScUnregisterDriver(AbiScUnregisterDriverForm* form);

struct AbiScGetDriverForm
{
    FsDriver driver;
    UInt32   driver_id;
};
typedef struct AbiScGetDriverForm AbiScGetDriverForm;
AbiScStatus AbiScGetDriver(AbiScGetDriverForm* form);

struct AbiScFindDriverByTypeForm
{
    FsDriver     driver;
    FsDriverType type;
};
typedef struct AbiScFindDriverByTypeForm AbiScFindDriverByTypeForm;
AbiScStatus AbiScFindDriverByType(AbiScFindDriverByTypeForm* form);

struct AbiScDriverReadForm
{
    UInt32 driver_id;
    UInt32 device_id;
    UInt64 offset;
    void*  buffer;
    UInt64 byte_count;
    UInt64 bytes_read;
};
typedef struct AbiScDriverReadForm AbiScDriverReadForm;
AbiScStatus AbiScDriverRead(AbiScDriverReadForm* form);

struct AbiScDriverWriteForm
{
    UInt32 driver_id;
    UInt32 device_id;
    UInt64 offset;
    void*  buffer;
    UInt64 byte_count;
    UInt64 bytes_written;
};
typedef struct AbiScDriverWriteForm AbiScDriverWriteForm;
AbiScStatus AbiScDriverWrite(AbiScDriverWriteForm* form);

struct AbiScDriverControlForm
{
    UInt32 driver_id;
    UInt32 device_id;
    UInt32 control_code;
    void*  argument;
};
typedef struct AbiScDriverControlForm AbiScDriverControlForm;
AbiScStatus AbiScDriverControl(AbiScDriverControlForm* form);

struct AbiScMountVolumeForm
{
    UInt32       volume_id;
    UInt32       driver_id;
    FsVolumeType type;
    const char*  mount_path;
};
typedef struct AbiScMountVolumeForm AbiScMountVolumeForm;
AbiScStatus AbiScMountVolume(AbiScMountVolumeForm* form);

struct AbiScUnmountVolumeForm
{
    UInt32 volume_id;
};
typedef struct AbiScUnmountVolumeForm AbiScUnmountVolumeForm;
AbiScStatus AbiScUnmountVolume(AbiScUnmountVolumeForm* form);

struct AbiScGetVolumeForm
{
    FsVolume volume;
    UInt32   volume_id;
};
typedef struct AbiScGetVolumeForm AbiScGetVolumeForm;
AbiScStatus AbiScGetVolume(AbiScGetVolumeForm* form);

struct AbiScFindVolumeByPathForm
{
    FsVolume    volume;
    const char* path;
};
typedef struct AbiScFindVolumeByPathForm AbiScFindVolumeByPathForm;
AbiScStatus AbiScFindVolumeByPath(AbiScFindVolumeByPathForm* form);

struct AbiScCreateNodeForm
{
    UInt32      node_id;
    UInt32      parent_id;
    const char* name;
    FsNodeType  type;
    UInt32      flags;
};
typedef struct AbiScCreateNodeForm AbiScCreateNodeForm;
AbiScStatus AbiScCreateNode(AbiScCreateNodeForm* form);

struct AbiScDestroyNodeForm
{
    UInt32 node_id;
};
typedef struct AbiScDestroyNodeForm AbiScDestroyNodeForm;
AbiScStatus AbiScDestroyNode(AbiScDestroyNodeForm* form);

struct AbiScGetNodeForm
{
    FsNode node;
    UInt32 node_id;
};
typedef struct AbiScGetNodeForm AbiScGetNodeForm;
AbiScStatus AbiScGetNode(AbiScGetNodeForm* form);

struct AbiScResolvePathForm
{
    UInt32      node_id;
    const char* path;
};
typedef struct AbiScResolvePathForm AbiScResolvePathForm;
AbiScStatus AbiScResolvePath(AbiScResolvePathForm* form);

struct AbiScGetNodeByPathForm
{
    FsNode      node;
    const char* path;
};
typedef struct AbiScGetNodeByPathForm AbiScGetNodeByPathForm;
AbiScStatus AbiScGetNodeByPath(AbiScGetNodeByPathForm* form);

struct AbiScBindNodeBufferForm
{
    UInt32 node_id;
    void*  buffer;
    UInt64 size;
};
typedef struct AbiScBindNodeBufferForm AbiScBindNodeBufferForm;
AbiScStatus AbiScBindNodeBuffer(AbiScBindNodeBufferForm* form);

struct AbiScReadNodeBufferForm
{
    UInt32 node_id;
    void*  buffer;
    UInt64 buffer_capacity;
    UInt64 bytes_read;
};
typedef struct AbiScReadNodeBufferForm AbiScReadNodeBufferForm;
AbiScStatus AbiScReadNodeBuffer(AbiScReadNodeBufferForm* form);

struct AbiScWriteNodeBufferForm
{
    UInt32 node_id;
    void*  buffer;
    UInt64 size;
};
typedef struct AbiScWriteNodeBufferForm AbiScWriteNodeBufferForm;
AbiScStatus AbiScWriteNodeBuffer(AbiScWriteNodeBufferForm* form);

struct AbiScOpenFileForm
{
    UInt32      file_id;
    const char* path;
    FsFileMode  mode;
};
typedef struct AbiScOpenFileForm AbiScOpenFileForm;
AbiScStatus AbiScOpenFile(AbiScOpenFileForm* form);

struct AbiScOpenFileByNodeForm
{
    UInt32     file_id;
    UInt32     node_id;
    FsFileMode mode;
};
typedef struct AbiScOpenFileByNodeForm AbiScOpenFileByNodeForm;
AbiScStatus AbiScOpenFileByNode(AbiScOpenFileByNodeForm* form);

struct AbiScCloseFileForm
{
    UInt32 file_id;
};
typedef struct AbiScCloseFileForm AbiScCloseFileForm;
AbiScStatus AbiScCloseFile(AbiScCloseFileForm* form);

struct AbiScReadFileForm
{
    UInt32 file_id;
    void*  buffer;
    UInt64 byte_count;
    UInt64 bytes_read;
};
typedef struct AbiScReadFileForm AbiScReadFileForm;
AbiScStatus AbiScReadFile(AbiScReadFileForm* form);

struct AbiScWriteFileForm
{
    UInt32 file_id;
    void*  buffer;
    UInt64 byte_count;
    UInt64 bytes_written;
};
typedef struct AbiScWriteFileForm AbiScWriteFileForm;
AbiScStatus AbiScWriteFile(AbiScWriteFileForm* form);

struct AbiScSeekFileForm
{
    UInt32     file_id;
    Int64      offset;
    FsFileSeek origin;
    UInt64     new_offset;
};
typedef struct AbiScSeekFileForm AbiScSeekFileForm;
AbiScStatus AbiScSeekFile(AbiScSeekFileForm* form);

struct AbiScGetFileForm
{
    FsFile file;
    UInt32 file_id;
};
typedef struct AbiScGetFileForm AbiScGetFileForm;
AbiScStatus AbiScGetFile(AbiScGetFileForm* form);

#endif
