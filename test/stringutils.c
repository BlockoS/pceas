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

MunitResult goto_eol_test(const MunitParameter params[], void* user_data)
{
    const char *input[] = 
    {
        "string 0\nextra",
        "string\t1\r\n",
        "string 2\r",
        "string 3",
        "string 4 ; comment\n",
        ""
    };

    const char *start, *end, *ptr;

    start = input[0]; end = start + strlen(start);
    ptr = goto_eol(start, end);
    munit_assert_char(ptr[0], ==, '\n');
    munit_assert_ptr_not_equal(ptr, end);

    start = input[1]; end = start + strlen(start);
    ptr = goto_eol(start, end);
    munit_assert_char(ptr[0], ==, '\r');
    munit_assert_char(ptr[1], ==, '\n');
    munit_assert_ptr_not_equal(ptr, end);

    start = input[2]; end = start + strlen(start);
    ptr = goto_eol(start, end);
    munit_assert_char(ptr[0], ==, '\r');
    munit_assert_ptr_not_equal(ptr, end);

    start = input[3]; end = start + strlen(start);
    ptr = goto_eol(start, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[4]; end = start + strlen(start);
    ptr = goto_eol(start, end);
    munit_assert_char(ptr[0], ==, ';');
    munit_assert_ptr_not_equal(ptr, end);

    ptr = goto_eol(ptr+1, end);
    munit_assert_char(ptr[0], ==, '\n');
    munit_assert_ptr_not_equal(ptr, end);

    ptr = goto_eol(ptr+1, end);
    munit_assert_ptr_equal(ptr, end);
 
    start = input[5]; end = start + strlen(start);
    ptr = goto_eol(start, end);
    munit_assert_ptr_equal(ptr, end);

    return MUNIT_OK;
}

MunitResult goto_space_test(const MunitParameter params[], void* user_data)
{
    const char *input[] = 
    {
        "string   ",
        "abcd\t efg",
        "nospaces",
        "    \t\t___",
        ""
    };

    const char *start, *end, *ptr;

    start = input[0]; end = start + strlen(start);
    ptr = goto_space(start, end);
    munit_assert_char(ptr[0], ==, ' ');
    munit_assert_ptr_not_equal(ptr, end);

    start = input[1]; end = start + strlen(start);
    ptr = goto_space(start, end);
    munit_assert_char(ptr[0], ==, '\t');
    munit_assert_ptr_not_equal(ptr, end);

    start = input[2]; end = start + strlen(start);
    ptr = goto_space(start, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[3]; end = start + strlen(start);
    ptr = goto_space(start, end);
    munit_assert_char(ptr[0], ==, ' ');
    munit_assert_ptr_equal(ptr, start);

    start = input[4]; end = start + strlen(start);
    ptr = goto_space(start, end);
    munit_assert_ptr_equal(ptr, end);

    return MUNIT_OK;
}

MunitResult next_line_test(const MunitParameter params[], void* user_data)
{
    const char *input[] = 
    {
        "line 0",
        "line 0\nline 1",
        "line 0\rline 1",
        "line 0\r\nline 1",
        "line 0\r\nline 1\rline 2\n",
        "line 0\n\r\n\nline 1",
        ""
    };

    const char *start, *end, *ptr;

    start = input[0]; end = start + strlen(start);
    ptr = next_line(start, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[1]; end = start + strlen(start);
    ptr = next_line(start, end);
    munit_assert_char(ptr[0], !=, '\n');
    munit_assert_ptr_not_equal(ptr, end);
    munit_assert_string_equal(ptr, "line 1");

    ptr = next_line(ptr, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[2]; end = start + strlen(start);
    ptr = next_line(start, end);
    munit_assert_char(ptr[0], !=, '\r');
    munit_assert_ptr_not_equal(ptr, end);
    munit_assert_string_equal(ptr, "line 1");

    ptr = next_line(ptr, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[3]; end = start + strlen(start);
    ptr = next_line(start, end);
    munit_assert_char(ptr[0], !=, '\r');
    munit_assert_ptr_not_equal(ptr, end);
    munit_assert_string_equal(ptr, "line 1");

    ptr = next_line(ptr, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[4]; end = start + strlen(start);
    ptr = next_line(start, end);
    munit_assert_ptr_not_equal(ptr, end);
    munit_assert_string_equal(ptr, input[4]+8);

    ptr = next_line(ptr, end);
    munit_assert_ptr_not_equal(ptr, end);
    munit_assert_string_equal(ptr, input[4]+15);

    ptr = next_line(ptr, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[5]; end = start + strlen(start);
    ptr = next_line(start, end);
    munit_assert_char(ptr[0], ==, '\r');
    munit_assert_ptr_not_equal(ptr, end);

    ptr = next_line(ptr, end);
    munit_assert_char(ptr[0], ==, '\n');
    munit_assert_ptr_not_equal(ptr, end);

    ptr = next_line(ptr, end);
    munit_assert_ptr_not_equal(ptr, end);
    munit_assert_string_equal(ptr, "line 1");

    ptr = next_line(ptr, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[6]; end = start + strlen(start);
    ptr = next_line(start, end);
    munit_assert_ptr_equal(ptr, end);

    ptr = next_line(ptr, end);
    munit_assert_ptr_equal(ptr, end);

    return MUNIT_OK;
}

MunitResult quoted_string_test(const MunitParameter params[], void* user_data)
{
    const char *input[] = 
    {
        "'abcdefg\n012345' ",
        "\"    __//\"  ",
        "\'quoted\\'char\' ",
        "'missing    ",
        "\"missing   ",
        "'mismatch\" ",
        "@no string",
        ""
    };

    const char *start, *end, *ptr;

    start = input[0]; end = start + strlen(start);
    ptr = skip_quoted_string(start, end);
    munit_assert_ptr_not_equal(ptr, end);
    munit_assert_char(ptr[-1], ==, '\'');

    start = input[1]; end = start + strlen(start);
    ptr = skip_quoted_string(start, end);
    munit_assert_ptr_not_equal(ptr, end);
    munit_assert_char(ptr[-1], ==, '\"');

    start = input[2]; end = start + strlen(start);
    ptr = skip_quoted_string(start, end);
    munit_assert_ptr_not_equal(ptr, end);
    munit_assert_char(ptr[-1], ==, '\'');

    start = input[3]; end = start + strlen(start);
    ptr = skip_quoted_string(start, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[4]; end = start + strlen(start);
    ptr = skip_quoted_string(start, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[5]; end = start + strlen(start);
    ptr = skip_quoted_string(start, end);
    munit_assert_ptr_equal(ptr, end);

    start = input[6]; end = start + strlen(start);
    ptr = skip_quoted_string(start, end);
    munit_assert_ptr_equal(ptr, end);
    
    return MUNIT_OK;
}

MunitResult simple_parse_test(const MunitParameter params[], void* user_data)
{
    char *line = 
    "   label:  lda    #$00\n"
    "           inc    A\n"
    "           bne    label\n"
    "           rts\n";
    
    const char *start, *end, *ptr;
    const char *eol;
    
    start = line; 
    end   = start + strlen(start);
    
    ptr = start;
    while(ptr != end) {
        int i;
        eol = goto_eol(ptr, end);
        while(ptr != eol) {
            ptr = skip_spaces(ptr, eol);
            
            while((ptr<eol) && (!isspace(*ptr))) {
                ++ptr;
            }
        }
        ptr = next_line(eol, end);
    }    
    return MUNIT_OK;
}

MunitTest string_utils_tests[] =
{
    { "/strip test", strip_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/go to end of line test", goto_eol_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/go to space test", goto_space_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/next line test", next_line_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/quoted string test", quoted_string_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/simple parse test", simple_parse_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite string_utils_suite = {
  "/string utils", string_utils_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char **argv)
{
    return munit_suite_main(&string_utils_suite, NULL, argc, argv);
}