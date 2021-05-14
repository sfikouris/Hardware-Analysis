
CF = #cut-off 
CFLAGS = -I./ 

all: conflicts functions rows 
	

conflicts: conflicts.c 
	gcc  -o $@  $^ $(CFLAGS)  



functions: functions.c 
	gcc  $(CF) -o $@ $^ $(CFLAGS) 

rows: row-index.c 
	gcc  $(CF) -o $@ $^ $(CFLAGS) 

clean:
	rm -f conflicts
	rm -f functions	
	rm -f rows
