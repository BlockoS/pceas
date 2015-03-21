#include <stdint.h>
#include <string.h>

#include "list.h"

typedef struct _list_iterator_impl_t
{
    struct _list_iterator_impl_t *previous;
    struct _list_iterator_impl_t *next;
} list_iterator_impl_t;

#define _list_iterator_impl(iterator) ((list_iterator_impl_t*)(iterator))
#define _list_iterator_ptr(impl)      ((list_iterator_t*)(impl))
#define _list_iterator_data(iterator) ((uint8_t*)((iterator)+1))

typedef struct
{
    list_iterator_impl_t *head;
    list_iterator_impl_t *tail;
} list_simple_t;

typedef struct
{
    size_t element_size;
    size_t element_count;
    list_simple_t elements;
    list_simple_t unused;
} list_impl_t;

#define _list_impl(list) ((list_impl_t*)(list))
#define _list_ptr(impl)  ((list_t*)(impl))

/**
 * Create a new list.
 * @param [in] size   Element size in bytes.
 * @return A pointer to the newly created list or NULL if an error
 *         occured.
 */
list_t* list_new(size_t size)
{
    list_impl_t *impl = (list_impl_t*)malloc(sizeof(list_impl_t));
    if(NULL == impl)
    {
        return NULL;
    }
    impl->element_size   = size;
    impl->element_count  = 0;
    impl->elements.head = NULL;
    impl->elements.tail = NULL;
    impl->unused.head   = NULL;
    impl->unused.tail   = NULL;
    return _list_ptr(impl);
}

/**
 * Delete list.
 */
void list_delete(list_t *list)
{
    list_impl_t *impl = _list_impl(list);
    list_iterator_impl_t *iterator;
    for(iterator=impl->elements.head; NULL!=iterator; )
    {
        list_iterator_impl_t *tmp = iterator;
        iterator = iterator->next;
        free(tmp);
    }
    for(iterator=impl->unused.head; NULL!=iterator; )
    {
        list_iterator_impl_t *tmp = iterator;
        iterator = iterator->next;
        free(tmp);
    }
    free(list);
}

/**
 * Empty list.
 */
void list_clear(list_t *list)
{
    list_impl_t *impl = _list_impl(list);
    if(NULL != impl->unused.tail)
    {
        impl->unused.tail->next = impl->elements.head;
    }
    if(NULL != impl->elements.head)
    {
        impl->elements.head->previous = impl->unused.tail;
    }
    impl->unused.tail = impl->elements.tail;
    if(NULL == impl->unused.head)
    {
        impl->unused.head = impl->elements.head;
    }
    impl->elements.head = impl->elements.tail = NULL;
}

/**
 * Create a copy of the specified list.
 * @param [in] src List to copy.
 * @return A pointer to the copy or NULL if an error occured.
 */
list_t* list_copy(list_t *src)
{
    list_t *dest = list_new(_list_impl(src)->element_size);
    list_iterator_impl_t* iterator;
    int err = 0;
    for(iterator=_list_impl(src)->elements.head; (NULL!=iterator) && (!err); iterator=iterator->next)
    {
       err = list_push_back(dest, _list_iterator_data(iterator));
    }
    if(err)
    {
        list_delete(dest);
        return NULL;
    }
    return dest;
}

/* Allocate a single list element. */
static list_iterator_impl_t* list_alloc(list_impl_t *impl, void *element)
{
    list_iterator_impl_t *iterator;
    /* Try to recycle list element. */
    if(NULL != impl->unused.head)
    {
        iterator = impl->unused.head;
        impl->unused.head = impl->unused.head->next;
        if(NULL != impl->unused.head)
        {
            impl->unused.head->previous = NULL;
        }
        if(iterator == impl->unused.tail)
        {
            impl->unused.tail = NULL;
        }
    }
    else
    {
        iterator = (list_iterator_impl_t*)malloc(sizeof(list_iterator_impl_t) + impl->element_size);
    }
    iterator->next = iterator->previous = NULL;
    if(NULL != element)
    {
        memcpy(_list_iterator_data(iterator), element, impl->element_size);
    }
    else
    {
        memset(_list_iterator_data(iterator), 0, impl->element_size);
    }
    return iterator;
}

/**
 * Add element to the end of the list.
 * @param [in] list    List.
 * @param [in] element Pointer to the element to be added.
 * @return 0 if the element was succesfuly added or -1 if an error occured.
 */
int list_push_back(list_t *list, void *element)
{
    list_impl_t *impl = _list_impl(list);
    list_iterator_impl_t *iterator = list_alloc(impl, element);
    if(NULL == iterator)
    {
        return -1;
    }
    
    iterator->previous = impl->elements.tail;
    if(NULL != impl->elements.tail)
    {
        impl->elements.tail->next = iterator;
    }
    if(NULL == impl->elements.head)
    {
        impl->elements.head = iterator;
    }
    impl->elements.tail = iterator;
    ++impl->element_count;
    return 0;
}

