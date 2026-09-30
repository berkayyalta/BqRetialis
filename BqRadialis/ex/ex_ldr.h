// Made by Berkay

#ifndef EX_LDR_H
#define EX_LDR_H

#include "../bk/bk_types.h"

enum ExLdrLimit : UInt32
{
    EX_LDR_MAX_PROCESSES    = 256,
    EX_LDR_USER_STACK_PAGES = 16
};

enum ExLdrElfType : UInt32
{
    EX_LDR_ELF_TYPE_NONE = 0,
    EX_LDR_ELF_TYPE_REL  = 1,
    EX_LDR_ELF_TYPE_EXEC = 2,
    EX_LDR_ELF_TYPE_DYN  = 3
};

enum ExLdrSegmentType : UInt32
{
    EX_LDR_SEGMENT_NULL = 0,
    EX_LDR_SEGMENT_LOAD = 1
};

enum ExLdrAddress : UInt64
{
    EX_LDR_USER_STACK_BASE = 0x00007FFFF0000000ULL,
    EX_LDR_HHDM_BASE       = 0xFFFF800000000000ULL
};

struct ExLdrElfHeader
{
    UInt8  ident[16];
    UInt16 type;
    UInt16 machine;
    UInt32 version;
    UInt64 entry;
    UInt64 phoff;
    UInt64 shoff;
    UInt32 flags;
    UInt16 ehsize;
    UInt16 phentsize;
    UInt16 phnum;
    UInt16 shentsize;
    UInt16 shnum;
    UInt16 shstrndx;
}
__attribute__((packed));

struct ExLdrElfProgramHeader
{
    UInt32 type;
    UInt32 flags;
    UInt64 offset;
    UInt64 vaddr;
    UInt64 paddr;
    UInt64 filesz;
    UInt64 memsz;
    UInt64 align;
}
__attribute__((packed));

struct ExLdrLedger
{
    UInt8  is_driver_process[EX_LDR_MAX_PROCESSES];
    UInt32 total_loaded;
    UInt8  is_initialized;
};

typedef struct ExLdrElfHeader ExLdrElfHeader;
typedef struct ExLdrElfProgramHeader ExLdrElfProgramHeader;
typedef struct ExLdrLedger ExLdrLedger;

#endif
