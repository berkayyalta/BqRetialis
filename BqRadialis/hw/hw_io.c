// Made by Berkay

#include "hw_private.h"

UInt8 HwIoIn8(UInt16 port)
{
    UInt8 ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void HwIoOut8(UInt16 port, UInt8 data)
{
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port) : "memory");
}

UInt16 HwIoIn16(UInt16 port)
{
    UInt16 ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void HwIoOut16(UInt16 port, UInt16 data)
{
    __asm__ volatile("outw %0, %1" : : "a"(data), "Nd"(port) : "memory");
}

UInt32 HwIoIn32(UInt16 port)
{
    UInt32 ret;
    __asm__ volatile("inl %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void HwIoOut32(UInt16 port, UInt32 data)
{
    __asm__ volatile("outl %0, %1" : : "a"(data), "Nd"(port) : "memory");
}

void HwIoWait(void)
{
    HwIoOut8(HW_IO_WAIT_PORT, 0);
}
