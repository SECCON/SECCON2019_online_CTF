#!/usr/bin/env python2
## -*- coding: utf-8 -*-
##
## An example of a small symbolic emulator for elf x86-64 binaries.
## Only simulates these following libc functions (but feel free to
## add more ones):
##
##  * __libc_start_main
##  * atoi
##  * atol
##  * atoll
##  * malloc
##  * printf
##  * putchar
##  * puts
##  * raise
##  * rand
##  * signal
##  * strlen
##  * strtoul


from __future__ import print_function
from triton     import TritonContext, ARCH, MemoryAccess, CPUSIZE, REG, Instruction, OPCODE, MODE

import sys
import string
import random

import trace

Triton = TritonContext()
Trace = None

# Script options
DEBUG = True

# Memory mapping
BASE_PLT   = 0x10000000
BASE_ARGV  = 0x20000000
BASE_ALLOC = 0x30000000
BASE_STACK = 0x9fffffff

# Signal handlers used by raise() and signal()
sigHandlers = dict()

# Allocation information used by malloc()
mallocCurrentAllocation = 0
mallocMaxAllocation     = 2048
mallocBase              = BASE_ALLOC
mallocChunkSize         = 0x00010000



def getMemoryString(addr):
    s = str()
    index = 0

    while Triton.getConcreteMemoryValue(addr+index):
        c = chr(Triton.getConcreteMemoryValue(addr+index))
        if c not in string.printable: c = ""
        s += c
        index  += 1

    return s


def getFormatString(addr):
    return getMemoryString(addr)                                                    \
           .replace("%s", "{}").replace("%d", "{:d}").replace("%#02x", "{:#02x}")   \
           .replace("%#x", "{:#x}").replace("%x", "{:x}").replace("%02X", "{:02x}") \
           .replace("%c", "{:c}").replace("%02x", "{:02x}").replace("%ld", "{:d}")  \
           .replace("%*s", "").replace("%lX", "{:x}").replace("%08x", "{:08x}")     \
           .replace("%u", "{:d}").replace("%lu", "{:d}")                            \


# Simulate the rand() function
def __rand():
    debug('rand hooked')
    # Return value
    return random.randrange(0xffffffff)


# Simulate the malloc() function
def __malloc(inst, size=0):
    global mallocCurrentAllocation
    global mallocMaxAllocation
    global mallocBase
    global mallocChunkSize

    debug('malloc hooked')

    # Get arguments
    if size == 0:
        size = Triton.getConcreteRegisterValue(Triton.registers.rdi)

    if size > mallocChunkSize:
        debug('malloc failed: size too big')
        sys.exit(-1)

    if mallocCurrentAllocation >= mallocMaxAllocation:
        debug('malloc failed: too many allocations done')
        sys.exit(-1)

    area = mallocBase + (mallocCurrentAllocation * mallocChunkSize)
    mallocCurrentAllocation += 1

    # Return value
    return area

def __calloc(inst):
    rdi = Triton.getConcreteRegisterValue(Triton.registers.rdi)
    rsi = Triton.getConcreteRegisterValue(Triton.registers.rsi)
    return __malloc(inst, rdi * rsi)

# Simulate the signal() function
def __signal():
    debug('signal hooked')

    # Get arguments
    signal  = Triton.getConcreteRegisterValue(Triton.registers.rdi)
    handler = Triton.getConcreteRegisterValue(Triton.registers.rsi)

    global sigHandlers
    sigHandlers.update({signal: handler})

    # Return value (void)
    return Triton.getConcreteRegisterValue(Triton.registers.rax)


# Simulate the raise() function
def __raise():
    debug('raise hooked')

    # Get arguments
    signal  = Triton.getConcreteRegisterValue(Triton.registers.rdi)
    handler = sigHandlers[signal]

    Triton.processing(Instruction(b"\x6A\x00")) # push 0
    emulate(handler)

    # Return value
    return 0


# Simulate the strlen() function
def __strlen():
    debug('strlen hooked')

    # Get arguments
    arg1 = getMemoryString(Triton.getConcreteRegisterValue(Triton.registers.rdi))

    # Return value
    return len(arg1)


