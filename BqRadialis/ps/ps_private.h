// Made by Berkay

#ifndef PS_PRIVATE_H
#define PS_PRIVATE_H

#include "ps_public.h"

#include "ps_lck.h"
#include "ps_prc.h"
#include "ps_thr.h"
#include "ps_sch.h"

#include "../bk/bk_public.h"
#include "../cp/cp_public.h"
#include "../mm/mm_public.h"
#include "../hw/hw_public.h"

typedef enum   PsLckLimit PsLckLimit;
typedef enum   PsLckType PsLckType;
typedef enum   PsLockState PsLockState;
typedef struct PsLckSpinlock PsLckSpinlock;
typedef struct PsLckMutex PsLckMutex;
typedef struct PsLckLedger PsLckLedger;

typedef enum   PsPrcLimit PsPrcLimit;
typedef enum   PsPrcState PsPrcState;
typedef enum   PsPrcPriority PsPrcPriority;
typedef enum   PsPrcPrivilege PsPrcPrivilege;
typedef struct PsPrcProcess PsPrcProcess;
typedef struct PsPrcLedger PsPrcLedger;

typedef enum   PsThrLimit PsThrLimit;
typedef enum   PsThrState PsThrState;
typedef enum   PsThrPriority PsThrPriority;
typedef struct PsThrContext PsThrContext;
typedef struct PsThrThread PsThrThread;
typedef struct PsThrLedger PsThrLedger;

typedef enum   PsSchLimit PsSchLimit;
typedef struct PsSchQueue PsSchQueue;
typedef struct PsSchCore PsSchCore;
typedef struct PsSchLedger PsSchLedger;

PsStatus PsLckInit(void);
PsStatus PsLckSpinlockCreate(UInt32* lock_id);
PsStatus PsLckSpinlockAcquire(UInt32 lock_id);
PsStatus PsLckSpinlockRelease(UInt32 lock_id);
PsStatus PsLckSpinlockDestroy(UInt32 lock_id);
PsStatus PsLckMutexCreate(UInt32* mutex_id);
PsStatus PsLckMutexAcquire(UInt32 mutex_id);
PsStatus PsLckMutexRelease(UInt32 mutex_id);
PsStatus PsLckMutexDestroy(UInt32 mutex_id);
PsStatus PsLckGet(PsLock* lock, UInt32 lock_id);

PsStatus PsPrcInit(void);
PsStatus PsPrcCreate(UInt32* process_id, PsPrcPrivilege privilege, PsPrcPriority priority);
PsStatus PsPrcDestroy(UInt32 process_id);
PsStatus PsPrcGet(PsPrcProcess** process, UInt32 process_id);
PsStatus PsPrcGetPublic(PsProcess* process, UInt32 process_id);
PsStatus PsPrcGetCurrentId(UInt32* process_id);
PsStatus PsPrcIncrementThreadCount(UInt32 process_id);
PsStatus PsPrcDecrementThreadCount(UInt32 process_id);

PsStatus PsThrInit(void);
PsStatus PsThrCreate(UInt32* thread_id, UInt32 process_id, PsThreadEntry entry, void* argument, PsThrPriority priority);
PsStatus PsThrCreateIdle(UInt32* thread_id, UInt32 core_index);
PsStatus PsThrDestroy(UInt32 thread_id);
PsStatus PsThrGet(PsThrThread** thread, UInt32 thread_id);
PsStatus PsThrGetPublic(PsThread* thread, UInt32 thread_id);
PsStatus PsThrGetCurrentId(UInt32* thread_id);
PsStatus PsThrGetContext(PsContext* context, UInt32 thread_id);
PsStatus PsThrSetContext(UInt32 thread_id, PsContext* context);
PsStatus PsThrBootstrap(void);

void     PsSchLoad(void);
PsStatus PsSchInit(void);
PsStatus PsSchStart(void);
PsStatus PsSchEnqueue(UInt32 core_index, UInt32 thread_id, PsThrPriority priority);
PsStatus PsSchDequeue(UInt32* thread_id, UInt32 core_index);
PsStatus PsSchPickNext(UInt32* next_thread_id, UInt32 core_index);
PsStatus PsSchSwitchContext(CpContext** next_cp_context, CpContext* current_cp_context);
PsStatus PsSchTick(void);
PsStatus PsSchYield(void);
PsStatus PsSchSleep(UInt64 ticks);
PsStatus PsSchBlock(UInt32 thread_id);
PsStatus PsSchUnblock(UInt32 thread_id);
PsStatus PsSchIdleEntry(void* argument);

PsStatus PsKitSpinlockAcquire(volatile UInt32* lock);
PsStatus PsKitSpinlockRelease(volatile UInt32* lock);
PsStatus PsKitSelectCore(UInt32* core_index);
PsStatus PsKitContextToCp(CpContext* cp_context, PsThrContext* thr_context);
PsStatus PsKitContextFromCp(PsThrContext* thr_context, CpContext* cp_context);
PsStatus PsKitContextToPublic(PsContext* public_context, PsThrContext* thr_context);
PsStatus PsKitContextFromPublic(PsThrContext* thr_context, PsContext* public_context);
PsStatus PsKitLockToPublic(PsLock* public_lock, PsLckSpinlock* spinlock, PsLckMutex* mutex);
PsStatus PsKitProcessToPublic(PsProcess* public_process, PsPrcProcess* prc_process);
PsStatus PsKitThreadToPublic(PsThread* public_thread, PsThrThread* thr_thread);

#endif
