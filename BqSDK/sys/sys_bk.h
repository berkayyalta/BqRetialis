// Made by Berkay

#ifndef SYS_BK_H
#define SYS_BK_H

#include "sys_core.h"

BqStatus BqGetKernelLayout(BkKernelLayout* layout);
BqStatus BqGetMemoryMap(BkMemoryMap* memory_map);
BqStatus BqGetAcpiRoot(BkAcpiRoot* acpi_root);
BqStatus BqGetAcpiMadt(BkAcpiMadt** madt);
BqStatus BqGetAcpiFadt(BkAcpiFadt** fadt);
BqStatus BqGetAcpiHpet(BkAcpiHpet** hpet);
BqStatus BqGetAcpiMcfg(BkAcpiMcfg** mcfg);
BqStatus BqGetFramebuffer(BkFramebuffer* framebuffer);
BqStatus BqGetBootInfo(BkBootInfo* boot_info);

#endif
