#ifndef PCEAS_LIST_H
#define PCEAS_LIST_H

#include <stdlib.h>

/**
 * List.
 */
typedef struct _list_t_ list_t;

/**
 * List iterator.
 */
typedef struct _list_iterator_ list_iterator_t;

/**
 * Create a new list.
 * @param [in] size   Element size in bytes.
 * @return A pointer to the newly created list or NULL if an error
 *         occured.
 */
list_t* list_new(size_t size);

/**
 * Delete list.
 */
void list_delete(list_t *list);

/**
 * Empty list.
 */
void list_clear(list_t *list);

/**
 * Create a copy of the specified list.
 * @param [in] src List to copy.
 * @return A pointer to the copy or NULL if an error occured.
 */
list_t* list_copy(list_t *src);

/**
 * Add element to the end of the list.
 * @param [in] list    List.
 * @param [in] element Pointer to the element to be added.
 * @return 0 if the element was succesfuly added or -1 if an error occured.
 */
int list_push_back(list_t *list, void *element);

/**
 * Add element to the begining of the list.
 * @param [in] list    List.
 * @param [in] element Pointer to the element to be added.
 * @return 0 if the element was succesfuly added or -1 if an error occured.
 */
int list_push_front(list_t *list, void *element);

/**
 * Insert a copy of the element in the list before the specified iterator.
 * @param [in] list     List.
 * @param [in] iterator Iterator into the list.
 * @param [in] element  Element to be inserted.
 * @return 0 if the element was succesfully added or -1 if an error occured.
 */
int list_insert(list_t* list, list_iterator_t* iterator, void *element);

/**
 * Remove the first element from the list.
 */
void list_pop_back(list_t *list);

/**
 * Remove the last element from the list.
 */
void list_pop_front(list_t *list);

/**
 * Remove the element at given position.
 * @param [in] list     List.
 * @param [in] iterator Iterator pointing to the element to be removed.
 */
void list_remove(list_t *list, list_iterator_t *iterator);

/**
 * Returns the number of elements in the list.
 */
size_t list_size(list_t* list);

/**
 * Returns an iterator to the first element of the list or NULL if the
 * list is empty.
 */
list_iterator_t* list_front(list_t* list);

/**
 * Returns an iterator to the last element of the list or NULL if the
 * list is empty.
 */
list_iterator_t* list_back(list_t* list);

/**
 * Returns a pointer the next iterator in the list or NULL if there is
 * no more element in the list.
 */
list_iterator_t* list_iterator_next(list_iterator_t *iterator);

/**
 * Returns a pointer the previous iterator in the list or NULL if there is
 * no more element in the list.
 */
list_iterator_t* list_iterator_previous(list_iterator_t *iterator);

/**
 * Returns a pointer to the iterator data.
 */
void* list_iterator_data(list_iterator_t* iterator);

#endif /* PCEAS_LIST_H */
