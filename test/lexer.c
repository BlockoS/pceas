#include "lexer.h"
#include <stdio.h>
#include <ctype.h>
#include <munit.h>

struct string_t buffer;
size_t buffer_offset;

int parser_getc(struct parser_t *parser, int *c) {
    if(buffer_offset >= buffer.length) {
        *c = EOF;
        return 0;
    }
    *c = buffer.buffer[buffer_offset++];
    return 0;
}

int parser_ungetc(struct parser_t *parser, int *c) {
    if(buffer_offset && (*c != EOF)) {
        buffer_offset--;
    }
    return 0;
}

MunitResult lexer_test(const MunitParameter params[], void* user_data) {
    struct parser_t parser;
    int ret;

    const char *txt = 
    " \tlda    #$02f0 ; comment\n"
    "  .macro plop\n"
    "    sta  <_al\n"
    "  .endm\n";

    token_t tokens[] = {
        IDENTIFIER_TOKEN, '#', '$', NUMBER_TOKEN, '\n',
        '.', IDENTIFIER_TOKEN, IDENTIFIER_TOKEN, '\n',
        IDENTIFIER_TOKEN, '<',  IDENTIFIER_TOKEN, '\n',
        '.', IDENTIFIER_TOKEN, '\n', EOF  
    };

    const char *str[] = {
        "", "lda", "#", "$", "02f0", "\n",
        ".", "macro", "plop", "\n",
        "sta","<", "_al", "\n",
        ".", "endm", "\n", ""
    };

    buffer_offset = 0;
    ret = string_copy_unsafe(&buffer, txt, strlen(txt));
    munit_assert_int(ret, ==, 0);
    
    parser.token = NONE_TOKEN;
    parser.line = 0;
    string_create(&parser.value, 32);
    
    struct string_t value;
    value.buffer = NULL;
    value.length = value.capacity = 0;
    
    int i = 0;
    while(parser.token != EOF) {
        ret = next_token(&parser, &value, &parser.token);
        munit_assert_int(ret, ==, 0);
        munit_assert_int(parser.token, ==, tokens[i]);
        munit_assert_memory_equal(value.length, value.buffer, str[i]);
        i++;
    }
    string_destroy(&value);
    string_destroy(&buffer);
    
    return MUNIT_OK;
}

MunitTest lexer_tests[] = {
    { "/test", lexer_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite lexer_suite = {
  "/lexer", lexer_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char **argv) {
    return munit_suite_main(&lexer_suite, NULL, argc, argv);
}
