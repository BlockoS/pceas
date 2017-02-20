#ifndef PCEAS_HASH_TABLE_H
#define PCEAS_HASH_TABLE_H

#include "hash.h"

/**
 * Opaque hash table structure.
 */
struct htab_t;

/**
 * Creates an empty hash table.
 * @param [in] size Element size (in bytes).
 * @return Pointer to the newly created hash table. 
 */
struct htab_t* htab_create(size_t size);
/**
 * Releases memory used by a hash table.
 * @param [in] Pointer to the hash table to be destroyed.
 */
void htab_destroy(struct htab_t* tab);
/**
 * Retrieves the entry associated to a given key.
 * @param [in] tab Hash table.
 * @param [in] key Entry key.
 * @param [in] len Length of the entry key.
 * @return Pointer to the entry associated to the input key, or NULL if the key
 *         does not exist in the table.
 */
void* htab_get(struct htab_t* tab, const char* key, size_t len);
/**
 * Adds a new entry to the hash table.
 * @param [in] tab Hash table.
 * @param [in] key Entry key.
 * @param [in] len Length of the entry key.
 * @param [in] element Pointer to the entry data. Its size must match the one
 *                     passed to htab_create.
 * @return 0 if the entry was succesfully created.
 * @todo error value (already exists, allocation error).
 */
int htab_add(struct htab_t* tab, const char* key, size_t len, void* element);
/**
 * Removes an entry from the hash table.
 * @param [in] tab Hash table.
 * @param [in] key Entry key.
 * @param [in] len Length of the entry key.
 * @return [todo]
 */
int htab_remove(struct htab_t* tab, const char* key, size_t len);
 
#endif /* PCEAS_HASH_TABLE_H */
