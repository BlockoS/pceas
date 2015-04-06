#ifndef PCEAS_HASH_H
#define PCEAS_HASH_H

#include "config.h"

/**
 * Compute hash value using Shift-Add-XOR.
 * @param [in] key Input key.
 * @param [in] len Key length (in bytes).
 * @return Hash value.
 */
uint32_t compute_hash(const void* key, size_t len);

/**
 * Compute hash value using MurmurHash3 by Austin Appleby.
 * @param [in] key Input key.
 * @param [in] len Key length (in bytes).
 * @return Hash value.
 */
uint32_t murmur3(const void* key, size_t len);

#endif /* PCEAS_HASH_H */
