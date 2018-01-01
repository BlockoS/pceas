#include "stringutils.h"
#include <string.h>
#include <ctype.h>

/**
 *
 */
void strip(const char *str, const char **start, const char **end)
{
    for(*start=str; **start && isspace(**start); ++*start) {}
    for(*end=str+strlen(str)-1; (*end > *start)  && isspace(**end); --*end) {}
    if(*end < *start) { *end = *start; }
}
