#include "stdio.h"
#include "stdlib.h"
#include "sys/mman.h"
#include "assert.h"
#include <time.h>
#include <sched.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE (1ULL<<30)
#define MMAP_FLAGS (MAP_ANONYMOUS|MAP_PRIVATE|MAP_POPULATE|MAP_HUGETLB|(30<<MAP_HUGE_SHIFT))
#define mem 71582
#define HUGETLBFS "/mnt/lab1/buff"

#ifndef CF
#define CF -1
#endif 
unsigned char* tmp[BUFFER_SIZE/mem];


//-----------------------------------------------------------------
// from https://graphics.stanford.edu/~seander/bithacks.html#NextBitPermutation
size_t next_bit_perm(size_t v) {
        size_t t = v | (v - 1);
        return (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctzl(v) + 1));
}

void swap(unsigned long  *xp, unsigned long  *yp) 
{ 
    unsigned long  temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
  
// A function to implement bubble sort 
void bubbleSort(unsigned long  arr[], unsigned long  n) 
{ 
   int i, j; 
   for (i = 0; i < n-1; i++)       
  
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
} 

void clflush(volatile void *p){
	asm volatile ("clflush (%0)" :: "r"(p));
}

void generate_random(unsigned char* l, unsigned char* r) { //this will generate random number in range l and r
	int i;
	unsigned char* rand_num;

	for (i = 0; i < BUFFER_SIZE/mem; i++) {
      	tmp[i] = (rand() % (r - l + 1)) + l;
	}
}


static __inline__ unsigned long rdtsc_s(void)
{
  unsigned a, d; 
  asm volatile("cpuid" ::: "%rax", "%rbx", "%rcx", "%rdx");
  asm volatile("rdtsc" : "=a" (a), "=d" (d)); 
  return ((unsigned long)a) | (((unsigned long)d) << 32); 
}

static __inline__ unsigned long rdtsc_e(void)
{
  unsigned a, d; 
  asm volatile("rdtscp" : "=a" (a), "=d" (d)); 
  asm volatile("cpuid" ::: "%rax", "%rbx", "%rcx", "%rdx");
  return ((unsigned long)a) | (((unsigned long)d) << 32); 
}


u_int64_t time_mes(volatile char* base, volatile char* rand_addr){
	int x = 1500;
	int i = 0;
	unsigned long media[1500];
	unsigned long tick,tick1;
	sched_yield();
	
	while(x--){
		tick = rdtsc_s();
		*base;
		*rand_addr;
		clflush(base);
		clflush(rand_addr);
		tick1 = rdtsc_e();
		media[i++] = tick1-tick;
	}
	bubbleSort(media,1500);


	if((media[1500/2])>=CF){
		return 1;

	}
	return 0;
}

int main() {

	if (CF == -1) {
		fprintf(stderr,"Cut-off value not defined\n");
		exit(1);
	}

	unsigned char* buff;
	int fd_hugepage;
	unsigned char* base ;
	int j = 0;
	int x = 0;
	int k = 0;
	int l = 0;
	int function_counter=0;
	unsigned long next_bit_for_one = 0;
	unsigned long functions[BUFFER_SIZE/mem];
	unsigned long top = 536870912;
	unsigned char* conflict_addr[BUFFER_SIZE/mem/28];

	fd_hugepage = open(HUGETLBFS, O_CREAT|O_RDWR);
	assert(fd_hugepage != -1);
	
	buff = (unsigned char*) mmap(NULL, BUFFER_SIZE, PROT_READ|PROT_WRITE, MMAP_FLAGS, fd_hugepage, 0 );
	assert(buff != (void*)-1);

/*
 *	__insert code here__
 * */

	for(int e=0;e<BUFFER_SIZE/mem;e++)
		functions[e]=0;


	srand(time(0));
	generate_random(buff,buff + BUFFER_SIZE);

	base = tmp[rand() % BUFFER_SIZE/mem];
	for(int i =0; i<BUFFER_SIZE/mem;i++){
		x = time_mes(base,tmp[i]);
		if(x){
			//printf("%d\n",j);
			conflict_addr[j++] = tmp[i];
		}
	}

	printf("%d\n",j);
	for(int i=0;i<6;i++){
		next_bit_for_one = next_bit_for_one*2+1;
		while(next_bit_for_one < top){
			next_bit_for_one = next_bit_perm(next_bit_for_one);
			for(k=0;k<j;k++){
				if((u_int64_t)base ^ next_bit_for_one == (u_int64_t)conflict_addr[k] ^ next_bit_for_one){
					function_counter++;
				}
			}

			if( function_counter >= j*0.9){
				functions[l++]= next_bit_for_one;
			}
		}
		next_bit_for_one = i*2+1;
		top = top + top/2;
	}

	j=0;
	while(functions[j]!=0){
		printf("%lx\n",functions[j]);
		j++;
	}
	
	assert(munmap(buff, BUFFER_SIZE) == 0);
	return 0;

}
