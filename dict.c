#include <string.h>

#include "dict.h"

#include "list.h"
#include "hash.h"
#include "stdint.h"

#define DICT_BUCKET_COUNT 256
#define DICT_BUCKET_INDEX(hash) ((hash) & 255)

typedef struct _dict_entry_t
{
    struct _dict_entry_t* next;
    /* value and key follows. */
} dict_entry_t;

typedef struct
{
    dict_entry_t* bucket[DICT_BUCKET_COUNT];
    size_t        element_size;
    size_t        element_count;
} dict_impl_t;

#define _dict_impl(ptr) ((dict_impl_t*)(ptr))
#define _dict_ptr(impl) ((dict_t*)(impl))
#define _dict_entry_value(impl, entry) ((uint8_t*)((entry)+1))
#define _dict_entry_key(impl, entry)   ((char*)((uint8_t*)((entry)+1) + impl->element_size))

/**
 * Create dictionary.
 * @param [in] size Element size.
 * @return A pointer to the newly created dictionary or NULL if an error
 *         occured.
 */
dict_t* dict_new(size_t size)
{
    dict_impl_t *impl = (dict_impl_t*)malloc(sizeof(dict_impl_t));
    if(NULL == impl)
    {
        return NULL;
    }
    impl->element_size  = size;
    impl->element_count = 0;
    memset(impl->bucket, 0, DICT_BUCKET_COUNT * sizeof(dict_entry_t*));
    return _dict_ptr(impl);
}

/**
 * Delete dictionary.
 */
void dict_delete(dict_t *dict)
{
    dict_impl_t *impl = _dict_impl(dict);
    int i;
    for(i=0; i<DICT_BUCKET_COUNT; i++)
    {
        while(NULL != impl->bucket[i])
        {
            dict_entry_t *tmp = impl->bucket[i];
            impl->bucket[i] = tmp->next;
            free(tmp);
        }
    }
    free(impl);
}

/* Create and add entry to dictionary.
 * Returns NULL if the entry is already present. */
static dict_entry_t* dict_create_entry(dict_impl_t *impl, const char *key, const void *value)
{
    size_t key_len = strlen(key) + 1;
    size_t entry_size   = sizeof(dict_entry_t) + impl->element_size + key_len;
    dict_entry_t* entry = (dict_entry_t*)malloc(entry_size);
    if(NULL == entry)
    {
        return NULL;
    }
    
    entry->next = NULL;
    memcpy(_dict_entry_value(impl, entry), value, impl->element_size);
    memcpy(_dict_entry_key(impl, entry), key, key_len);
    
    return entry;
}

/* Find the entry associated to the given key in the bucket identified by the specified index. */
static dict_entry_t* dict_find_internal(dict_impl_t *impl, int index, const char *key)
{
    dict_entry_t *entry = impl->bucket[index];
    for(; NULL!=entry; entry=entry->next)
    {
        if(0 == strcmp(key, _dict_entry_key(impl, entry)))
        {
            break;
        }
    }
    return entry;
}
/**
 * Add a new entry to the dictionary.
 * @param [in] dict  Dictionary.
 * @param [in] key   Key.
 * @param [in] value Value.
 * @return 0 if the entry was succesfully added or -1 if an error occured.
 */
int dict_add(dict_t *dict, const char* key, const void *value)
{
    size_t key_len = strlen(key);
    unsigned int hash  = compute_hash(key, key_len);
    unsigned int index = DICT_BUCKET_INDEX(hash);
    dict_impl_t  *impl  = _dict_impl(dict);
    dict_entry_t *entry = dict_find_internal(impl, index, key);
    if(NULL != entry)
    {
        return -1;
    }
    
    entry = dict_create_entry(impl, key, value);
    if(NULL == entry)
    {
        return -1;
    }
    
    entry->next = impl->bucket[index];
    impl->bucket[index] = entry;
    ++impl->element_count;
    return 0;
}

/**
 * Add a new entry to the dictionary and return the associated entry.
 * @param [in] dict  Dictionary.
 * @param [in] key   Key.
 * @param [in] value Value.
 * @return A pointer to the entry or NULL if an error occured.
 */
void* dict_add_ex(dict_t *dict, const char* key, const void *value)
{
    size_t key_len = strlen(key);
    unsigned int hash  = compute_hash(key, key_len);
    unsigned int index = DICT_BUCKET_INDEX(hash);
    dict_impl_t  *impl  = _dict_impl(dict);
    dict_entry_t *entry = dict_find_internal(impl, index, key);
    if(NULL == entry)
    {
        entry = dict_create_entry(impl, key, value);
        if(NULL == entry)
        {
            return NULL;
        }
        entry->next = impl->bucket[index];
        impl->bucket[index] = entry;
        ++impl->element_count;
    }
    return _dict_entry_value(impl, entry);
}

/**
 * Remove entry from the dictionary.
 * @param [in] dict Dictionary.
 * @param [in] key  Entry key.
 */
void dict_remove(dict_t *dict, const char* key)
{
    unsigned int hash  = compute_hash(key, strlen(key));
    unsigned int index = DICT_BUCKET_INDEX(hash);
    dict_impl_t  *impl = _dict_impl(dict);
    dict_entry_t *entry, *previous;

    for(previous=NULL, entry=impl->bucket[index]; NULL!=entry; previous=entry, entry=entry->next)
    {
        if(0 == strcmp(_dict_entry_key(impl, entry), key))
        {
            break;
        }
    }
    /* No matrching entry found */
    if(NULL == entry)
    {
        return;
    }
    if(NULL == previous)
    {
        /* We are removing the first bucket element */
        impl->bucket[index] = entry->next;
    }
    else
    {
        previous->next = entry->next;
    }
    free(entry);
    --impl->element_count;
}

/**
 * Search for the value associated to the specified key in the dictionary.
 * @param [in] dict Dictionary.
 * @param [in] key  Entry key.
 * @return A pointer to the value associated to the specified key or NULL
 *         if no corresponding entry was found.
 */
void* dict_find(dict_t *dict, const char* key)
{
    unsigned int hash  = compute_hash(key, strlen(key));
    unsigned int index = DICT_BUCKET_INDEX(hash);
    dict_impl_t  *impl = _dict_impl(dict);
    dict_entry_t *entry = dict_find_internal(impl, index, key);
    if(NULL == entry)
    {
        return NULL;
    }
    return _dict_entry_value(impl, entry);
}
