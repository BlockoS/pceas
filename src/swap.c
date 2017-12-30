#include <string.h>
#include "swap.h"

#define SWAP_BUFFER_SIZE 32

/**
 * Swaps @b size bytes from the memory location pointed by @b a with @b b.
 */
void swap(uint8_t* a, uint8_t *b, uint32_t len)
{
    uint8_t buffer[SWAP_BUFFER_SIZE];
    while(len > 0)
    {
        uint32_t sz = (len > SWAP_BUFFER_SIZE) ? SWAP_BUFFER_SIZE : len;
        memcpy(buffer, a, sz);
        memcpy(a, b, sz);
        memcpy(b, buffer, sz);
        len -= sz;
    }
}
