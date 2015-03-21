#include <stdint.h>
#include "hash.h"

/**
 * Compute hash value using Shift-Add-XOR.
 * @param [in] key Input key.
 * @param [in] len Key length (in bytes).
 * @return Hash value.
 */
unsigned int compute_hash(const void* key, size_t len)
{
    uint8_t *ptr = (uint8_t*)key;
    unsigned int hash;
    for(hash=0; len; --len)
    {
        hash += *ptr;
        hash  = (hash << 3) + (hash >> 5) + *ptr++;
    }
    return hash;
}
