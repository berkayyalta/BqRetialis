// Made by Berkay

#ifndef ABI_SC_BK_H
#define ABI_SC_BK_H

#include "abi_sc_status.h"
#include "../bk/bk_public.h"

struct AbiScGetKernelLayoutForm
{
    BkKernelLayout layout;
};
typedef struct AbiScGetKernelLayoutForm AbiScGetKernelLayoutForm;
AbiScStatus AbiScGetKernelLayout(AbiScGetKernelLayoutForm* form);

struct AbiScGetMemoryMapForm
{
    BkMemoryMap memory_map;
};
typedef struct AbiScGetMemoryMapForm AbiScGetMemoryMapForm;
AbiScStatus AbiScGetMemoryMap(AbiScGetMemoryMapForm* form);

struct AbiScGetAcpiRootForm
{
    BkAcpiRoot acpi_root;
};
typedef struct AbiScGetAcpiRootForm AbiScGetAcpiRootForm;
AbiScStatus AbiScGetAcpiRoot(AbiScGetAcpiRootForm* form);

struct AbiScGetAcpiMadtForm
{
    BkAcpiMadt* madt;
};
typedef struct AbiScGetAcpiMadtForm AbiScGetAcpiMadtForm;
AbiScStatus AbiScGetAcpiMadt(AbiScGetAcpiMadtForm* form);

struct AbiScGetAcpiFadtForm
{
    BkAcpiFadt* fadt;
};
typedef struct AbiScGetAcpiFadtForm AbiScGetAcpiFadtForm;
AbiScStatus AbiScGetAcpiFadt(AbiScGetAcpiFadtForm* form);

struct AbiScGetAcpiHpetForm
{
    BkAcpiHpet* hpet;
};
typedef struct AbiScGetAcpiHpetForm AbiScGetAcpiHpetForm;
AbiScStatus AbiScGetAcpiHpet(AbiScGetAcpiHpetForm* form);

struct AbiScGetAcpiMcfgForm
{
    BkAcpiMcfg* mcfg;
};
typedef struct AbiScGetAcpiMcfgForm AbiScGetAcpiMcfgForm;
AbiScStatus AbiScGetAcpiMcfg(AbiScGetAcpiMcfgForm* form);

struct AbiScGetFramebufferForm
{
    BkFramebuffer framebuffer;
};
typedef struct AbiScGetFramebufferForm AbiScGetFramebufferForm;
AbiScStatus AbiScGetFramebuffer(AbiScGetFramebufferForm* form);

struct AbiScGetBootInfoForm
{
    BkBootInfo boot_info;
};
typedef struct AbiScGetBootInfoForm AbiScGetBootInfoForm;
AbiScStatus AbiScGetBootInfo(AbiScGetBootInfoForm* form);

#endif
