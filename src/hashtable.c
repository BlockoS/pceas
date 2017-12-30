#include <string.h>

#include "hash.h"
#include "swap.h"
#include "hashtable.h"

#define INITIAL_BUCKET_COUNT 2

/** Hash table bucket. **/
struct bucket_t
{
    /** Hash of the key. **/
    uint32_t hash;
    /** Key. **/
    char *key;
    /** Key length. **/
    size_t len;
};
/**
 * Opaque hash table structure.
 */
struct htab_t
{
    /** Hash table buckets. **/
    struct bucket_t *buckets;
    /** Data buffer. **/
    uint8_t *data;
    /** Capacity (number of allocated entries). **/
    uint32_t capacity;
    /** Number of used entries. **/ 
    uint32_t used;
    /** Size of a single value. **/
    uint32_t size;
    /** Temporary buffer. */
    uint8_t *tmp;
};
/**
 * Compute probing distance.
 */
static int htab_distance(struct htab_t *tab, uint32_t pos, uint32_t hash)
{
    if(NULL == tab->buckets[pos].key)
    {
        return -1;
    }
    uint32_t original = tab->buckets[pos].hash % tab->capacity;
    return (tab->capacity + pos - original) % tab->capacity;
}
/**
 * Releases memory used by a hash table.
 * @param [in] Pointer to the hash table to be destroyed.
 */
void htab_destroy(struct htab_t* tab)
{
    if(tab)
    {
        if(tab->buckets)
        {
            uint32_t i;
            for(i=0; i<tab->capacity; i++) /* this can be smarter... */
            {
                if(tab->buckets[i].key)
                {
                    free(tab->buckets[i].key);
                }
            }
            free(tab->buckets);
        }
        if(tab->data)
        {
            free(tab->data);
        }
        free(tab);
    }
}
/**
 * Creates an empty hash table.
 * @param [in] size Element size (in bytes).
 * @return Pointer to the newly created hash table. 
 */
struct htab_t* htab_create(size_t size)
{
    struct htab_t* tab = (struct htab_t*)malloc(sizeof(struct htab_t));
    if(NULL == tab)
    {
        return NULL;
    }
    memset(tab, 0, sizeof(struct htab_t));
 
    tab->capacity = INITIAL_BUCKET_COUNT;
    tab->used = 0;
    tab->buckets = (struct bucket_t*)malloc(tab->capacity * sizeof(struct bucket_t));
    if(NULL == tab->buckets)
    {
        htab_destroy(tab);
        return NULL;
    }

    tab->size = size;
    tab->data = (uint8_t*)malloc(tab->capacity * size);
    if(NULL == tab->data)
    {
        htab_destroy(tab);
        return NULL;
    }

    memset(tab->buckets, 0, tab->capacity * sizeof(struct bucket_t));
    memset(tab->data, 0, tab->capacity * size);
    
    tab->tmp = (uint8_t*)malloc(size);
    if(NULL == tab->tmp)
    {
        htab_destroy(tab);
        return NULL;
    }
    return tab; 
}
/**
 * Retrieves the index of the bucket associated to a given key.
 * @param [in] tab Hash table.
 * @param [in] hash Entry hash.
 * @param [in] key Entry key.
 * @param [in] len Length of the entry key.
 * @return index of the matching bucket or UINT32_MAX if there is no matching entry.
 */
uint32_t htab_index(struct htab_t* tab, uint32_t hash, const char* key, int len)
{
    uint32_t i;
    for(i=0; i<tab->capacity; i++)
    {
        uint32_t pos = (hash + i) % tab->capacity;
        int distance = htab_distance(tab, pos, hash);
        if((NULL == tab->buckets[pos].key) || (i > distance))
        {
            return UINT32_MAX;
        }
        else if((len == tab->buckets[pos].len) && (0 == memcmp(key, tab->buckets[pos].key, len)))
        {
            return pos;
        }
    }
    return UINT32_MAX;
}
/**
 * Retrieves the entry associated to a given key.
 * @param [in] tab Hash table.
 * @param [in] key Entry key.
 * @param [in] len Length of the entry key.
 * @return Pointer to the entry associated to the input key, or NULL if the key
 *         does not exist in the table.
 */
void* htab_get(struct htab_t* tab, const char* key, size_t len)
{
    uint32_t hash = elf_hash(key, len);
    uint32_t pos = htab_index(tab, hash, key, len);
    if(UINT32_MAX == pos)
    {
        return NULL;
    }
    return tab->data + (pos * tab->size);
}
/**
 * Insert bucket into the hash table.
 */
static void htab_insert(struct htab_t* tab, struct bucket_t *bucket, uint8_t *data)
{
    int probe = 0;
    uint32_t i;
    
    uint32_t pos = bucket->hash % tab->capacity;
    for(i=0; i<tab->capacity; i++, probe++, pos=(pos+1)%tab->capacity)
    {
        if(tab->buckets[pos].key)
        {
            int distance = htab_distance(tab, pos, bucket->hash);
            if(probe > distance)
            {
                struct bucket_t tmp = tab->buckets[pos];
                tab->buckets[pos] = *bucket;
                *bucket = tmp;
                
                swap(tab->data + (pos*tab->size), data, tab->size);
                
                probe = distance;
            }
        }
        else
        {
            tab->buckets[pos] = *bucket;
            memcpy(tab->data + (pos*tab->size), data, tab->size);
            break;
        }
    }
}
/**
 * Expand hash table.
 */
