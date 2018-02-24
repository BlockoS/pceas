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
 * Jump to the next space character.
 * @param [in] start Pointer to the first character of the string.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer to the first space character,
 *         or end if no space was found.
 */
const char* goto_space(const char *start, const char *end);

/**
 * Jump to next line.
 * @param [in] start Pointer to the first character of the string.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer to the first character of the next line,
           or end if there is no new line.
 */
const char* next_line(const char *start, const char *end);

/**
 * Skip spaces.
 * @param [in] start Pointer to the first character of the string.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer to the first non whitespace character.
 */
const char* skip_spaces(const char *start, const char *end);

/**
 * Skip quoted string.
 * @param [in] start Pointer to the string quote character.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer past the next string quote or end if the terminating quote is missing.
 */
const char* skip_quoted_string(const char *start, const char *end);

#endif // PCEAS_STRING_UTILS_H