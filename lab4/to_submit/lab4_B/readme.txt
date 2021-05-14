To run ROP.py
1) connect target to PC.
2) run ROP.py
3) On different terminal : sudo screen /dev/ttyACM0 9600 (blackscreen it's appear.)
4)copy/paste in the terminal that ROP.py runs the instructions from line 13-15 one by one


===============================================================================================

To find all the gadgets : ROPgadget --rawArch ARM --binary hwsec.elf --thumb


From ghidra I found that decrypt_flag function doesn't return a value BUT it load the pointer of flag to R4 register as function argument. 
Then I search for a gadget that moves R4 to R0. As we know R0 is the first argument of a function. 
For the last step I found a print. Print function check if it has a load arguments, and in this case it Found the pointer of flag and it printed it out.
gadgets : 08002d9a -> (Dycryptflag) movs r3, #0 ; cmp r3, r2 ; it ge ; popge {r4, r5, r7, pc}
	  080030a2 - > mov r0, r4 ; pop {r4, pc}
	  08002dc0  -> (print) mov r0, sp ; movs r1, #0xa ; bl #0x8002d62 ; add sp, #0x20 ; pop {r7, pc}
