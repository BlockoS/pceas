#include "parser.h"

int parser_getc(struct parser_t *parser, int *c) {
    if(parser->in == NULL) {
        return -1;
    }
    *c = fgetc(parser->in);
    if((*c == EOF) && ferror(parser->in)) {
        return -1;
    }
    return 0;
}

int parser_ungetc(struct parser_t *parser, int *c) {
    if(parser->in == NULL) {
        return -1;
    }
    if(ungetc(c, parser->in) == EOF) {
        return -1;
    }
    return 0;
}

