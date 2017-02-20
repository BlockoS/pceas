#include <string.h>

#include "hash.h"

#define INITIAL_BUCKET_COUNT 64

/** Hash table bucket. **/
struct bucket_t
{
    /** Hash of the key. **/
    uint32_t hash;
    /** Key. **/
    char *key;
    /** Key length. **/
    size_t len;
    /** Offset in bytes in the data buffer of the key and value. **/
    off_t offset;
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
        free(tab);
        return NULL;
    }

    tab->size = size;
    tab->data = (uint8_t*)malloc(tab->capacity * size);
    if(NULL == tab->data)
    {
        free(tab->buckets);
        free(tab);
        return NULL;
    }

    memset(tab->buckets, 0, tab->capacity * sizeof(struct bucket_t));
    memset(tab->data, 0, tab->capacity * size);
    return tab; 
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
    uint32_t i;
    for(i=0; i<tab->capacity; i++)
    {
        uint32_t pos = (hash + i) % tab->capacity;
        int distance = htab_distance(tab, pos, hash);
        if((NULL == tab->buckets[pos].key) || (i > distance))
        {
            return NULL;
        }
        else if((len == tab->buckets[pos].len) && (0 == memcmp(key, tab->buckets[pos].key, len)))
        {
            return tab->data + (tab->buckets[pos].offset * tab->size);
        }
    }
    return NULL;
}
/**
 * Insert bucket into the hash table.
 */
static void htab_insert(struct htab_t* tab, struct bucket_t *bucket)
{
    int probe = 0;
    uint32_t i;
    
    for(i=0; i<tab->capacity; i++, probe++)
    {
        uint32_t pos = (bucket->hash + i) % tab->capacity;
        if(tab->buckets[pos].len)
        {
            int distance = htab_distance(tab, pos, bucket->hash);
            if(probe > distance)
            {
                struct bucket_t tmp = tab->buckets[pos];
                tab->buckets[pos] = *bucket;
                *bucket = tmp;
                probe = distance;
            }
        }
        else
        {
            tab->buckets[pos] = *bucket;
            break;
        }
    }
}
/**
 * Expand hash table.
 */
static int htab_grow(struct htab_t* tab)
{
    struct bucket_t *old;
    struct bucket_t *tmp;
    
    uint32_t new_capacity = tab->capacity * 2;
    uint8_t *data = (uint8_t*)realloc(tab->data, new_capacity * tab->size);
    if(NULL == data)
    {
        // [todo] error value
        return 1;
    }
    tab->data = data;
    
    old = tab->buckets;
    tmp = (struct bucket_t*)malloc(new_capacity * sizeof(struct bucket_t));
    if(NULL == tmp)
    {
        // [todo] error value
        return 1;
    }
    tab->buckets = tmp;
    tab->capacity = new_capacity;
    memset(tab->buckets, 0, tab->capacity * sizeof(struct bucket_t));
    for(uint32_t i=0; i<tab->capacity; i++)
    {
        if(old[i].key)
        {
            htab_insert(tab, &old[i]);
        }
    }
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
    bucket.offset = tab->used;
    
    if(tab->used == tab->capacity)
    {
        if(htab_grow(tab))
        {
            free(bucket.key);
            // [todo] error value
            return 1;
        }
    }
    
    memcpy(tab->data + (bucket.offset*tab->size), element, tab->size);
    
    tab->used++;
    htab_insert(tab, &bucket);
    
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
    // [todo]
    return 1; 
}
