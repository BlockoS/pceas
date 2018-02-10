#ifndef PCEAS_STRING_UTILS_H
#define PCEAS_STRING_UTILS_H

/**
 * Find the first and last non space character of a string.
 * @param [in] str    Input string.
 * @param [out] start Pointer to the first non space character.
 * @param [out] end   Pointer to the last non space character. 
 */
void strip(const char *str, const char **start, const char **end);

/**
 * Jump to the end of line.
 * A line ends with '\r', '\n' or the beginning of a comment ';'. 
 * @param [in] start Pointer to the first character of the string.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer to the first character of the end of line sequence,
 *         or end if no end of line sequence was found.
 */
const char* goto_eol(const char *start, const char *end);

/**
 * Jump to next line.
 * @param [in] start Pointer to the first character of the string.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer to the first character of the next line,
           or end if there is no new line.
 */
const char* next_line(const char *start, const char *end);

#endif // PCEAS_STRING_UTILS_H