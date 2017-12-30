#ifndef PCEAS_HASH_TABLE_H
#define PCEAS_HASH_TABLE_H

#include "hash.h"

/**
 * Opaque hash table structure.
 */
struct htab_t;
/**
 * Hash table iterator.
 */
typedef int64_t htab_iterator_t;
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
/**
 * Returns the number of entries.
 * @param (in] tab Hash table.
 * @return Number of entries in the hash table.
 */
size_t htab_count(struct htab_t* tab);
/**
 * Clears hash table content.
 * @param [in][out] tab Hash table to be cleared.
 */
void htab_clear(struct htab_t* tab);
/**
 * Returns an iterator to the first hash table entry.
 * @param [in] tab Hash table.
 * @return iterator to first hash table entry.
 */
htab_iterator_t htab_begin(struct htab_t* tab);
/**
 * Returns an iterator to the entry following the last hash table entry.
 * @param [in] tab Hash table.
 * @return iterator to the entry following the last hash table entry.
 */
htab_iterator_t htab_end(struct htab_t* tab);
/**
 * Returns the iterator to the next hash table entry.
 * @param [in] tab Hash table.
 * @param [in] it Iterator to the current entry.
 * @return iterator to the next hash table entry.
 */
htab_iterator_t htab_next(struct htab_t* tab, htab_iterator_t it);
/**
 * Returns the key associated to the iterator.
 * @param [in] tab Hash table.
 * @param [in] it Iterator to the current entry.
 * @param [out] key Entry key.
 * @param [out] len Key length;
 * @return 0 if the key was successfully retrieved.
 */
 int htab_key(struct htab_t* tab, htab_iterator_t it, const char** key, size_t* len);
/**
 * Returns the value associated to the iterator.
 * @param [in] tab Hash table.
 * @param [in] it Iterator to the current entry.
 * @return a pointer to the entry element of NULL if the iterator is past the last element.
 */
void* htab_value(struct htab_t* tab, htab_iterator_t it);
 
#endif /* PCEAS_HASH_TABLE_H */
