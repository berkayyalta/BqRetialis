// Made by Berkay

#ifndef SYS_PS_H
#define SYS_PS_H

#include "sys_core.h"

BqStatus BqSpinlockCreate(UInt32* lock_id);
BqStatus BqSpinlockAcquire(UInt32 lock_id);
BqStatus BqSpinlockRelease(UInt32 lock_id);
BqStatus BqSpinlockDestroy(UInt32 lock_id);
BqStatus BqMutexCreate(UInt32* mutex_id);
BqStatus BqMutexAcquire(UInt32 mutex_id);
BqStatus BqMutexRelease(UInt32 mutex_id);
BqStatus BqMutexDestroy(UInt32 mutex_id);
BqStatus BqGetLock(UInt32 lock_id, PsLock* lock);
BqStatus BqProcessCreate(PsProcessPrivilege privilege, PsProcessPriority priority, UInt32* process_id);
BqStatus BqProcessDestroy(UInt32 process_id);
BqStatus BqProcessGet(UInt32 process_id, PsProcess* process);
BqStatus BqProcessGetCurrentId(UInt32* process_id);
BqStatus BqThreadCreate(UInt32 process_id, PsThreadEntry entry, void* argument, PsThreadPriority priority, UInt32* thread_id);
BqStatus BqThreadDestroy(UInt32 thread_id);
BqStatus BqThreadGet(UInt32 thread_id, PsThread* thread);
BqStatus BqThreadGetCurrentId(UInt32* thread_id);
BqStatus BqThreadGetContext(UInt32 thread_id, PsContext* context);
BqStatus BqThreadSetContext(UInt32 thread_id, const PsContext* context);
BqStatus BqYield(void);
BqStatus BqSleep(UInt64 ticks);
BqStatus BqBlock(UInt32 thread_id);
BqStatus BqUnblock(UInt32 thread_id);

#endif