# Simulate the strtoul() function
def __strtoul():
    debug('strtoul hooked')

    # Get arguments
    nptr   = getMemoryString(Triton.getConcreteRegisterValue(Triton.registers.rdi))
    endptr = Triton.getConcreteRegisterValue(Triton.registers.rsi)
    base   = Triton.getConcreteRegisterValue(Triton.registers.rdx)

    # Return value
    return int(nptr, base)


# Simulate the printf() function
def __printf(inst):
    debug('printf hooked')

    # Get arguments
    arg1   = getFormatString(Triton.getConcreteRegisterValue(Triton.registers.rdi))
    arg2   = Triton.getConcreteRegisterValue(Triton.registers.rsi)
    arg3   = Triton.getConcreteRegisterValue(Triton.registers.rdx)
    arg4   = Triton.getConcreteRegisterValue(Triton.registers.rcx)
    arg5   = Triton.getConcreteRegisterValue(Triton.registers.r8)
    arg6   = Triton.getConcreteRegisterValue(Triton.registers.r9)
    nbArgs = arg1.count("{")
    args   = [arg2, arg3, arg4, arg5, arg6][:nbArgs]
    s      = arg1.format(*args)

    sys.stdout.write(s)

    # Return value
    return len(s)


# Simulate the putchar() function
def __putchar():
    debug('putchar hooked')

    # Get arguments
    arg1 = Triton.getConcreteRegisterValue(Triton.registers.rdi)
    sys.stdout.write(chr(arg1) + '\n')

    # Return value
    return 2


# Simulate the puts() function
def __puts(instruction):
    debug('puts hooked')

    # Get arguments
    arg1 = getMemoryString(Triton.getConcreteRegisterValue(Triton.registers.rdi))
    print('puts("{}")'.format(arg1))

    # Return value
    return len(arg1) + 1


def __libc_start_main(instruction):
    debug('__libc_start_main hooked')

    # Get arguments
    main = Triton.getConcreteRegisterValue(Triton.registers.rdi)

    # Push the return value to jump into the main() function
    Triton.concretizeRegister(Triton.registers.rsp)
    Triton.setConcreteRegisterValue(Triton.registers.rsp, Triton.getConcreteRegisterValue(Triton.registers.rsp)-CPUSIZE.QWORD)

    ret2main = MemoryAccess(Triton.getConcreteRegisterValue(Triton.registers.rsp), CPUSIZE.QWORD)
    Triton.concretizeMemory(ret2main)
    Triton.setConcreteMemoryValue(ret2main, main)

    # Setup argc / argv
    Triton.concretizeRegister(Triton.registers.rdi)
    Triton.concretizeRegister(Triton.registers.rsi)

    # Setup target argvs
    argvs = sys.argv[2:]

    # Define argc / argv
    base  = BASE_ARGV
    addrs = list()

    # Set content of argv
    index = 0
    for argv_i, argv in enumerate(argvs):
        addrs.append(base)
        # トレース時の引数を書き込む
        # Triton.setConcreteMemoryAreaValue(base, bytes(argv.encode('utf8')) + b'\x00')

        # Symbolize argv
        # Tainting argvs
        for i in range(len(argv) + 1): # + 1 is null byte
            Triton.convertMemoryToSymbolicVariable(MemoryAccess(base + i, CPUSIZE.BYTE), "argv[{}][{}]".format(argv_i, i))
            Triton.taintMemory(base + i)

        base += len(argv)+1
        debug('argv[%d] = %s' %(index, argv))
        index += 1

    # Set argv pointer list
    argc = len(argvs)
    argv = base
    for addr in addrs:
        Triton.setConcreteMemoryValue(MemoryAccess(base, CPUSIZE.QWORD), addr)
        base += CPUSIZE.QWORD

    Triton.setConcreteRegisterValue(Triton.registers.rdi, argc)
    # Triton.convertRegisterToSymbolicVariable(Triton.registers.rdi, "argv[1]")
    Triton.setConcreteRegisterValue(Triton.registers.rsi, argv)

    return 0


