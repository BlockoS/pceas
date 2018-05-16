#include "str.h"
#include <stdio.h>
#include <ctype.h>
#include <munit.h>

MunitResult string_push_test(const MunitParameter params[], void* user_data)
{
    const char *in = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    size_t len = strlen(in);
    struct string_t str;
    int ret;

    ret = string_create(&str, 8);
    munit_assert_int(ret, ==, 0);
    for(size_t i=0; i<len; i++) {
        ret = string_push(&str, in[i]);
        munit_assert_int(ret, ==, 0);
    }
    munit_assert_size(str.length, ==, len);
    munit_assert_memory_equal(len, str.buffer, in);
    return MUNIT_OK;
}

MunitResult string_pop_test(const MunitParameter params[], void* user_data)
{
    const char *in = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    size_t len = strlen(in);
    off_t offset;
    struct string_t str = {
        .buffer = NULL,
        .length = 0,
        .capacity = 0
    };
    int ret;
    char c;

    ret = string_copy_unsafe(&str, in, len);
    munit_assert_int(ret, ==, 0);
    offset = len;
    do {
        c = string_pop(&str);
        munit_assert_char(c, ==, in[--offset]);
    } while((c != '\0') && (offset >= 0));
    munit_assert_char(c, ==, '\0');
    return MUNIT_OK;
}

MunitTest string_tests[] =
{
    { "/push test", string_push_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/pop test", string_pop_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite string_suite = {
  "/string", string_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char **argv)
{
    return munit_suite_main(&string_suite, NULL, argc, argv);
}
