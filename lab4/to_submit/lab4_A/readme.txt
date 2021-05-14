for static analysis: Using ghidra I found the exponents table and after that I just wrote the program again without the sleep.

Second solution. In ghidra I can nop.w the time function and then loadit to nucleo. 
I tried openocd -f board/stm32L1discovery.cfg -c "program hwsec.elf.bin exit 0x08000000"
	openocd -f board/stm32L152RET6.cfg -c "program hwsec.elf.bin exit 0x08000000"
But no luck.
Open On-Chip Debugger 0.10.0
Licensed under GNU GPL v2
For bug reports, read
	http://openocd.org/doc/doxygen/bugs.html
embedded:startup.tcl:60: Error: Can't find board/stm32L152RET6.cfg
in procedure 'script' 
at file "embedded:startup.tcl", line 60


