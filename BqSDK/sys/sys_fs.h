// Made by Berkay

#ifndef SYS_FS_H
#define SYS_FS_H

#include "sys_core.h"

BqStatus BqRegisterDriver(const FsDriver* driver, UInt32* driver_id);
BqStatus BqUnregisterDriver(UInt32 driver_id);
BqStatus BqGetDriver(UInt32 driver_id, FsDriver* driver);
BqStatus BqFindDriverByType(FsDriverType type, FsDriver* driver);
BqStatus BqDriverRead(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read);
BqStatus BqDriverWrite(UInt32 driver_id, UInt32 device_id, UInt64 offset, const void* buffer, UInt64 byte_count, UInt64* bytes_written);
BqStatus BqDriverControl(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument);
BqStatus BqMountVolume(UInt32 driver_id, FsVolumeType type, const char* mount_path, UInt32* volume_id);
BqStatus BqUnmountVolume(UInt32 volume_id);
BqStatus BqGetVolume(UInt32 volume_id, FsVolume* volume);
BqStatus BqFindVolumeByPath(const char* path, FsVolume* volume);
BqStatus BqCreateNode(UInt32 parent_id, const char* name, FsNodeType type, UInt32 flags, UInt32* node_id);
BqStatus BqDestroyNode(UInt32 node_id);
BqStatus BqGetNode(UInt32 node_id, FsNode* node);
BqStatus BqResolvePath(const char* path, UInt32* node_id);
BqStatus BqGetNodeByPath(const char* path, FsNode* node);
BqStatus BqBindNodeBuffer(UInt32 node_id, void* buffer, UInt64 size);
BqStatus BqReadNodeBuffer(UInt32 node_id, void* buffer, UInt64 buffer_capacity, UInt64* bytes_read);
BqStatus BqWriteNodeBuffer(UInt32 node_id, const void* buffer, UInt64 size);
BqStatus BqOpenFile(const char* path, FsFileMode mode, UInt32* file_id);
BqStatus BqOpenFileByNode(UInt32 node_id, FsFileMode mode, UInt32* file_id);
BqStatus BqCloseFile(UInt32 file_id);
BqStatus BqReadFile(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_read);
BqStatus BqWriteFile(UInt32 file_id, const void* buffer, UInt64 byte_count, UInt64* bytes_written);
BqStatus BqSeekFile(UInt32 file_id, Int64 offset, FsFileSeek origin, UInt64* new_offset);
BqStatus BqGetFile(UInt32 file_id, FsFile* file);

#endif
