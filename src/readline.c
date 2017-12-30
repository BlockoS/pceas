#include "readline.h"

#include <stdlib.h>

char* read_line(FILE *in, char* buffer, size_t* len)
{
    char *tmp = buffer;
    if((0 == *len) || (NULL == buffer))
    {
        *len = 64;
        tmp = (char*)malloc(*len);
    }
    int i = 0;
    int current, previous = 0;
    while((current = fgetc(in)) != EOF)
    {
        if(current == '\r')
        {
            previous = current;
            current = fgetc(in);
        }
        if((current == '\n') || (current == EOF))
        {
            break;
        }
        if(previous == '\r')
        {
            fseek(in, -1, SEEK_SET);
            break;
        }
        if((i+1) >= *len)
        {
            size_t n = *len * 2;
            tmp = (char*)realloc(tmp, n);
            *len = n;
        }
        tmp[i++] = current;
        previous = current;
    }
    tmp[i] = '\0';
    return tmp;
}