/**
 * Add element to the begining of the list.
 * @param [in] list    List.
 * @param [in] element Pointer to the element to be added.
 * @return 0 if the element was succesfuly added or -1 if an error occured.
 */
int list_push_front(list_t *list, void *element)
{
    list_impl_t *impl = _list_impl(list);
    list_iterator_impl_t *iterator = list_alloc(impl, element);
    if(NULL == iterator)
    {
        return -1;
    }
    
    iterator->next = impl->elements.head;
    if(NULL != impl->elements.head)
    {
        impl->elements.head->previous = iterator;
    }
    if(NULL == impl->elements.tail)
    {
        impl->elements.tail = iterator;
    }
    impl->elements.head = iterator;
    ++impl->element_count;
    return 0;
}

/**
 * Insert a copy of the element in the list before the specified iterator.
 * @param [in] list     List.
 * @param [in] iterator Iterator into the list.
 * @param [in] element  Element to be inserted.
 * @return 0 if the element was succesfully added or -1 if an error occured.
 */
int list_insert(list_t* list, list_iterator_t* iterator, void *element)
{
    list_impl_t *impl = _list_impl(list);
    list_iterator_impl_t *current    = _list_iterator_impl(iterator);
    list_iterator_impl_t *newElement;
    
    if(NULL == iterator)
    {
        return -1;
    }
    
    newElement = list_alloc(impl, element);
    if(NULL == newElement)
    {
        return -1;
    }
    
    newElement->next     = current;
    newElement->previous = current->previous;
    if(NULL != current->previous)
    {
        current->previous->next = newElement;
    }
    current->previous    = newElement;
    
    if(current == impl->elements.head)
    {
        impl->elements.head = newElement;
    }
    return 0;
}

/* Remove item from the list. */
static void list_remove_internal(list_impl_t *impl, list_iterator_impl_t *iterator)
{
    list_iterator_impl_t *previous = iterator->previous;
    list_iterator_impl_t *next     = iterator->next;
    
    iterator->next     = NULL;
    iterator->previous = impl->unused.tail;
    if(NULL != impl->unused.tail)
    {
        impl->unused.tail->next = iterator;
    }
    if(NULL == impl->unused.head)
    {
        impl->unused.head = iterator;
    }
    impl->unused.tail = iterator;
    
    if(NULL != previous)
    {
        previous->next = next;
    }
    if(NULL != next)
    {
        next->previous = previous;
    }
    if(iterator == impl->elements.head)
    {
        impl->elements.head = next;
    }
    if(iterator == impl->elements.tail)
    {
        impl->elements.tail = previous;
    }
    --impl->element_count;
}

/**
 * Remove the first element from the list.
 */
void list_pop_back(list_t *list)
{
    list_impl_t *impl = _list_impl(list);
    list_remove_internal(impl, impl->elements.tail);
}

/**
 * Remove the last element from the list.
 */
void list_pop_front(list_t *list)
{
    list_impl_t *impl = _list_impl(list);
    list_remove_internal(impl, impl->elements.head);
}

/**
 * Remove the element at given position.
 * @param [in] list     List.
 * @param [in] iterator Iterator pointing to the element to be removed.
 */
void list_remove(list_t *list, list_iterator_t *iterator)
{
    list_remove_internal(_list_impl(list), _list_iterator_impl(iterator));
}

/**
 * Returns the number of elements in the list.
 */
size_t list_size(list_t* list)
{
    return _list_impl(list)->element_count;
}

/**
 * Returns an iterator to the first element of the list or NULL if the
 * list is empty.
 */
list_iterator_t* list_front(list_t* list)
{
    return _list_iterator_ptr(_list_impl(list)->elements.head);
}

/**
 * Returns an iterator to the last element of the list or NULL if the
 * list is empty.
 */
list_iterator_t* list_back(list_t* list)
{
    return _list_iterator_ptr(_list_impl(list)->elements.tail);
}

/**
 * Returns a pointer the next iterator in the list or NULL if there is
 * no more element in the list.
 */
list_iterator_t* list_iterator_next(list_iterator_t *iterator)
{
    if(NULL == iterator)
    {
        return NULL;
    }
    return _list_iterator_ptr(_list_iterator_impl(iterator)->next);
}

/**
 * Returns a pointer the previous iterator in the list or NULL if there is
 * no more element in the list.
 */
list_iterator_t* list_iterator_previous(list_iterator_t *iterator)
{
    if(NULL == iterator)
    {
        return NULL;
    }
    return _list_iterator_ptr(_list_iterator_impl(iterator)->previous);
}

/**
 * Returns a pointer to the iterator data.
 */
void* list_iterator_data(list_iterator_t* iterator)
{
    if(NULL == iterator)
    {
        return NULL;
    }
    return _list_iterator_data(_list_iterator_impl(iterator));
}