# Simulate the atoi() function
def __atoi(instruction):
    debug('atoi hooked')

    # 2文字以上だと解なしになってしまう
    atoi_argv_len = 1

    # Get arguments
    arg1_ptr = Triton.getConcreteRegisterValue(Triton.registers.rdi)
    arg1 = getMemoryString(arg1_ptr)
    # print('atoi("{}")'.format(arg1))

    if Triton.getMemoryAst(MemoryAccess(arg1_ptr, CPUSIZE.BYTE)).isSymbolized():
        debug("atoi is symbolized")
        sym_arg1_offset = []
        for i in range(atoi_argv_len):
            sym_arg1_offset.append(Triton.getMemoryAst(MemoryAccess(arg1_ptr + i, CPUSIZE.BYTE)))

        # Symbolize return register
        Triton.convertRegisterToSymbolicVariable(Triton.registers.rax)
        reg_rax = Triton.getSymbolicRegister(Triton.registers.rax)

        ast = Triton.getAstContext()

        ret_value = 0
        for i in range(atoi_argv_len):
            ret_value += (sym_arg1_offset[atoi_argv_len - i - 1] - ord('0')) * (10 ** i)
            conditional_branch_constraints.append(ast.land([sym_arg1_offset[i] >= ord('0'), sym_arg1_offset[i] <= ord('9')]))

        padding_size = reg_rax.getAst().getBitvectorSize() - CPUSIZE.BYTE_BIT
        rax_ast = ast.sx(
            padding_size,
            ret_value
            )
        Triton.createSymbolicRegisterExpression(instruction, rax_ast, Triton.registers.rax, "assgin return value")

        # import ipdb; ipdb.set_trace()
        return 0
    else:
        # Return value
        return int(arg1)


# Simulate the atol() function
def __atol():
    debug('atol hooked')

    # Get arguments
    arg1 = getMemoryString(Triton.getConcreteRegisterValue(Triton.registers.rdi))

    # Return value
    return int(arg1)


# Simulate the atoll() function
def __atoll():
    debug('atoll hooked')

    # Get arguments
    arg1 = getMemoryString(Triton.getConcreteRegisterValue(Triton.registers.rdi))

    # Return value
    return int(arg1)


def __exit():
    debug('exit hooked')

    # Get arguments
    arg1 = Triton.getConcreteRegisterValue(Triton.registers.rdi)
    print('exit({})'.format(arg1))

    # Return value
    return int(arg1)


customRelocation = [
    ['__libc_start_main', __libc_start_main,    None],
    ['atoi',              __atoi,               None],
    ['atol',              __atol,               None],
    ['atoll',             __atoll,              None],
    ['exit',              __exit,               None],
    ['malloc',            __malloc,             None],
    ['calloc',            __calloc,             None],
    ['printf',            __printf,             None],
    ['putchar',           __putchar,            None],
    ['puts',              __puts,               None],
    ['raise',             __raise,              None],
    ['rand',              __rand,               None],
    ['signal',            __signal,             None],
    ['strlen',            __strlen,             None],
    ['strtoul',           __strtoul,            None],
]


def hookingHandler(instruction):
    pc = Triton.getConcreteRegisterValue(Triton.registers.rip)
    for rel in customRelocation:
        if rel[2] == pc:
            # Emulate the routine and the return value
            ret_value = rel[1](instruction)
            # Triton.concretizeRegister(Triton.registers.rax)
            Triton.setConcreteRegisterValue(Triton.registers.rax, ret_value)

            if rel[0] == 'exit':
                return True

            # Get the return address
            ret_addr = Triton.getConcreteMemoryValue(MemoryAccess(Triton.getConcreteRegisterValue(Triton.registers.rsp), CPUSIZE.QWORD))

            # Hijack RIP to skip the call
            Triton.concretizeRegister(Triton.registers.rip)
            Triton.setConcreteRegisterValue(Triton.registers.rip, ret_addr)

            # Restore RSP (simulate the ret)
            Triton.concretizeRegister(Triton.registers.rsp)
            Triton.setConcreteRegisterValue(Triton.registers.rsp, Triton.getConcreteRegisterValue(Triton.registers.rsp)+CPUSIZE.QWORD)

            return False
    return False

