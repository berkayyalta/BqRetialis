// Made by Berkay

.global CpIsrStubDefault
.global CpSyscallStubDefault
.global CpSyscallStub
.global CpIsrStubTable
.global CpIsrCommonStub
.global CpCtxLoad
.global CpSmpTrampolineStart
.global CpSmpTrampolineStage64
.global CpSmpTrampolineEnd

.extern CpCtxDispatch
.extern CpSyscallDispatch

.code64
CpIsrStubDefault:
    cli
    hlt
    jmp CpIsrStubDefault

CpSyscallStubDefault:
    sysretq

CpSyscallStub:
    swapgs
    movq %rsp, %gs:0x08
    movq %gs:0x00, %rsp
    pushq $0x1B
    pushq %gs:0x08
    pushq %r11
    pushq $0x23
    pushq %rcx
    pushq $0
    pushq $0x80
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    movq %cr3, %rax
    pushq %rax
    movq %rsp, %rdi
    movq %rsp, %rbp
    andq $-16, %rsp
    call CpSyscallDispatch
    movq %rbp, %rsp
    popq %rax
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    addq $16, %rsp
    popq %rcx
    addq $8, %rsp
    popq %r11
    popq %rsp
    swapgs
    sysretq

CpIsrCommonStub:
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    movq %cr3, %rax
    pushq %rax
    testb $3, 152(%rsp)
    jz 1f
    swapgs
1:
    movq %rsp, %rsi
    pushq %rsi
    movq %rsp, %rdi
    movq %rsp, %rbp
    andq $-16, %rsp
    call CpCtxDispatch
    movq %rbp, %rsp
    popq %rdi
CpCtxLoad:
    cli
    movq %rdi, %rsp
    testb $3, 152(%rsp)
    jz 2f
    swapgs
2:
    popq %rax
    movq %cr3, %rcx
    cmpq %rcx, %rax
    je 3f
    movq %rax, %cr3
3:
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    addq $16, %rsp
    iretq

.altmacro
.macro CP_ISR_STUB vector
CpIsrStub\vector:
.if (\vector == 8) || (\vector == 10) || (\vector == 11) || (\vector == 12) || (\vector == 13) || (\vector == 14) || (\vector == 17) || (\vector == 21) || (\vector == 29) || (\vector == 30)
    pushq $\vector
    jmp CpIsrCommonStub
.else
    pushq $0
    pushq $\vector
    jmp CpIsrCommonStub
.endif
.endm

.set i, 0
.rept 256
    CP_ISR_STUB %i
    .set i, i + 1
.endr

.balign 8
.macro CP_ISR_TABLE_ENTRY vector
    .quad CpIsrStub\vector
.endm

CpIsrStubTable:
.set i, 0
.rept 256
    CP_ISR_TABLE_ENTRY %i
    .set i, i + 1
.endr

.code16
.balign 16
CpSmpTrampolineStart:
    cli
    cld
    movw %cs, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    lgdtl (0x0F40)
    movl %cr4, %eax
    orl $0x000006A0, %eax
    movl %eax, %cr4
    movl (0x0F00), %eax
    movl %eax, %cr3
    movl $0xC0000080, %ecx
    rdmsr
    orl $0x00000100, %eax
    wrmsr
    movl %cr0, %eax
    andl $0xFFFFFFF3, %eax
    orl $0x80000003, %eax
    movl %eax, %cr0
    ljmpl *(0x0F04)

.code64
CpSmpTrampolineStage64:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    leaq (%rip), %rbx
    andq $-4096, %rbx
    addq $0x0F00, %rbx
    movq 0x10(%rbx), %rsp
    movl 0x0C(%rbx), %edi
    movl $1, 0x20(%rbx)
    jmpq *0x18(%rbx)
CpSmpTrampolineEnd:
