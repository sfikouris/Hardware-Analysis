#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef SECRET                              
#define SECRET 0x4c65616b206d6521           
#endif       


     

#define SECRET_LENGTH 8

uint64_t secret = __builtin_bswap64(SECRET);

void clflush(volatile void *p){
	asm volatile ("clflush (%0)" :: "r"(p));
}


/*I TOOK THIS FUNCTION FROM https://www.usenix.org/system/files/conference/usenixsecurity14/sec14-paper-yarom.pdf */
unsigned long probe( unsigned char *address ) {
    volatile unsigned long time;
    asm __volatile__ (
        "  mfence             \n"
        "  lfence             \n"
        "  rdtsc              \n"
        "  lfence             \n"
        "  movl %%eax, %%esi  \n"
        "  movl (%1), %%eax   \n"
        "  lfence             \n"
        "  rdtsc              \n"
        "  subl %%esi, %%eax  \n"
        : "=a" (time)
        : "c" (address)
        :  "%esi", "%edx");
    return time ;
}



int main(int argc, char *argv[]) {
// #ifdef TESTING
// int cntr = 0;
// for(int retry = 0; retry < 100; ++retry){
// #endif  

    char leaked_secret[SECRET_LENGTH + 1] ={0};

    // You need to properly set the following variables
    int stride=4096;
    int reloadbuffer_size=256;
    unsigned char *reloadbuffer;
    unsigned long miss_val=0,hit_val=0;
    unsigned long max = 0;

    reloadbuffer = malloc((reloadbuffer_size * stride));

    for(int i=0;i<reloadbuffer_size*stride;i++){
        reloadbuffer[i] = 0;
    }
    probe(reloadbuffer+(3*stride));
    
    for(int i=0;i<reloadbuffer_size*stride;i++){
        clflush(reloadbuffer+i);
    }

#ifdef EVICT 

    // Your EVICT code goes here

#else

  
    for(int i=0;i<10000;i++){
            clflush(reloadbuffer+(4*stride));
            miss_val =miss_val+probe(reloadbuffer+(4*stride));
            hit_val=hit_val+probe(reloadbuffer+(4*stride));
    }
    printf("miss: %lu, hit: %lu\n",(miss_val/10000),(hit_val/10000));
   
#endif

    /*
     * Encode the SECRET bytes into the reloadbuffer
     */

    // Your RELOAD code goes here 

  
    int l=0;
    int t=0;
    int thesi;
    int check_buf[256];
    unsigned long min = 1000;
    unsigned long tmp;
     //printf("%lx\n",secret);
    
    for(int i=0;i<256;i++)
        check_buf[i] = 0;

    for(int i=0;i<reloadbuffer_size*stride;i++){
        clflush(reloadbuffer+i);
        //printf("time %lu \n", probe(reloadbuffer+i));      
    }

    while(secret!=0){
      //  printf("DAME\n");
        for(int i=0;i<reloadbuffer_size*stride;i++){
            clflush(reloadbuffer+i);
        }

        unsigned char byte = ((secret>>0) & 0xff);
        //printf("byte : %d\n",byte);

        for(int h=0;h<10000;h++){

            for(int i=0;i<reloadbuffer_size;i++){
                clflush(reloadbuffer+(i*stride));
            }

           
            *(reloadbuffer+(byte*stride))=0; 
            // probe(reloadbuffer+(byte*stride));

            for(int k=0;k<reloadbuffer_size;k++){
                l = ((k * 167) + 13) & 0xff;
                tmp = probe(reloadbuffer+(l*stride));
                if(tmp<=(((hit_val/10000)+(miss_val/10000))/2)){
                    check_buf[l]=check_buf[l] + 1; 
                }
            }
        }


        for(int f=0;f<256;f++){
            if(max<check_buf[f]){
                max = check_buf[f];
                thesi = f;
            }
        }
       // printf("thesi %d\n",thesi);
        leaked_secret[t++]=thesi;

        for(int i=0;i<256;i++)
            check_buf[i] = 0;
        max = 0;

        secret >>=8;

    }
  
    
    printf("\nSecret: %s\n", leaked_secret);

    // #ifdef TESTING
    
    // if(!strcmp(leaked_secret,"Leak me!")) cntr++;

    // }//retry for loop

    // printf("%d/100\n",cntr);
    // #endif

    return 0;

}
