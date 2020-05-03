###
##
# Entry point and setup for our RSP
##

.equ RAM_SIZE, 4096

.text
.align 2
.section .text
.global rsp_mips_start
.extern main
.ent rsp_mips_start
.type rsp_mips_start, @function

rsp_mips_start:
   nop
redo:
   li $a0, 0 # argc = 0, it will never be used anyways on this stuff. :D
   li $a1, 0 # argv = 0
   li $sp, (0x00700000 + RAM_SIZE - 4) # Set up stack.

   jal main
   j redo

.end rsp_mips_start
.size rsp_mips_start, .-rsp_mips_start

