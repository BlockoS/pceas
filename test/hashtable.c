#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <hashtable.h>

#include <munit.h>

MunitResult htab_insert_test(const MunitParameter params[], void* user_data_or_fixture)
{
    struct 
    {
        char *key;
        uint64_t value;
    } test_data[] = 
    {
        { "abc", 0xa00000000000 },
        { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 0x0123456789ab },
        { "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklm"
          "nhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu", 0x0000ffff },
        { "@_[+]/", 0xdeadbeef },
        { "(0000)", 0x01 },
        { "(0001)", 0x02 },
        { "(0002)", 0x03 },
        { "(0003)", 0x04 }
    };
    int index[] = { 6, 3, 5, 0, 7, 2, 1, 4 };
    struct htab_t *tab;
    int i, ret;
    
    (void)params, (void)user_data_or_fixture;
    
    tab = htab_create(sizeof(uint64_t));
    munit_assert_not_null(tab);
    for(int i=0; i<8; i++)
    {
        ret = htab_add(tab, test_data[i].key, strlen(test_data[i].key), &test_data[i].value);
        munit_assert_int(ret, ==, 0);
    }
    
    for(int i=0; i<8; i++)
    {
        int j = index[i];
        uint64_t *value = htab_get(tab, test_data[j].key, strlen(test_data[j].key));
        munit_assert_uint64(*value, ==, test_data[j].value);
    }
    htab_destroy(tab);
    return MUNIT_OK;
}

MunitResult htab_remove_test(const MunitParameter params[], void* user_data_or_fixture)
{
    struct 
    {
        char key[16];
        uint64_t value;
    } test_data[8];
    int ret, i;
    uint64_t *entry;
    struct htab_t *tab;
    htab_iterator_t it;
    
    (void)params, (void)user_data_or_fixture;
        
    tab = htab_create(sizeof(uint64_t));
    munit_assert_not_null(tab);

    for(i=0; i<4; i++) 
    {
        snprintf(test_data[i].key, 16, "entry #%02d", i);
        test_data[i].value = i;
        ret = htab_add(tab, test_data[i].key, strlen(test_data[i].key), &test_data[i].value);
        munit_assert_int(ret, ==, 0);
    }
    
    ret = htab_remove(tab, test_data[2].key, strlen(test_data[2].key));
    munit_assert_int(ret, ==, 0);

    ret = htab_remove(tab, "out for lunch", 13);
    munit_assert_int(ret, !=, 0);

    entry = (uint64_t*)htab_get(tab, test_data[2].key, strlen(test_data[2].key));
    munit_assert_null(entry);

    for(i=4; i<8; i++) 
    {
        snprintf(test_data[i].key, 16, "entry #%02d", i);
        test_data[i].value = i;
        ret = htab_add(tab, test_data[i].key, strlen(test_data[i].key), &test_data[i].value);
        munit_assert_int(ret, ==, 0);
    }

    for(i=0,it=htab_begin(tab); it!=htab_end(tab); it=htab_next(tab, it), i++)
    {
        const char *key;
        size_t len;
        uint64_t *value;

        ret = htab_key(tab, it, &key, &len);
        munit_assert_int(ret, ==, 0);
        munit_assert_not_null(key);
        munit_assert_memory_not_equal(len, key, test_data[2].key);
        
        value = (uint64_t*)htab_value(tab, it);
        munit_assert_uint64(*value, !=, test_data[2].value);
    }
        
    htab_destroy(tab);
    return MUNIT_OK;
}

MunitTest htab_tests[] =
{
    { "/table insert test", htab_insert_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/table remove test", htab_remove_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    /* */
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite htab_suite = {
  "/htab", htab_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char **argv)
{
    return munit_suite_main(&htab_suite, NULL, argc, argv);
}
