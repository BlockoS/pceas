#ifndef PCEAS_HASH_H
#define PCEAS_HASH_H

#include <stdlib.h>

/**
 * Compute hash value using Shift-Add-XOR.
 * @param [in] key Input key.
 * @param [in] len Key length (in bytes).
 * @return Hash value.
 */
unsigned int compute_hash(const void* key, size_t len);

#endif /* PCEAS_HASH_H */
