#include "hash.h"

/**
 * ELF hash.
 * @param [in] key  Input string.
 * @param [in] len  Length of the input string.
 * @return 32 bits hash of the input key.
 */
uint32_t elf_hash(const char* key, size_t len) {
    const uint8_t *in = (const uint8_t*)key;
    
    size_t  i;
    uint32_t h;
    
    for(i=0, h=0; i<len; i++) {
        uint32_t g;
        h = (h << 4) + in[i];
        g = h & 0xf0000000;
        if(g) {
            h ^= g >> 24;
        }
        h &= ~g;
    }
    return h;
} 

/**
 * Update current hash.
 * @param [in] c    Input character.
 * @param [in] hash Current hash value.
 * @return Updated hash value.
 */
uint32_t elf_hash_update(char c, uint32_t hash) {
    uint32_t g;
    hash = (hash << 4) + c;
    g = hash & 0xf0000000;
    if(g) {
        hash ^= g >> 24;
    }
    return hash & ~g;
}
