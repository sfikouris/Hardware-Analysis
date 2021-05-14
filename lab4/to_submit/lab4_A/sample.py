#!/usr/bin/env python
# Sample code for ARM of Unicorn. Nguyen Anh Quynh <aquynh@gmail.com>
# Python sample ported by Loi Anh Tuan <loianhtuan@gmail.com>

#08002f54 - 08002fa8 rehosting 0x08002f56
#08002fc8 sleep
# Dump the firmware of rehosting address or also modpow function.

"""
r0             0x13                19
r1             0x20000380          536871808
r2             0x40                64
r3             0x200019cc          536877516
r4             0x80084bd           134251709
r5             0x8004de8           134237672
r6             0x8004e00           134237696
r7             0x32                50
r8             0x0                 0
r9             0x0                 0
r10            0x0                 0
r11            0x0                 0
r12            0x8007889           134248585
sp             0x200019c8          0x200019c8 <_main_stack+4080>
lr             0x8004ddb           134237659
pc             0x8002f56           0x8002f56 <assignment_rehosting()+2>
xPSR           0x61000000          1627389952
msp            0x20013fd0          0x20013fd0
psp            0x200019c8          0x200019c8 <_main_stack+4080>
primask        0x0                 0
basepri        0x0                 0
faultmask      0x0                 0
control        0x2                 2

0x08000000 = base 
"""




from __future__ import print_function
from unicorn import *
from unicorn.arm_const import *

ARM_CODE = open("firmware_dump", "rb")
RAM_CODE = open("mem_dump","rb")

# code to be emulated
#ARM_CODE   = b"\x37\x00\xa0\xe3\x03\x10\x42\xe0" # mov r0, #0x37; sub r1, r2, r3
# memory address where emulation starts
arm_ADDRESS = 0x08000000
ram_ADDRESS = 0x20000000
#hook put, flush, time.
instructions_skip_list = [0x08002fc8,0x08002f78,0x08002f58]
fun_putchar = 0x08002f72

# callback for tracing instructions
def hook_code(mu, address, size, user_data):
   
    if address in instructions_skip_list:
        mu.reg_write(UC_ARM_REG_R15, address + size | 1)

    
    elif address == 0x08002f72:
        c = mu.reg_read(UC_ARM_REG_R0)
        print(chr(c),end='')
        mu.reg_write(UC_ARM_REG_R15, address+size | 1)


        

# Test ARM
def test_arm():
    print("Emulate ARM code")
    try:
        # Initialize emulator in ARM mode
        mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)

        # map 2MB memory for this emulation
        mu.mem_map(arm_ADDRESS, 2 * 1024 * 1024)
        mu.mem_map(ram_ADDRESS, 80 * 1024)


        # write machine code to be emulated to memory
        mu.mem_write(arm_ADDRESS, ARM_CODE.read())
        mu.mem_write(ram_ADDRESS, RAM_CODE.read())

        # initialize machine registers
        mu.reg_write(UC_ARM_REG_R0, 0x13)
        mu.reg_write(UC_ARM_REG_R1, 0x20000380)
        mu.reg_write(UC_ARM_REG_R2, 0x40)
        mu.reg_write(UC_ARM_REG_R3, 0x200019cc)
        mu.reg_write(UC_ARM_REG_R4, 0x80084bd)
        mu.reg_write(UC_ARM_REG_R5, 0x8004de8)
        mu.reg_write(UC_ARM_REG_R6, 0x8004e00)
        mu.reg_write(UC_ARM_REG_R7, 0x32)
        mu.reg_write(UC_ARM_REG_R8, 0x0)
        mu.reg_write(UC_ARM_REG_R9, 0x0)
        mu.reg_write(UC_ARM_REG_R10, 0x0)
        mu.reg_write(UC_ARM_REG_R11, 0x0)
        mu.reg_write(UC_ARM_REG_R12, 0x8007889)
        mu.reg_write(UC_ARM_REG_R13, 0x200019c8 )
        mu.reg_write(UC_ARM_REG_R14, 0x8004ddb )
        mu.reg_write(UC_ARM_REG_R15, 0x8002f56 )
        mu.reg_write(UC_ARM_REG_MSP,0x20013fd0)
        mu.reg_write(UC_ARM_REG_PSP,0x200019c8)
        mu.reg_write(UC_ARM_REG_CPSR, 0x61000000)
        #mu.reg_write(UC_ARM_REG_APSR, 0xFFFFFFFF) #All application flags turned on

        # tracing all basic blocks with customized callback
        #mu.hook_add(UC_HOOK_BLOCK, hook_block)

        # tracing one instruction at ADDRESS with customized callback
        mu.hook_add(UC_HOOK_CODE, hook_code)

        # emulate machine code in infinite time
        mu.emu_start(0x08002f54 | 1 ,0x08002f60 | 1)  #arm_ADDRESS+len(ARM_CODE.read())

        # now print out some registers
        print(">>> Emulation done. Below is the CPU context")

        #r0 = mu.reg_read(UC_ARM_REG_R5)
        #print(">>> R0 = 0x%x" %r5)

    except UcError as e:
        print("ERROR: %s" % e)





test_arm()
print("=" * 26)
