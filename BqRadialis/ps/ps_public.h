// Made by Berkay

#ifndef PS_PUBLIC_H
#define PS_PUBLIC_H

#include "ps_status.h"
#include "ps_context.h"
#include "ps_lock.h"
#include "ps_process.h"
#include "ps_thread.h"

typedef enum   PsStatus PsStatus;

typedef struct PsContext PsContext;

typedef enum   PsLockType PsLockType;
typedef enum   PsLockState PsLockState;
typedef struct PsLock PsLock;

typedef enum   PsProcessState PsProcessState;
typedef enum   PsProcessPriority PsProcessPriority;
typedef enum   PsProcessPrivilege PsProcessPrivilege;
typedef struct PsProcess PsProcess;

typedef enum   PsThreadState PsThreadState;
typedef enum   PsThreadPriority PsThreadPriority;
typedef void (*PsThreadEntry)(void* argument);
typedef struct PsThread PsThread;

void     PsLoad(void);
PsStatus PsInit(void);
PsStatus PsStartScheduling(void);

PsStatus PsSpinlockCreate(UInt32* lock_id);
PsStatus PsSpinlockAcquire(UInt32 lock_id);
PsStatus PsSpinlockRelease(UInt32 lock_id);
PsStatus PsSpinlockDestroy(UInt32 lock_id);
PsStatus PsMutexCreate(UInt32* mutex_id);
PsStatus PsMutexAcquire(UInt32 mutex_id);
PsStatus PsMutexRelease(UInt32 mutex_id);
PsStatus PsMutexDestroy(UInt32 mutex_id);
PsStatus PsGetLock(PsLock* lock, UInt32 lock_id);

PsStatus PsProcessCreate(UInt32* process_id, PsProcessPrivilege privilege, PsProcessPriority priority);
PsStatus PsProcessDestroy(UInt32 process_id);
PsStatus PsProcessGet(PsProcess* process, UInt32 process_id);
PsStatus PsProcessGetCurrentId(UInt32* process_id);

PsStatus PsThreadCreate(UInt32* thread_id, UInt32 process_id, PsThreadEntry entry, void* argument, PsThreadPriority priority);
PsStatus PsThreadDestroy(UInt32 thread_id);
PsStatus PsThreadGet(PsThread* thread, UInt32 thread_id);
PsStatus PsThreadGetCurrentId(UInt32* thread_id);
PsStatus PsThreadGetContext(PsContext* context, UInt32 thread_id);
PsStatus PsThreadSetContext(UInt32 thread_id, PsContext* context);

PsStatus PsTick(void);
PsStatus PsYield(void);
PsStatus PsSleep(UInt64 ticks);
PsStatus PsBlock(UInt32 thread_id);
PsStatus PsUnblock(UInt32 thread_id);

#endif
