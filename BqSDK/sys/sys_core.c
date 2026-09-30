// Made by Berkay

#include "sys_core.h"

BqStatus BqSyscall(AbiScId syscall_id, void* form)
{
    UInt64 result;
    __asm__ __volatile__(
        "syscall"
        : "=a"(result)
        : "a"((UInt64)syscall_id), "D"(form)
        : "rcx", "r11", "memory"
    );
    return (BqStatus)result;
}

void* memcpy(void* dest, const void* src, Size n)
{
    UInt8* d = (UInt8*)dest;
    const UInt8* s = (const UInt8*)src;
    for (Size i = 0; i < n; i++)
    {
        d[i] = s[i];
    }
    return dest;
}

void* memset(void* dest, int c, Size n)
{
    UInt8* d = (UInt8*)dest;
    for (Size i = 0; i < n; i++)
    {
        d[i] = (UInt8)c;
    }
    return dest;
}

void* memmove(void* dest, const void* src, Size n)
{
    UInt8* d = (UInt8*)dest;
    const UInt8* s = (const UInt8*)src;
    if (d < s)
    {
        for (Size i = 0; i < n; i++)
        {
            d[i] = s[i];
        }
    }
    else if (d > s)
    {
        for (Size i = n; i > 0; i--)
        {
            d[i - 1] = s[i - 1];
        }
    }
    return dest;
}

int memcmp(const void* s1, const void* s2, Size n)
{
    const UInt8* p1 = (const UInt8*)s1;
    const UInt8* p2 = (const UInt8*)s2;
    for (Size i = 0; i < n; i++)
    {
        if (p1[i] != p2[i])
        {
            return (int)p1[i] - (int)p2[i];
        }
    }
    return 0;
}
