#ifndef PCEAS_DICT_H
#define PCEAS_DICT_H

#include <stdlib.h>

/**
 * Dictionary.
 */
typedef struct _dict_t dict_t;

/**
 * Create dictionary.
 * @param [in] size Element size.
 * @return A pointer to the newly created dictionary or NULL if an error
 *         occured.
 */
dict_t* dict_new(size_t size);

/**
 * Delete dictionary.
 */
void dict_delete(dict_t *dict);

/**
 * Add a new entry to the dictionary.
 * @param [in] dict  Dictionary.
 * @param [in] key   Key.
 * @param [in] value Value.
 * @return 0 if the entry was succesfully added or -1 if an error occured.
 */
int dict_add(dict_t *dict, const char* key, const void *value);

/**
 * Add a new entry to the dictionary and return the associated entry.
 * @param [in] dict  Dictionary.
 * @param [in] key   Key.
 * @param [in] value Value.
 * @return A pointer to the entry or NULL if an error occured.
 */
void* dict_add_ex(dict_t *dict, const char* key, const void *value);

/**
 * Remove entry from the dictionary.
 * @param [in] dict Dictionary.
 * @param [in] key  Entry key.
 */
void dict_remove(dict_t *dict, const char* key);

/**
 * Search for the value associated to the specified key in the dictionary.
 * @param [in] dict Dictionary.
 * @param [in] key  Entry key.
 * @return A pointer to the value associated to the specified key or NULL
 *         if no corresponding entry was found.
 */
void* dict_find(dict_t *dict, const char* key);

#endif /* PCEAS_DICT_H */
