#ifndef PCEAS_ARRAY_H
#define PCEAS_ARRAY_H

#include <stdlib.h>

/**
 * Array.
 */
typedef struct _array_t_ array_t;

/**
 * Create a new array of nmemb elements of data, each of size bytes long.
 * @param [in] size   Element size in bytes.
 * @param [in] nmemb  Number of elements.
 * @return A pointer to the newly created array or NULL if an error
 *         occured.
 */
array_t* array_new(size_t size, size_t nmemb);

/**
 * Delete array.
 */
void array_delete(array_t *array);

/**
 * Create a copy of the specified array.
 * @param [in] src Array to copy.
 * @return A pointer to the copy or NULL if an error occured.
 */
array_t* array_copy(array_t *src);

/**
 * Resize the array storage to the specified number of elements.
 * @param [in] array Array.
 * @param [in] count Number of elements the array should contain.
 * @return A pointer to the expanded array or NULL if an error occured.
 */
array_t* array_resize(array_t *array, size_t count);

/**
 */
array_t* array_push_back(array_t *array, void* element);

/**
 */
array_t* array_push_front(array_t *array, void* element);

/**
 */
array_t *array_insert(array_t *array, size_t index, void* element);

/**
 * Returns a pointer to the first element of the array.
 */
void* array_front(array_t *array);

/**
 * Returns a pointer to the last element of the array.
 */
void* array_back(array_t *array);

/**
 * Remove the first element of the array.
 * This can be seen as an alias to array_remove(array, 0);
 * @return 0 if the operation succeeded.
 *         1 if the array is already empty.
 */
int array_pop_front(array_t *array);

/**
 * Remove the last element of the array.
 * This can be seen as an alias to array_remove(array, array_size(array)-1);
 * @return 0 if the operation succeeded.
 *         1 if the array is already empty.
 */
int array_pop_back(array_t *array);

/**
 * Remove the element at the specified index.
 * @param [in] array Array.
 * @param [in] index Index of the element to remove.
 * @return 0 if the operation succeeded.
 *         1 if the array is already empty, or if the index is out of range.
 */
int array_remove(array_t *array, size_t index);

/**
 * 
 */
void array_clear(array_t *array);

/**
 * Returns the number of elements stored in the array.
 * @param [in] array Array.
 */
size_t array_size(array_t* array);

/**
 * Returns the total number of elements that can be stored in the array
 * before needing to allocate more memory.
 */
size_t array_capacity(array_t* array);

/**
 * Returns a pointer to the array buffer.
 */
void* array_data(array_t* array);

/**
 * Returns a pointer to the element at the specified index.
 * @param [in] array Array.
 * @param [in] index Element index.
 * @return A pointer to the element at the specified index.
 *         NULL if the index is out of array range.
 */
void* array_at(array_t* array, size_t index);


#endif /* PCEAS_ARRAY_H */
