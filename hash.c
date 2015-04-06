#include "hash.h"

/**
 * Compute hash value using Shift-Add-XOR.
 * @param [in] key Input key.
 * @param [in] len Key length (in bytes).
 * @return Hash value.
 */
uint32_t compute_hash(const void* key, size_t len)
{
    uint8_t *ptr = (uint8_t*)key;
    uint32_t hash;
    for(hash=0; len; --len)
    {
        hash += *ptr;
        hash  = (hash << 3) + (hash >> 5) + *ptr++;
    }
    return hash;
}

#ifdef __GNUC__
/**
 * Perform a bitwise rotation.
 */
static FORCE_INLINE uint32_t _rotl(uint32_t r, uint8_t by)
{
    __asm __volatile__ 
    (
        "rol %%cl, %%eax"
        : "=a" (r)
        : "a" (r), "c" (by)
    );
    return r;
}
#endif /* __GNUC__ */

#define BIG_CONSTANT(x) (x##LLU)

/**
 * Compute hash value using MurmurHash3 by Austin Appleby.
 * @param [in] key Input key.
 * @param [in] len Key length (in bytes).
 * @return Hash value.
 */
uint32_t murmur3(const void* key, size_t len)
{
    static const uint32_t seed  = 0x00504345;
    static const uint32_t c1    = 0xcc9e2d51;
    static const uint32_t c2    = 0x1b873593;

    const int      nblocks = len / 4;
    const uint8_t *data = (const uint8_t*)key;
    const uint8_t *tail = (const uint8_t*)(data + nblocks*4);
    
    int i;
    uint32_t h1 = seed;
    uint32_t k1 = 0;

    const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);
    for(i = -nblocks; i; i++)
    {
        uint32_t k1 = blocks[i];
        k1 *= c1;
        k1 = _rotl(k1, 15);
        k1 *= c2;
        h1 ^= k1;
        h1 = _rotl(h1,13);
        h1 = h1*5 + 0xe6546b64;
    }

    k1 = 0;
    switch(len & 3)
    {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
        k1 *= c1; k1 = _rotl(k1,15); k1 *= c2; h1 ^= k1;
    };

    h1 ^= len;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;
    return h1;
}
