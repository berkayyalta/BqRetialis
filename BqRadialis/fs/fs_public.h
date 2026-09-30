// Made by Berkay

#ifndef FS_PUBLIC_H
#define FS_PUBLIC_H

#include "fs_status.h"
#include "fs_driver.h"
#include "fs_volume.h"
#include "fs_node.h"
#include "fs_file.h"

typedef enum   FsStatus FsStatus;

typedef enum   FsDriverLimit FsDriverLimit;
typedef enum   FsDriverType FsDriverType;
typedef enum   FsDriverState FsDriverState;
typedef enum   FsStatus (*FsDriverReadOperation)(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read);
typedef enum   FsStatus (*FsDriverWriteOperation)(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written);
typedef enum   FsStatus (*FsDriverControlOperation)(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument);
typedef struct FsDriver FsDriver;

typedef enum   FsVolumeLimit FsVolumeLimit;
typedef enum   FsVolumeType FsVolumeType;
typedef enum   FsVolumeState FsVolumeState;
typedef struct FsVolume FsVolume;

typedef enum   FsNodeLimit FsNodeLimit;
typedef enum   FsNodeType FsNodeType;
typedef enum   FsNodeFlags FsNodeFlags;
typedef struct FsNode FsNode;

typedef enum   FsFileMode FsFileMode;
typedef enum   FsFileSeek FsFileSeek;
typedef enum   FsFileState FsFileState;
typedef struct FsFile FsFile;

void     FsLoad(void);
FsStatus FsInit(void);

FsStatus FsRegisterDriver(UInt32* driver_id, FsDriver* driver);
FsStatus FsUnregisterDriver(UInt32 driver_id);
FsStatus FsGetDriver(FsDriver* driver, UInt32 driver_id);
FsStatus FsFindDriverByType(FsDriver* driver, FsDriverType type);
FsStatus FsDriverRead(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read);
FsStatus FsDriverWrite(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written);
FsStatus FsDriverControl(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument);

FsStatus FsMountVolume(UInt32* volume_id, UInt32 driver_id, FsVolumeType type, const char* mount_path);
FsStatus FsUnmountVolume(UInt32 volume_id);
FsStatus FsGetVolume(FsVolume* volume, UInt32 volume_id);
FsStatus FsFindVolumeByPath(FsVolume* volume, const char* path);

FsStatus FsCreateNode(UInt32* node_id, UInt32 parent_id, const char* name, FsNodeType type, UInt32 flags);
FsStatus FsDestroyNode(UInt32 node_id);
FsStatus FsGetNode(FsNode* node, UInt32 node_id);
FsStatus FsResolvePath(UInt32* node_id, const char* path);
FsStatus FsGetNodeByPath(FsNode* node, const char* path);
FsStatus FsBindNodeBuffer(UInt32 node_id, void* buffer, UInt64 size);
FsStatus FsReadNodeBuffer(UInt32 node_id, void* buffer, UInt64 buffer_capacity, UInt64* bytes_read);
FsStatus FsWriteNodeBuffer(UInt32 node_id, void* buffer, UInt64 size);

FsStatus FsOpenFile(UInt32* file_id, const char* path, FsFileMode mode);
FsStatus FsOpenFileByNode(UInt32* file_id, UInt32 node_id, FsFileMode mode);
FsStatus FsCloseFile(UInt32 file_id);
FsStatus FsReadFile(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_read);
FsStatus FsWriteFile(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_written);
FsStatus FsSeekFile(UInt32 file_id, Int64 offset, FsFileSeek origin, UInt64* new_offset);
FsStatus FsGetFile(FsFile* file, UInt32 file_id);

#endif
