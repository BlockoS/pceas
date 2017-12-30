#include "readline.h"

#include <munit.h>

FILE* create_file(char* text)
{
    size_t n;
    size_t text_len = strlen(text);
    
    FILE *out = tmpfile();
    munit_assert_not_null(out);
    
    n = fwrite(text, 1, text_len, out);
    munit_assert_size(n, ==, text_len);

    fflush(out);
    fseek(out, 0, SEEK_SET);

    return out;
}

void* read_line_setup(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;
    return create_file(
        " line 1\r\n"
        " line 2\n"
        " line 3\n"
        " line 4");
}

void read_line_tear_down(void* fixture) {
    fclose((FILE*)fixture);
}

MunitResult read_line_test(const MunitParameter params[], void* fixture)
{
    char dummy[32];
    char* buffer = NULL;
    char* line = NULL;
    size_t len = 0;
    
    (void)params;
    
    FILE *in = (FILE*)fixture;
    
    for(int i=0; i<4; i++)
    {
        line = read_line(in, buffer, &len);
        munit_assert_not_null(line);
        
        snprintf(dummy, 32, " line %d", i+1);
        munit_assert_string_equal(line, dummy);
        
        buffer = line;
    }

    line = read_line(in, buffer, &len);
    munit_assert_size(strlen(line), ==, 0);

    free(buffer);
    
    return MUNIT_OK;
}

void* empty_lines_setup(const MunitParameter params[], void* user_data)
{
    (void)params;
    (void)user_data;
    return create_file(
        "line 1\r\n"
        "\r\n"
        "line 2\n"
        "\n"
        "\r\n"
        "line 3\n"
        "\r\n");
}

MunitResult empty_lines_test(const MunitParameter params[], void* fixture)
{
    char* buffer = NULL;
    char* line = NULL;
    size_t len = 0;
    
    (void)params;
    
    FILE *in = (FILE*)fixture;
    
    line = read_line(in, buffer, &len);
    munit_assert_not_null(line);
    munit_assert_string_equal(line, "line 1");
    buffer = line;
    
    line = read_line(in, buffer, &len);
    munit_assert_not_null(line);
    munit_assert_size(strlen(line), ==, 0);
    buffer = line;

    line = read_line(in, buffer, &len);
    munit_assert_not_null(line);
    munit_assert_string_equal(line, "line 2");
    buffer = line;

    line = read_line(in, buffer, &len);
    munit_assert_not_null(line);
    munit_assert_size(strlen(line), ==, 0);
    buffer = line;

    line = read_line(in, buffer, &len);
    munit_assert_not_null(line);
    munit_assert_size(strlen(line), ==, 0);
    buffer = line;

    line = read_line(in, buffer, &len);
    munit_assert_not_null(line);
    munit_assert_string_equal(line, "line 3");
    buffer = line;

    line = read_line(in, buffer, &len);
    munit_assert_not_null(line);
    munit_assert_size(strlen(line), ==, 0);
    buffer = line;

    line = read_line(in, buffer, &len);
    munit_assert_not_null(line);
    munit_assert_size(strlen(line), ==, 0);
    buffer = line;

    free(buffer);    
    return MUNIT_OK;
}

MunitTest read_line_tests[] =
{
    { "/read lines test", read_line_test, read_line_setup, read_line_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
    { "/empty lines test", empty_lines_test, empty_lines_setup, read_line_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite read_line_suite = {
  "/readline", read_line_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char **argv)
{
    return munit_suite_main(&read_line_suite, NULL, argc, argv);
}