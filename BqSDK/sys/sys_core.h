// Made by Berkay

#ifndef SYS_CORE_H
#define SYS_CORE_H

#if __has_include("abi_sc.h")
#include "abi_sc.h"
#elif __has_include("../../BqRadialis/abi/abi_sc.h")
#include "../../BqRadialis/abi/abi_sc.h"
#elif __has_include("../BqRadialis/abi/abi_sc.h")
#include "../BqRadialis/abi/abi_sc.h"
#endif

typedef enum BqStatus
{
    BQ_STATUS_SUCCESS          = ABI_SC_STATUS_SUCCESS,
    BQ_STATUS_INVALID_ARGUMENT = ABI_SC_STATUS_INVALID_ARGUMENT,
    BQ_STATUS_ACCESS_DENIED    = ABI_SC_STATUS_ACCESS_DENIED,
    BQ_STATUS_NOT_FOUND        = ABI_SC_STATUS_NOT_FOUND,
    BQ_STATUS_OUT_OF_MEMORY    = ABI_SC_STATUS_OUT_OF_MEMORY,
    BQ_STATUS_OUT_OF_BOUNDS    = ABI_SC_STATUS_OUT_OF_BOUNDS,
    BQ_STATUS_BUSY             = ABI_SC_STATUS_BUSY,
    BQ_STATUS_NOT_READY        = ABI_SC_STATUS_NOT_READY,
    BQ_STATUS_NOT_SUPPORTED    = ABI_SC_STATUS_NOT_SUPPORTED,
    BQ_STATUS_ALREADY_EXISTS   = ABI_SC_STATUS_ALREADY_EXISTS,
    BQ_STATUS_IO_ERROR         = ABI_SC_STATUS_IO_ERROR,
    BQ_STATUS_END_OF_FILE      = ABI_SC_STATUS_END_OF_FILE,
    BQ_STATUS_INVALID_SYSCALL  = ABI_SC_STATUS_INVALID_SYSCALL,
    BQ_STATUS_HARDWARE_FAILURE = ABI_SC_STATUS_HARDWARE_FAILURE,
    BQ_STATUS_INTERNAL_ERROR   = ABI_SC_STATUS_INTERNAL_ERROR
} BqStatus;

BqStatus BqSyscall(AbiScId syscall_id, void* form);

void* memcpy(void* dest, const void* src, Size n);
void* memset(void* dest, int c, Size n);
void* memmove(void* dest, const void* src, Size n);
int memcmp(const void* s1, const void* s2, Size n);

#endif
