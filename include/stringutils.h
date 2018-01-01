#ifndef PCEAS_STRING_UTILS_H
#define PCEAS_STRING_UTILS_H

/**
 * Find the first and last non space character of a string.
 * @param [in] str    Input string.
 * @param [out] start Pointer to the first non space character.
 * @param [out] end   Pointer to the last non space character. 
 */
void strip(const char *str, const char **start, const char **end);

#endif // PCEAS_STRING_UTILS_H