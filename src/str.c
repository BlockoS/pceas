#include "str.h"

#include <string.h>
#include <stdlib.h>

/* Expand string capacity. */
static int string_grow(struct string_t *out, size_t capacity) {
    if(capacity <= out->capacity) {
        return 0;
    }
    char *tmp = (char*)realloc(out->buffer, capacity);
    if(tmp == NULL) {
        return -1;
    }
    out->buffer = tmp;
    out->capacity = capacity;
    return 0;
}
/**
 * Creates a string.
 * @param [out] str String.
 * @param [in] capacity String buffer storage capacity.
 * @return 0 upon success.
 */
int string_create(struct string_t *str, int capacity) {
    str->length = 0;
    str->capacity = 0;
    str->buffer = NULL;
    return string_grow(str, capacity);
}
/**
 * Releases allocated resources.
 * @param [in] str String to be destroyed.
 */
void string_destroy(struct string_t *str) {
    if(str->buffer) {
        free(str->buffer);
        str->buffer = NULL;
    }
    str->length = str->capacity = 0;
}
/**
 * Copies the content of a string into another.
 * @param [out] out Outout string.
 * @param [in] in Input string.
 * @return 0 upon success.
 */
int string_copy(struct string_t *out, struct string_t *in) {
    int ret = string_grow(out, in->capacity);
    if(!ret) { 
        memcpy(out->buffer, in->buffer, in->length);
        out->length = in->length;
    }
    return ret;
}
/**
 * Copies the content of a string into another.
 * @param [out] out Output string.
 * @param [in] in Pointer to the input string.
 * @param [in] len Length of the input string.
 * @return 0 upon success.
 */
int string_copy_unsafe(struct string_t *out, const char *in, size_t len) {
    int ret = string_grow(out, len);
    if(!ret) {
        memcpy(out->buffer, in, len);
        out->length = len;
    }
    return ret;
}
/**
 * Append a single character to the string.
 * @param [in][out] str Output string.
 * @param [in] c Character to append.
 * @return 0 upon success.
 */
int string_push(struct string_t *str, char c) {
    if(str->length == str->capacity) {
        int ret = string_grow(str, str->capacity*2);
        if(ret) {
            return ret;
        }
    }
    str->buffer[str->length++] = c;
    return 0;
}
/**
 * Delete and returns the last char of the string.
 * @param [in][out] str String from which the char will extracted.
 * @return Popped char or '\0' if the string is empty.
 */
char string_pop(struct string_t *str) {
    if(!str->length) {
        return '\0';
    }
    return str->buffer[--str->length];
}
/**
 * Clears string content.
 * @param [in][out] str String.
 */
void string_clear(struct string_t *str) {
    str->length = 0;
}
/**
 * Returns the last character.
 */
char string_last(struct string_t *str) {
    return (str->length) ? str->buffer[str->length-1] : '\0';
}
