#include "stringutils.h"
#include <stdio.h>
#include <ctype.h>
#include <munit.h>

MunitResult strip_test(const MunitParameter params[], void* user_data)
{
    const char *input[] = 
    {
        "    \tstring 0",
        "string 1\t \t \t \n    ",
        "  \r\n\tstring 2\t\t  \n",
        "string 3",
        "\r\n \t \t \n\r",
        ""
    };
    const char *expected[] = 
    {
        "string 0",
        "string 1",
        "string 2",
        "string 3",
        "",
        ""
    };

    const char *start, *end;

    strip(input[0], &start, &end);
    munit_assert_memory_equal(end-start+1, start, expected[0]);
    munit_assert_int(isspace(*start), ==, 0);
    munit_assert_int(isspace(*end), ==, 0);

    strip(input[1], &start, &end);
    munit_assert_memory_equal(end-start+1, start, expected[1]);
    munit_assert_int(isspace(*start), ==, 0);
    munit_assert_int(isspace(*end), ==, 0);

    strip(input[2], &start, &end);
    munit_assert_memory_equal(end-start+1, start, expected[2]);
    munit_assert_int(isspace(*start), ==, 0);
    munit_assert_int(isspace(*end), ==, 0);

    strip(input[3], &start, &end);
    munit_assert_memory_equal(end-start+1, start, expected[3]);
    munit_assert_int(isspace(*start), ==, 0);
    munit_assert_int(isspace(*end), ==, 0);

    strip(input[4], &start, &end);
    munit_assert_ptr_equal(start, end);

    strip(input[5], &start, &end);
    munit_assert_ptr_equal(start, end);
    munit_assert_char(*start, ==, '\0');

    return MUNIT_OK;
}

MunitTest strip_tests[] =
{
    { "/strip test", strip_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite string_utils_suite = {
  "/string utils", strip_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char **argv)
{
    return munit_suite_main(&string_utils_suite, NULL, argc, argv);
}