#include "stdio.h"
#include "stdlib.h"
#include "sys/mman.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "assert.h"

#define BUFFER_SIZE (1ULL<<30)
#define MMAP_FLAGS (MAP_ANONYMOUS|MAP_PRIVATE|MAP_POPULATE|MAP_HUGETLB|(30<<MAP_HUGE_SHIFT))

#define HUGETLBFS "/mnt/lab1/buff"

#ifndef CF
#define CF -1
#endif 





int main() {
	unsigned char* buff;
	int fd_hugepage;	

	if (CF == -1) {
		fprintf(stderr,"Cut-off value not defined\n");
		exit(1);
	}

	fd_hugepage = open(HUGETLBFS, O_CREAT|O_RDWR);
	assert(fd_hugepage != -1);
	
	buff = (unsigned char*) mmap(NULL, BUFFER_SIZE, PROT_READ|PROT_WRITE, MMAP_FLAGS, fd_hugepage, 0 );
	assert(buff != (void*)-1);

/*
 *	__insert code here__
 * */

	

	assert (munmap(buff, BUFFER_SIZE) == 0);
	return 0;
}

