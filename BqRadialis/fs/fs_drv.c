// Made by Berkay

#include "fs_private.h"

static FsDrvLedger s_drv_ledger;

FsStatus FsDrvInit(void)
{
    UInt8* raw = (UInt8*)&s_drv_ledger;
    for (UInt64 i = 0; i < sizeof(FsDrvLedger); i++)
    {
        raw[i] = 0;
    }

    PsStatus ps_status = PsSpinlockCreate(&s_drv_ledger.lock_id);
    if (ps_status != PS_STATUS_SUCCESS)
    {
        return FS_STATUS_INTERNAL_ERROR;
    }

    s_drv_ledger.drivers[1].id           = 1;
    s_drv_ledger.drivers[1].type         = FS_DRV_TYPE_RAMFS;
    s_drv_ledger.drivers[1].state        = FS_DRV_STATE_ACTIVE;
    s_drv_ledger.drivers[1].read         = FsDrvRamfsRead;
    s_drv_ledger.drivers[1].write        = FsDrvRamfsWrite;
    s_drv_ledger.drivers[1].control      = FsDrvRamfsControl;
    s_drv_ledger.drivers[1].is_allocated = 1;

    FsKitStringCopy((char*)s_drv_ledger.drivers[1].name, "ramfs", FS_DRV_NAME_CAPACITY);

    s_drv_ledger.driver_count   = 1;
    s_drv_ledger.is_initialized = 1;

    return FS_STATUS_SUCCESS;
}

FsStatus FsDrvRegister(UInt32* driver_id, FsDriver* driver)
{
    if (driver_id == NULL || driver == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_drv_ledger.lock_id);

    UInt32 slot = 0;
    for (UInt32 i = 2; i < FS_DRV_MAX_DRIVERS; i++)
    {
        if (s_drv_ledger.drivers[i].is_allocated == 0)
        {
            slot = i;
            break;
        }
    }

    if (slot == 0)
    {
        PsSpinlockRelease(s_drv_ledger.lock_id);
        return FS_STATUS_OUT_OF_RESOURCES;
    }

    s_drv_ledger.drivers[slot].id           = slot;
    s_drv_ledger.drivers[slot].type         = (FsDrvType)driver->type;
    s_drv_ledger.drivers[slot].state        = FS_DRV_STATE_ACTIVE;
    s_drv_ledger.drivers[slot].read         = (FsDrvReadOperation)driver->read;
    s_drv_ledger.drivers[slot].write        = (FsDrvWriteOperation)driver->write;
    s_drv_ledger.drivers[slot].control      = (FsDrvControlOperation)driver->control;
    s_drv_ledger.drivers[slot].is_allocated = 1;

    FsKitStringCopy((char*)s_drv_ledger.drivers[slot].name, (const char*)driver->name, FS_DRV_NAME_CAPACITY);

    s_drv_ledger.driver_count++;

    *driver_id = slot;

    PsSpinlockRelease(s_drv_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsDrvUnregister(UInt32 driver_id)
{
    if (driver_id == 0 || driver_id >= FS_DRV_MAX_DRIVERS)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_drv_ledger.lock_id);

    if (s_drv_ledger.drivers[driver_id].is_allocated == 0)
    {
        PsSpinlockRelease(s_drv_ledger.lock_id);
        return FS_STATUS_NOT_FOUND;
    }

    s_drv_ledger.drivers[driver_id].is_allocated = 0;
    s_drv_ledger.drivers[driver_id].state        = FS_DRV_STATE_FREE;
    s_drv_ledger.driver_count--;

    PsSpinlockRelease(s_drv_ledger.lock_id);

    return FS_STATUS_SUCCESS;
}

FsStatus FsDrvGet(FsDrvDriver** driver, UInt32 driver_id)
{
    if (driver == NULL || driver_id == 0 || driver_id >= FS_DRV_MAX_DRIVERS)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    if (s_drv_ledger.drivers[driver_id].is_allocated == 0)
    {
        return FS_STATUS_NOT_FOUND;
    }

    *driver = &s_drv_ledger.drivers[driver_id];

    return FS_STATUS_SUCCESS;
}

FsStatus FsDrvGetPublic(FsDriver* driver, UInt32 driver_id)
{
    if (driver == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    FsDrvDriver* internal_driver = NULL;
    FsStatus status = FsDrvGet(&internal_driver, driver_id);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    return FsKitDriverToPublic(driver, internal_driver);
}

FsStatus FsDrvFindByType(FsDrvDriver** driver, FsDrvType type)
{
    if (driver == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    PsSpinlockAcquire(s_drv_ledger.lock_id);

    for (UInt32 i = 1; i < FS_DRV_MAX_DRIVERS; i++)
    {
        if (s_drv_ledger.drivers[i].is_allocated != 0 && s_drv_ledger.drivers[i].type == type)
        {
            *driver = &s_drv_ledger.drivers[i];
            PsSpinlockRelease(s_drv_ledger.lock_id);
            return FS_STATUS_SUCCESS;
        }
    }

    PsSpinlockRelease(s_drv_ledger.lock_id);

    return FS_STATUS_NOT_FOUND;
}

FsStatus FsDrvFindByTypePublic(FsDriver* driver, FsDrvType type)
{
    if (driver == NULL)
    {
        return FS_STATUS_INVALID_PARAMETER;
    }

    FsDrvDriver* internal_driver = NULL;
    FsStatus status = FsDrvFindByType(&internal_driver, type);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    return FsKitDriverToPublic(driver, internal_driver);
}

FsStatus FsDrvInvokeRead(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read)
{
    FsDrvDriver* driver = NULL;
    FsStatus status = FsDrvGet(&driver, driver_id);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    if (driver->read == NULL)
    {
        return FS_STATUS_NOT_SUPPORTED;
    }

    return driver->read(driver_id, device_id, offset, buffer, byte_count, bytes_read);
}

FsStatus FsDrvInvokeWrite(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written)
{
    FsDrvDriver* driver = NULL;
    FsStatus status = FsDrvGet(&driver, driver_id);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    if (driver->write == NULL)
    {
        return FS_STATUS_NOT_SUPPORTED;
    }

    return driver->write(driver_id, device_id, offset, buffer, byte_count, bytes_written);
}

FsStatus FsDrvInvokeControl(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument)
{
    FsDrvDriver* driver = NULL;
    FsStatus status = FsDrvGet(&driver, driver_id);
    if (status != FS_STATUS_SUCCESS)
    {
        return status;
    }

    if (driver->control == NULL)
    {
        return FS_STATUS_NOT_SUPPORTED;
    }

    return driver->control(driver_id, device_id, control_code, argument);
}

FsStatus FsDrvRamfsRead(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_read)
{
    (void)driver_id;
    (void)device_id;
    (void)offset;
    (void)buffer;
    (void)byte_count;

    if (bytes_read != NULL)
    {
        *bytes_read = 0;
    }

    return FS_STATUS_SUCCESS;
}

FsStatus FsDrvRamfsWrite(UInt32 driver_id, UInt32 device_id, UInt64 offset, void* buffer, UInt64 byte_count, UInt64* bytes_written)
{
    (void)driver_id;
    (void)device_id;
    (void)offset;
    (void)buffer;
    (void)byte_count;

    if (bytes_written != NULL)
    {
        *bytes_written = 0;
    }

    return FS_STATUS_SUCCESS;
}

FsStatus FsDrvRamfsControl(UInt32 driver_id, UInt32 device_id, UInt32 control_code, void* argument)
{
    (void)driver_id;
    (void)device_id;
    (void)control_code;
    (void)argument;

    return FS_STATUS_SUCCESS;
}
