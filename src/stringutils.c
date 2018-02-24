#include "stringutils.h"
#include <string.h>
#include <ctype.h>

/**
 * Find the first and last non space character of a string.
 * @param [in] str    Input string.
 * @param [out] start Pointer to the first non space character.
 * @param [out] end   Pointer to the last non space character. 
 */
void strip(const char *str, const char **start, const char **end)
{
    for(*start=str; **start && isspace(**start); ++*start) {}
    for(*end=str+strlen(str)-1; (*end > *start)  && isspace(**end); --*end) {}
    if(*end < *start) { *end = *start; }
}

/**
 * Jump to the end of line.
 * @param [in] start Pointer to the first character of the string.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer to the first character of the end of line sequence,
 *         or end if no end of line sequence was found.
 */
const char* goto_eol(const char *start, const char *end) {
    while(   (start < end) 
          && (start[0] != '\r')
          && (start[0] != '\n')
          && (start[0] != ';' ) ) {
        ++start;
    }
    return start;
}

/**
 * Jump to the next space character.
 * @param [in] start Pointer to the first character of the string.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer to the first space character,
 *         or end if no space was found.
 */
const char* goto_space(const char *start, const char *end) {
    for( ; (start < end) && !isspace(*start); start++) {
    }
    return start;
}

/**
 * Jump to next line.
 * @param [in] start Pointer to the first character of the string.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer to the first character of the next line,
 *         or end if there is no new line.
 */
const char* next_line(const char *start, const char *end) {
    for(; start < end; ++start) { 
        if(start[0] == '\r') {
            if(start[1] == '\n') {
                return start+2;
            }
            return start+1;
        }
        else if(start[0] == '\n') {
            return start+1;
        }
    }
    return start;
}

/**
 * Skip spaces.
 * @param [in] start Pointer to the first character of the string.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer to the first non whitespace character.
 */
const char* skip_spaces(const char *start, const char *end) {
    for(; (start < end) && isspace(*start); ++start) {
    }
    return start;
}

/**
 * Skip quoted string.
 * @param [in] start Pointer to the string quote character.
 * @param [in] end   Pointer past the last character of the string.
 * @return Pointer past the next string quote or end if the terminating quote is missing.
 */
const char* skip_quoted_string(const char *start, const char *end) {
    int quote = *start++;
    if((quote == '\'') || (quote == '"')) {
        for(; start < end; ++start) {
            if(*start == quote) {
                return start+1;
            }
            else if(*start == '\\') { /* skip escaped char */
                ++start;
            }
        }
    }
    return end;
}