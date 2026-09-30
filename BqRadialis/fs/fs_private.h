// Made by Berkay

#ifndef FS_PRIVATE_H
#define FS_PRIVATE_H

#include "fs_public.h"

#include "fs_drv.h"
#include "fs_vol.h"
#include "fs_nod.h"
#include "fs_fil.h"

#include "../bk/bk_public.h"
#include "../cp/cp_public.h"
#include "../mm/mm_public.h"
#include "../hw/hw_public.h"
#include "../ps/ps_public.h"

typedef enum   FsDrvLimit FsDrvLimit;
typedef enum   FsDrvType FsDrvType;
typedef enum   FsDrvState FsDrvState;
typedef enum   FsStatus (*FsDrvReadOperation)(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read);
typedef enum   FsStatus (*FsDrvWriteOperation)(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written);
typedef enum   FsStatus (*FsDrvControlOperation)(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument);
typedef struct FsDrvDriver FsDrvDriver;
typedef struct FsDrvLedger FsDrvLedger;

typedef enum   FsVolLimit FsVolLimit;
typedef enum   FsVolType FsVolType;
typedef enum   FsVolState FsVolState;
typedef struct FsVolVolume FsVolVolume;
typedef struct FsVolLedger FsVolLedger;

typedef enum   FsNodLimit FsNodLimit;
typedef enum   FsNodType FsNodType;
typedef enum   FsNodFlags FsNodFlags;
typedef struct FsNodNode FsNodNode;
typedef struct FsNodLedger FsNodLedger;

typedef enum   FsFilLimit FsFilLimit;
typedef enum   FsFilMode FsFilMode;
typedef enum   FsFilSeek FsFilSeek;
typedef enum   FsFilState FsFilState;
typedef struct FsFilFile FsFilFile;
typedef struct FsFilLedger FsFilLedger;

FsStatus FsDrvInit(void);
FsStatus FsDrvRegister(UInt32* driver_id, FsDriver* driver);
FsStatus FsDrvUnregister(UInt32 driver_id);
FsStatus FsDrvGet(FsDrvDriver** driver, UInt32 driver_id);
FsStatus FsDrvGetPublic(FsDriver* driver, UInt32 driver_id);
FsStatus FsDrvFindByType(FsDrvDriver** driver, FsDrvType type);
FsStatus FsDrvFindByTypePublic(FsDriver* driver, FsDrvType type);
FsStatus FsDrvInvokeRead(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read);
FsStatus FsDrvInvokeWrite(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written);
FsStatus FsDrvInvokeControl(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument);
FsStatus FsDrvRamfsRead(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read);
FsStatus FsDrvRamfsWrite(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written);
FsStatus FsDrvRamfsControl(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument);

FsStatus FsVolInit(void);
FsStatus FsVolMount(UInt32* volume_id, UInt32 driver_id, FsVolType type, const char* mount_path);
FsStatus FsVolUnmount(UInt32 volume_id);
FsStatus FsVolGet(FsVolVolume** volume, UInt32 volume_id);
FsStatus FsVolGetPublic(FsVolume* volume, UInt32 volume_id);
FsStatus FsVolFindByPath(FsVolVolume** volume, const char* path);
FsStatus FsVolFindByPathPublic(FsVolume* volume, const char* path);
FsStatus FsVolSetRootNode(UInt32 volume_id, UInt32 root_node_id);
FsStatus FsVolUpdateUsage(UInt32 volume_id, Int64 delta_bytes);

FsStatus FsNodInit(void);
FsStatus FsNodCreate(UInt32* node_id, UInt32 parent_id, const char* name, FsNodType type, UInt32 flags);
FsStatus FsNodDestroy(UInt32 node_id);
FsStatus FsNodGet(FsNodNode** node, UInt32 node_id);
FsStatus FsNodGetPublic(FsNode* node, UInt32 node_id);
FsStatus FsNodResolvePath(UInt32* node_id, const char* path);
FsStatus FsNodGetByPathPublic(FsNode* node, const char* path);
FsStatus FsNodBindBuffer(UInt32 node_id, void* buffer, UInt64 size);
FsStatus FsNodReadBuffer(UInt32 node_id, void* buffer, UInt64 buffer_capacity, UInt64* bytes_read);
FsStatus FsNodWriteBuffer(UInt32 node_id, void* buffer, UInt64 size);
FsStatus FsNodFindChild(UInt32* child_id, UInt32 parent_id, const char* name);
FsStatus FsNodAddChild(UInt32 parent_id, UInt32 child_id);
FsStatus FsNodRemoveChild(UInt32 parent_id, UInt32 child_id);
FsStatus FsNodAllocateBuffer(UInt32 node_id, UInt64 capacity);
FsStatus FsNodFreeBuffer(UInt32 node_id);

FsStatus FsFilInit(void);
FsStatus FsFilOpen(UInt32* file_id, const char* path, FsFilMode mode);
FsStatus FsFilOpenByNode(UInt32* file_id, UInt32 node_id, FsFilMode mode);
FsStatus FsFilClose(UInt32 file_id);
FsStatus FsFilRead(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_read);
FsStatus FsFilWrite(UInt32 file_id, void* buffer, UInt64 byte_count, UInt64* bytes_written);
FsStatus FsFilSeekFile(UInt32 file_id, Int64 offset, FsFilSeek origin, UInt64* new_offset);
FsStatus FsFilGet(FsFilFile** file, UInt32 file_id);
FsStatus FsFilGetPublic(FsFile* file, UInt32 file_id);

FsStatus FsKitCopyMemory(void* destination, const void* source, UInt64 byte_count);
FsStatus FsKitZeroMemory(void* destination, UInt64 byte_count);
FsStatus FsKitCompareMemory(UInt8* is_equal, const void* first, const void* second, UInt64 byte_count);
FsStatus FsKitStringLength(UInt64* length, const char* string);
FsStatus FsKitStringCopy(char* destination, const char* source, UInt64 capacity);
FsStatus FsKitStringCompare(Int32* difference, const char* first, const char* second);
FsStatus FsKitExtractNextToken(const char* path, UInt64* current_index, char* token_buffer, UInt64 token_capacity);
FsStatus FsKitDriverToPublic(FsDriver* public_driver, FsDrvDriver* private_driver);
FsStatus FsKitVolumeToPublic(FsVolume* public_volume, FsVolVolume* private_volume);
FsStatus FsKitNodeToPublic(FsNode* public_node, FsNodNode* private_node);
FsStatus FsKitFileToPublic(FsFile* public_file, FsFilFile* private_file);

#endif