conditional_branch_constraints = []
# Emulate the binary.
def emulate(pc):
    count = 0
    while pc:
        # Fetch opcode
        opcode = Triton.getConcreteMemoryAreaValue(pc, 16)

        # Create the Triton instruction
        instruction = Instruction()
        instruction.setOpcode(opcode)
        instruction.setAddress(pc)

        # Process
        Triton.processing(instruction)
        count += 1

        # Print instruction
        # print("pc = {:#x}, rax = {:#x}".format(pc, Triton.getConcreteRegisterValue(Triton.registers.rax)))
        # debug(instruction)

        # for expr in instruction.getSymbolicExpressions():
        #     print('\t', expr)

        if instruction.getType() == OPCODE.X86.HLT:
            break

        # Simulate routines
        flag_exit = hookingHandler(instruction)
        if flag_exit:
            break

        # Update branch constraint
        reg_zf = Triton.getRegisterAst(Triton.getRegister(REG.X86.ZF))
        reg_sf = Triton.getRegisterAst(Triton.getRegister(REG.X86.SF))
        reg_of = Triton.getRegisterAst(Triton.getRegister(REG.X86.OF))
        reg_cf = Triton.getRegisterAst(Triton.getRegister(REG.X86.CF))
        ast = Triton.getAstContext()

        # Next
        if instruction.isBranch() and not instruction.getType() == OPCODE.X86.JMP:
            # メインのバイナリのトレースになるまで読み飛ばし
            while True:
                event = Trace.get_next_branch()
                # debug(event)
                # if Trace.get_relative_addr_and_image(event['inst_addr'])[0] == pc:
                #     break
                if event['inst_addr'].startswith('0x5') and int(event['inst_addr'], 16) & 0xfff == pc & 0xfff:
                    break
            # debug(event)

            # 分岐条件を収集
            def ja_succ(cf, zf):
                # CF=0 and ZF=0
                return ast.land([cf == 0, zf == 0])

            def jae_succ(cf):
                return cf == 0

            def jb_succ(cf):
                return cf == 1

            def jbe_succ(cf, zf):
                # CF=1 or ZF=1
                return ast.lor([cf == 1, zf == 1])

            def jg_succ(zf, sf, of):
                # ZF=0 and SF=OF
                return ast.land([zf == 0, sf == of])

            def jge_succ(sf, of):
                # SF=OF
                return sf == of

            def jl_succ(sf, of):
                # SF ≠ OF
                return sf != of

            def jle_succ(zf, sf, of):
                # ZF=1 or SF ≠ OF                
                return ast.lor([zf == 1, sf != of])

            def je_succ(zf):
                return zf == 1

            def jne_succ(zf):
                return zf == 0

            def selecter(instruction, event, succ_opcode, succ, fail_opcode, fail):
                if instruction.getType() == succ_opcode:
                    if event['branch_taken']:
                        return succ
                    else:
                        return fail
                if instruction.getType() == fail_opcode:
                    if event['branch_taken']:
                        return fail
                    else:
                        return succ

            if False:
                pass
            elif instruction.getType() in [OPCODE.X86.JA, OPCODE.X86.JBE]:
                conditional_branch_constraints.append(selecter(instruction, event, OPCODE.X86.JA, ja_succ, OPCODE.X86.JBE, jbe_succ)(cf=reg_cf, zf=reg_zf))
            elif instruction.getType() in [OPCODE.X86.JB, OPCODE.X86.JAE]:
                conditional_branch_constraints.append(selecter(instruction, event, OPCODE.X86.JB, jb_succ, OPCODE.X86.JAE, jae_succ)(cf=reg_cf, zf=reg_zf))
            elif instruction.getType() in [OPCODE.X86.JL, OPCODE.X86.JGE]:
                conditional_branch_constraints.append(selecter(instruction, event, OPCODE.X86.JL, jl_succ, OPCODE.X86.JGE, jge_succ)(sf=reg_sf, of=reg_of))
            elif instruction.getType() in [OPCODE.X86.JG, OPCODE.X86.JLE]:
                conditional_branch_constraints.append(selecter(instruction, event, OPCODE.X86.JG, jg_succ, OPCODE.X86.JLE, jle_succ)(zf=reg_zf, sf=reg_sf, of=reg_of))
            elif instruction.getType() in [OPCODE.X86.JE, OPCODE.X86.JNE]:
                conditional_branch_constraints.append(selecter(instruction, event, OPCODE.X86.JE, je_succ, OPCODE.X86.JNE, jne_succ)(zf=reg_zf))
            else:
                raise Exception("Unhandled jcc instruction: {}".format(instruction))

            # 分岐結果をプログラムカウンタに反映
            if event['branch_taken']:
                pc = int(instruction.getDisassembly().split(' ')[-1], 16)
                continue
            else:
                pc = pc + instruction.getSize()
        else:
            pc = Triton.getConcreteRegisterValue(Triton.registers.rip)

    debug('Number of instruction executed: %d' %(count))
    return


