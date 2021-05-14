#include "stdio.h"
#include "stdlib.h"
#include "sys/mman.h"
#include "assert.h"

#define BUFFER_SIZE (1ULL<<30)
#define MMAP_FLAGS (MAP_PRIVATE|MAP_ANONYMOUS|MAP_POPULATE)




int main() {


	unsigned char* buff = (unsigned char*) mmap(NULL, BUFFER_SIZE, PROT_READ|PROT_WRITE, MMAP_FLAGS, -1,0);
	assert(buff != MAP_FAILED);

/*
 *	__insert code here__
 * */
	

	assert(munmap(buff, BUFFER_SIZE) ==0);
	return 0;

}