static int htab_grow(struct htab_t* tab)
{
    struct bucket_t *buckets;
    uint8_t *data;
    uint32_t capacity;
    
    uint32_t old_capacity = tab->capacity;
    uint8_t *old_data = tab->data;
    struct bucket_t *old_buckets = tab->buckets;

    capacity = tab->capacity * 2;
    data = (uint8_t*)malloc(capacity * tab->size);
    if(NULL == data)
    {
        // [todo] error value
        return 1;
    }
    
    buckets = (struct bucket_t*)malloc(capacity * sizeof(struct bucket_t));
    if(NULL == buckets)
    {
        // [todo] error value
        free(data);
        return 1;
    }
    memset(buckets, 0, capacity * sizeof(struct bucket_t));

    tab->capacity = capacity;
    tab->buckets = buckets;
    tab->data = data;
    for(uint32_t i=0; i<old_capacity; i++)
    {
        if(old_buckets[i].key)
        {
            htab_insert(tab, &old_buckets[i], &old_data[i*tab->size]);
        }
    }
    free(old_data);
    free(old_buckets);
    return 0;
}
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
int htab_add(struct htab_t* tab, const char* key, size_t len, void* element)
{
    struct bucket_t bucket;
    uint32_t hash = elf_hash(key, len);    
    bucket.key = (char*)malloc(len);
    if(NULL == bucket.key)
    {
        // [todo] error value
        return 1;
    }
    memcpy(bucket.key, key, len);
    bucket.hash   = hash;
    bucket.len    = len;
    
    if(tab->used == tab->capacity)
    {
        if(htab_grow(tab))
        {
            free(bucket.key);
            // [todo] error value
            return 1;
        }
    }
    
    tab->used++;
    memcpy(tab->tmp, element, tab->size);
    htab_insert(tab, &bucket, tab->tmp);
    
    return 0;
}
/**
 * Removes an entry from the hash table.
 * @param [in] tab Hash table.
 * @param [in] key Entry key.
 * @param [in] len Length of the entry key.
 * @return [todo]
 */
int htab_remove(struct htab_t* tab, const char* key, size_t len)
{
    uint32_t i;
    uint32_t hash = elf_hash(key, len);
    uint32_t pos = htab_index(tab, hash, key, len);
    if(UINT32_MAX == pos)
    {
        // [todo] error value
        return 1;
    }
    
    for(i=1; i<tab->capacity; i++)
    {
        struct bucket_t bucket;
        int distance;
        uint32_t next = (pos + 1) % tab->capacity;
        if(NULL == tab->buckets[next].key)
        {
            break;
        }
        distance = htab_distance(tab, next, hash);
        if(0 == distance)
        {
            break;
        }
        
        bucket = tab->buckets[next];
        tab->buckets[next] = tab->buckets[pos];
        tab->buckets[pos]  = bucket;
        
        swap(tab->data + (pos*tab->size), tab->data + (next*tab->size), tab->size);

        pos = next;
    }
    if(tab->buckets[pos].key)
    {
        free(tab->buckets[pos].key);
    }
    memset(tab->buckets + pos, 0, sizeof(struct bucket_t));
    memset(tab->data + (pos*tab->size), 0, tab->size);
    return 0; 
}
/**
 * Returns the number of entries.
 * @param (in] tab Hash table.
 * @return Number of entries in the hash table.
 */
size_t htab_count(struct htab_t* tab)
{
    return tab ? tab->used : 0;
}
/**
 * Clears hash table content.
 * @param [in][out] tab Hash table to be cleared.
 */
void htab_clear(struct htab_t* tab)
{
    uint32_t i;
    for(i=0; i<tab->capacity; i++)
    {
        if(tab->buckets[i].key)
        {
            free(tab->buckets[i].key);
        }
    }
    memset(tab->buckets, 0, sizeof(struct bucket_t));
    tab->used = 0;
}
/**
 * Returns an iterator to the first hash table entry.
 * @param [in] tab Hash table.
 * @return iterator to first hash table entry.
 */
htab_iterator_t htab_begin(struct htab_t* tab)
{
    return htab_next(tab, -1);
}
/**
 * Returns an iterator to the entry following the last hash table entry.
 * @param [in] tab Hash table.
 * @return iterator to the entry following the last hash table entry.
 */
htab_iterator_t htab_end(struct htab_t* tab)
{
    return (htab_iterator_t)tab->capacity;
}
/**
 * Returns the iterator to the next hash table entry.
 * @param [in] tab Hash table.
 * @return iterator to the next hash table entry.
 */
htab_iterator_t htab_next(struct htab_t* tab, htab_iterator_t it)
{
    const htab_iterator_t n = htab_end(tab);
    for(++it; (it<n) && (NULL == tab->buckets[it].key); ++it)
    {}
    return it;
}
/**
 * Returns the key associated to the iterator.
 * @param [in] tab Hash table.
 * @param [in] it Iterator to the current entry.
 * @param [out] key Entry key.
 * @param [out] len Key length;
 * @return 0 if the key was successfully retrieved.
 */
int htab_key(struct htab_t* tab, htab_iterator_t it, const char** key, size_t* len)
{
    if((it<htab_end(tab)) && tab->buckets[it].key)
    {
        *key = tab->buckets[it].key;
        *len = tab->buckets[it].len;
        return 0;
    }
    *key = NULL;
    *len = 0;
    return -1;
}
/**
 * Returns the value associated to the iterator.
 * @param [in] tab Hash table.
 * @param [in] it Iterator to the current entry.
 * @return a pointer to the entry element of NULL if the iterator is past the last element.
 */
void* htab_value(struct htab_t* tab, htab_iterator_t it)
{
    if((it<htab_end(tab)) && tab->buckets[it].key)
    {
        return tab->data + (it * tab->size);
    }
    return NULL;
}
