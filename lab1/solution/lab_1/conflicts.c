#include "stdio.h"
#include "stdlib.h"
#include "sys/mman.h"
#include "assert.h"
#include <time.h>
#include <sched.h>

#define BUFFER_SIZE (1ULL<<30)
#define MMAP_FLAGS (MAP_PRIVATE|MAP_ANONYMOUS|MAP_POPULATE)
#define mem 71582

unsigned char* tmp[BUFFER_SIZE/mem];





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

	
	return media[1500/2];
}

int main() {


	unsigned char* buff = (unsigned char*) mmap(NULL, BUFFER_SIZE, PROT_READ|PROT_WRITE, MMAP_FLAGS, -1,0);
	assert(buff != MAP_FAILED);
	unsigned char* base ;


/*
 *	__insert code here__
 * */

	srand(time(0));
	generate_random(buff,buff + BUFFER_SIZE);

	base = tmp[rand() % BUFFER_SIZE/mem];
	for(int i =0; i<BUFFER_SIZE/mem;i++){
		printf("%lu\n",time_mes(base,tmp[i]));
	}
	
	assert(munmap(buff, BUFFER_SIZE) ==0);
	return 0;

}