def loadBinary(path):
    import lief
    # Map the binary into the memory
    binary = lief.parse(path)
    phdrs  = binary.segments
    for phdr in phdrs:
        size   = phdr.physical_size
        vaddr  = phdr.virtual_address
        debug('Loading 0x%06x - 0x%06x' %(vaddr, vaddr+size))
        Triton.setConcreteMemoryAreaValue(vaddr, phdr.content)
    return binary

def makeRelocation(ctx, binary):
    # Assign address
    for i, crel in enumerate(customRelocation):
        crel[2] = BASE_PLT + i
    
    # Perform our own relocations
    try:
        for rel in binary.pltgot_relocations:
            symbolName = rel.symbol.name
            symbolRelo = rel.address
            for crel in customRelocation:
                if symbolName == crel[0]:
                    debug('[+] Hooking %s' %(symbolName))
                    ctx.setConcreteMemoryValue(MemoryAccess(symbolRelo, CPUSIZE.QWORD), crel[2])
    except:
        pass

    # Perform our own relocations
    try:
        for rel in binary.dynamic_relocations:
            symbolName = rel.symbol.name
            symbolRelo = rel.address
            for crel in customRelocation:
                if symbolName == crel[0]:
                    debug('[+] Hooking %s' %(symbolName))
                    ctx.setConcreteMemoryValue(MemoryAccess(symbolRelo, CPUSIZE.QWORD), crel[2])
    except:
        pass
    return

def solve():
    debug("=== solve")
    ast = Triton.getAstContext()
    pco = Triton.getPathConstraintsAst()
    if len(conditional_branch_constraints) > 1:
        bco = ast.land(conditional_branch_constraints)
    else:
        bco = conditional_branch_constraints[0]
    # print("pco = {}".format(pco))
    # print("bco = {}".format(bco))

    # models = Triton.getModel(ast.land([pco, bco]))
    # models = Triton.getModel(pco)
    models = Triton.getModel(bco)
    test_input = ""
    for k, v in sorted(models.items(), key=lambda v: v[0]):
        value = v.getValue()
        print("{} = {} {} -- {}".format(v.getVariable(), value, repr(chr(value)) if 0<= value and value < 256 else '', v.getVariable().getComment()))
        test_input += chr(value)
    test_input = test_input[:test_input.find('\0')] # Trim null byte
    print("input = {!r}".format(test_input))
    return test_input

def test_run(args):
    import subprocess
    p = subprocess.Popen(args)
    stdout, stderr = p.communicate()
    return

def debug(s):
    if DEBUG:
        print('[Triton] %s' %(s))
    return


if __name__ == '__main__':
    # Set the architecture
    Triton.setArchitecture(ARCH.X86_64)

    # Set a symbolic optimization mode
    Triton.enableMode(MODE.ALIGNED_MEMORY, True)
    Triton.enableMode(MODE.ONLY_ON_SYMBOLIZED, True)

    # AST representation as Python syntax
    #setAstRepresentationMode(AST_REPRESENTATION.PYTHON)

    if len(sys.argv) < 2:
        debug('Syntax: %s <tracer log> <elf binary> [arg1, arg2, ...]' %(sys.argv[0]))
        sys.exit(1)

    tracer_log = sys.argv[1]
    binary_file = sys.argv[2]

    Trace = trace.Trace(tracer_log)

    # Load the binary
    binary = loadBinary(binary_file)

    # Perform our own relocations
    makeRelocation(Triton, binary)

    # Define a fake stack
    Triton.setConcreteRegisterValue(Triton.registers.rbp, BASE_STACK)
    Triton.setConcreteRegisterValue(Triton.registers.rsp, BASE_STACK)

    # Let's emulate the binary from the entry point
    debug('Starting emulation')
    emulate(binary.entrypoint)
    debug('Emulation done')


    # Solve
    sys.stdout.flush()
    test_input = solve()
    sys.stdout.flush()

    with open('test-input', 'w') as f:
        f.write(test_input)

    # # test run
    # test_input = test_input[:test_input.find('\0')] # trim with null
    # test_run([binary_file, test_input])

    sys.exit(0)
