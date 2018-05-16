#ifndef PCEAS_HASH_H
#define PCEAS_HASH_H

#include <stdlib.h>
#include <stdint.h>

/**
 * ELF hash.
 * @param [in] key  Input string.
 * @param [in] len  Length of the input string.
 * @return 32 bits hash of the input key.
 */
uint32_t elf_hash(const char* key, size_t len); 

/**
 * Update current hash.
 * @param [in] c    Input character.
 * @param [in] hash Current hash value.
 * @return Updated hash value.
 */
uint32_t elf_hash_update(char c, uint32_t hash);

#endif /* PCEAS_HASH_H */
