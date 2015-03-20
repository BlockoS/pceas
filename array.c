#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "array.h"

typedef struct
{
    size_t  capacity;
    size_t  element_count;
    size_t  element_size;
} array_impl_t;

#define _array_impl(array) ((array_impl_t*)(array))
#define _array_ptr(impl)   ((array_t*)(impl))
#define _array_data(impl, index) (((uint8_t*)((impl)+1)) + ((index) * (impl)->element_size))

/**
 * Create a new array of nmemb elements of data, each of size bytes long.
 * @param [in] size   Element size in bytes.
 * @param [in] nmemb  Number of elements.
 * @return A pointer to the newly created array or NULL if an error
 *         occured.
 */
array_t* array_new(size_t size, size_t nmemb)
{
    size_t capacity  = (nmemb ? nmemb : 1);
    size_t data_size = size * capacity;
    array_impl_t *impl = NULL;
    if(0 == data_size)
    {
        return NULL;
    }
    impl = (array_impl_t*)malloc(sizeof(array_impl_t) + data_size);
    if(NULL == impl)
    {
        return NULL;
    }
    impl->capacity      = capacity,
    impl->element_count = nmemb;
    impl->element_size  = size;
    return _array_ptr(impl);
}

/**
 * Delete array.
 */
void array_delete(array_t *array)
{
    free(_array_impl(array));
}

/**
 * Create a copy of the specified array.
 * @param [in] array Array to copy.
 * @return A pointer to the copy or NULL if an error occured.
 */
array_t* array_copy(array_t *src)
{
    array_impl_t *src_impl = _array_impl(src);
    array_t      *dst      = array_new(src_impl->element_count, src_impl->element_size);
    array_impl_t *dst_impl = _array_impl(dst);
    if(NULL != dst)
    {
        memcpy(_array_data(dst_impl,0), _array_data(src_impl,0), src_impl->element_count * src_impl->element_size);
    }
    return dst;
}

/**
 * Resize the array storage to the specified number of elements.
 * @param [in] array Array.
 * @param [in] count Number of elements the array should contain.
 * @return A pointer to the expanded array or NULL if an error occured.
 */
array_t* array_resize(array_t *array, size_t count)
{
    array_impl_t *impl = _array_impl(array);
    if(count <= impl->capacity)
    {
        impl->element_count = count;
        return array;
    }
    else
    {
        impl = (array_impl_t*)realloc(impl, sizeof(array_impl_t) + (count * impl->element_size));
        if(NULL == impl)
        {
            return NULL;
        }
        impl->capacity      = count;
        impl->element_count = count;
        return _array_ptr(impl);
    }
}

/* When adding a new element, check if the array storage needs to be expanded. */
static array_impl_t* array_expand_if_needed(array_impl_t* impl)
{
    if((impl->element_count+1) <= impl->capacity)
    {
        return impl;
    }
    else
    {
        size_t new_capacity = impl->capacity * 2;
        impl = (array_impl_t*)realloc(impl, sizeof(array_impl_t) + (new_capacity * impl->element_size));
        if(NULL == impl)
        {
            return NULL;
        }
        impl->capacity = new_capacity;
        return impl;
    }
}

/**
 * Add element to the end of the array.
 * @param [in] array   Array.
 * @param [in] element Pointer to the element to be added.
 * @return A pointer to the expanded array (@note the pointer might have
 *         changed) or NULL if an error occured.
 */
array_t* array_push_back(array_t *array, void* element)
{
    array_impl_t *impl = array_expand_if_needed(_array_impl(array));
    if(NULL == impl)
    {
        return NULL;
    }
    else
    {
        uint8_t *data = _array_data(impl, impl->element_count);
        memcpy(data, element, impl->element_size);
        ++impl->element_count;
        return _array_ptr(impl);
    }
}

/**
 * Add element to the beginning of the array.
 * @param [in] array   Array.
 * @param [in] element Pointer to the element to be added.
 * @return A pointer to the expanded array (@note the pointer might have
 *         changed) or NULL if an error occured.
 */
