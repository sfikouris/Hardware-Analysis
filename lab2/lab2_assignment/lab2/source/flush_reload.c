#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef SECRET                              
#define SECRET 0x4c65616b206d6521           
#endif                                      
                                            
#define SECRET_LENGTH 8

uint64_t secret = __builtin_bswap64(SECRET);

int main(int argc, char *argv[]) {
    char leaked_secret[SECRET_LENGTH + 1] = {0};

    // You need to properly set the following variables
    unsigned char *reloadbuffer;
    int stride;
    int reloadbuffer_size;

#ifdef EVICT 

    // Your EVICT code goes here

#else

    // Your FLUSH code goes here

#endif

    /*
     * Encode the SECRET bytes into the reloadbuffer
     */

    // Your RELOAD code goes here 

    printf("Secret: %s\n", leaked_secret);

    return 0;

}
