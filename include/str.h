#ifndef PCEAS_STR_H
#define PCEAS_STR_H

#include <stddef.h>

/**
 * ASCII string. 
 */
struct string_t {
    char   *buffer;
    size_t length;
    size_t capacity;
};

/**
 * Creates a string.
 * @param [out] str String.
 * @param [in] capacity String buffer storage capacity.
 * @return 0 upon success.
 */
int string_create(struct string_t *str, int capacity);
/**
 * Releases allocated resources.
 * @param [in] str String to be destroyed.
 */
void string_destroy(struct string_t *str);
/**
 * Copies the content of a string into another.
 * @param [out] out Outout string.
 * @param [in] in Input string.
 * @return 0 upon success.
 */
int string_copy(struct string_t *out, struct string_t *in);
/**
 * Copies the content of a string into another.
 * @param [out] out Output string.
 * @param [in] in Pointer to the input string.
 * @param [in] len Length of the input string.
 * @return 0 upon success.
 */
int string_copy_unsafe(struct string_t *out, const char *in, size_t len);
/**
 * Append a single character to the string.
 * @param [in][out] str Output string.
 * @param [in] c Character to append.
 * @return 0 upon success.
 */
int string_push(struct string_t *str, char c);
/**
 * Delete and returns the last char of the string.
 * @param [in][out] str String from which the char will extracted.
 * @return Popped char or '\0' if the string is empty.
 */
char string_pop(struct string_t *str);
/**
 * Clears string content.
 * @param [in][out] str String.
 */
void string_clear(struct string_t *str);
/**
 * Returns the last character.
 */
char string_last(struct string_t *str);

#endif // PCEAS_STR_H
