#ifndef PCEAS_LEXER_H
#define PCEAS_LEXER_H

#include "parser.h"

int next_token(struct parser_t *in, struct string_t *out, token_t *token);

#endif // PCEAS_LEXER_H
