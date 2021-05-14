
CF = 500 
CFLAGS = -I./ 

all: conflicts functions rows 
	

conflicts: conflicts.c 
	gcc  -o $@  $^ $(CFLAGS)  



functions: functions.c 
	gcc  $(CF) -o $@ $^ $(CFLAGS) 



clean:
	rm -f conflicts
	rm -f functions	