array_t* array_push_front(array_t *array, void* element)
{
    return array_insert(array, 0, element);
}

/**
 * Insert element at the specified index.
 * @param [in] array   Array.
 * @param [in] index   Index where the element will be copied.
 * @param [in] element Element to be inserted.
 * @return A pointer to the expanded array (@note the pointer might have
 *         changed) or NULL if an error occured.
 */
array_t* array_insert(array_t *array, size_t index, void* element)
{
    array_impl_t *impl = _array_impl(array);
    if(index > impl->element_count)
    {
        return NULL;
    }
    impl = array_expand_if_needed(impl);
    if(NULL == impl)
    {
        return NULL;
    }
    if(index < impl->element_count)
    {
        memmove(_array_data(impl, index+1), _array_data(impl, index), (impl->element_count-index)*impl->element_size);
    }
    memcpy(_array_data(impl, index), element, impl->element_size);
    ++impl->element_count;
    return _array_ptr(impl);
}

/**
 * Store a copy of the element at the specified index.
 * @param [in] array   Array.
 * @param [in] index   Index where the element will be copied.
 * @param [in] element Pointer to the element to be inserted.
 * @return 1 if the element was succesfully copied.
 *         0 if the index is out of array range.
 */
int array_store_at(array_t *array, size_t index, void* element)
{
    array_impl_t *impl = _array_impl(array);
    if(index >= impl->element_count)
    {
        return 0;
    }
    memcpy(_array_data(impl, index), element, impl->element_size);
    return 1;
}

/**
 * Returns a pointer to the element at the specified index.
 * @param [in] array Array.
 * @param [in] index Element index.
 * @return A pointer to the element at the specified index.
 *         NULL if the index is out of array range.
 */
void* array_at(array_t* array, size_t index)
{
    array_impl_t *impl = _array_impl(array);
    if(index >= impl->element_count)
    {
        return NULL;
    }
    return _array_data(impl, index);
}

/**
 * Returns a pointer to the first element of the array.
 */
void* array_front(array_t *array)
{
    return array_at(array, 0);
}

/**
 * Returns a pointer to the last element of the array.
 */
void* array_back(array_t *array)
{
    array_impl_t *impl = _array_impl(array);
    if(0 == impl->element_count)
    {
        return NULL;
    }
    return _array_data(impl, impl->element_count-1);
}

/**
 * Remove the first element of the array.
 * This can be seen as an alias to array_remove(array, 0);
 */
void array_pop_front(array_t *array)
{
    array_remove(array, 0);
}

/**
 * Remove the last element of the array.
 * This can be seen as an alias to array_remove(array, array_size(array)-1);
 */
void array_pop_back(array_t *array)
{
    array_impl_t *impl = _array_impl(array);
    if(impl->element_count)
    {
        --impl->element_count;
    }
}

/**
 * Remove the element at the specified index.
 * @param [in] array Array.
 * @param [in] index Index of the element to remove.
 */
void array_remove(array_t *array, size_t index)
{
    array_impl_t *impl = _array_impl(array);
    if(index >= impl->element_count)
    {
        return;
    }
    if(index < (impl->element_count-1))
    {
        memmove(_array_data(impl, index), _array_data(impl, index+1), (impl->element_count-index-1)*impl->element_size);
    }
    --impl->element_count;
}

/**
 * Empty array.
 */
void array_clear(array_t *array)
{
    _array_impl(array)->element_count = 0;
}

/**
 * Returns the number of elements stored in the array.
 * @param [in] array Array.
 */
size_t array_size(array_t* array)
{
    return (_array_impl(array)->element_count);
}

/**
 * Returns the total number of elements that can be stored in the array
 * before needing to allocate more memory.
 */
size_t array_capacity(array_t* array)
{
    return (_array_impl(array)->capacity);
}

/**
 * Returns a pointer to the array buffer.
 * @param [in] array Array.
 */
void* array_data(array_t* array)
{
    return _array_data(_array_impl(array), 0);
}
