#ifndef PCEAS_PARSER_H
#define PCEAS_PARSER_H

#include <stdio.h>
#include "str.h"

#define ERROR_TOKEN -1
#define NONE_TOKEN 0
#define IDENTIFIER_TOKEN 1
#define NUMBER_TOKEN 2

typedef int token_t;

struct parser_t {
    int line;
    token_t token;
    struct string_t value;
    FILE *in;
};

// [todo] message output (log/warning/error)
// [todo] create
// [todo] delete
// [todo] pratt/recurive parsing with operator precedence stuff...

int parser_getc(struct parser_t *parser, int *c);
int parser_ungetc(struct parser_t *parser, int *c);

#endif // PCEAS_PARSER_H
