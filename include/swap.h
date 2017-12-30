#ifndef PCEAS_SWAP_H
#define PCEAS_SWAP_H

#include <stdint.h>

/**
 * Swaps @b size bytes from the memory location pointed by @b a with @b b.
 */
void swap(uint8_t* a, uint8_t* b, uint32_t size);

#endif /* PCEAS_SWAP_H */
