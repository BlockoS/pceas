#include <stdio.h>
#include <stdint.h>
#include <check.h>

#include "dict.h"

struct dummy_t
{
    uint32_t foo[3];
    uint32_t bar;
};

static const struct dummy_t input[4] =
{
    { {0x00000060, 0x0000c000, 0x34bcfedc }, 0x01111110 },
    { {0x01000507, 0x000b0d00, 0x25ad89ab }, 0x10011001 },
    { {0x00204000, 0x80a000e0, 0x169e7654 }, 0x10011001 },
    { {0x00030000, 0x0900000f, 0x078f0123 }, 0x01111110 }
};

static const char* key[4] =
{
    "Tyrannosaurus Rex",
    "Kamen Rider",
    "Supercalifragilisticexpialidocious",
    "rx_78_2"
};

START_TEST(test_dict_new)
{
    dict_t *dict;
    
    dict = dict_new(7);
    fail_if(NULL == dict, "Dictionary allocation failed");
    dict_delete(dict);
    
    dict = dict_new(2000);
    fail_if(NULL == dict, "Dictionary allocation failed");
    dict_delete(dict);
}
END_TEST

START_TEST(test_dict_add)
{
    dict_t *dict;
    struct dummy_t* entry;
    struct dummy_t dummy;
    int i, err;
    char buffer[64];
    
    dict = dict_new(sizeof(struct dummy_t));
    fail_if(NULL == dict, "Dictionary allocation failed");
    
    for(i=0; i<4; i++)
    {
        err = dict_add(dict, key[i], &input[i]);
        fail_if(err < 0, "Failed to add entry \"%s\"", key[i]);
    }
    
    entry = dict_find(dict, key[1]);
    fail_if(NULL == entry, "\"%s\" not found", key[1]);
    fail_if(memcmp(entry, &input[1], sizeof(struct dummy_t)), "Invalid data for \"%s\"", key[1]);
    
    for(i=0; i<32768; i++)
    {
        dummy.foo[0] = i >> 24;
        dummy.foo[1] = i >> 16;
        dummy.foo[2] = i >>  8;
        dummy.bar    = i ;
        snprintf(buffer, 64, "%08x", i);
        err = dict_add(dict, buffer, &dummy);
        fail_if(err < 0, "Failed to add entry \"%s\"", buffer);
    }
    
    for(i=0; i<32768; i++)
    {
        /* el cheapo random */
        int j = (i & 0xff00) | ((i & 0x0f) << 4) | ((i & 0xf0) >> 4);
        dummy.foo[0] = j >> 24;
        dummy.foo[1] = j >> 16;
        dummy.foo[2] = j >>  8;
        dummy.bar    = j ;
        snprintf(buffer, 64, "%08x", j);
        entry = dict_find(dict, buffer);
        fail_if(NULL == entry, "\"%s\" not found", buffer);
        fail_if(memcmp(entry, &dummy, sizeof(struct dummy_t)), "Invalid data for \"%s\"", buffer);
    }
    
    dict_delete(dict);
}
END_TEST

START_TEST(test_dict_remove)
{
    dict_t *dict;
    struct dummy_t* entry;
    struct dummy_t dummy;
    int i, err;
    char buffer[64];
    
    dict = dict_new(sizeof(struct dummy_t));
    fail_if(NULL == dict, "Dictionary allocation failed");
    
    for(i=0; i<4; i++)
    {
        err = dict_add(dict, key[i], &input[i]);
        fail_if(err < 0, "Failed to add entry \"%s\"", key[i]);
    }
        
    for(i=0; i<32768; i++)
    {
        dummy.foo[0] = i >> 24;
        dummy.foo[1] = i >> 16;
        dummy.foo[2] = i >>  8;
        dummy.bar    = i ;
        snprintf(buffer, 64, "%08x", i);
        err = dict_add(dict, buffer, &dummy);
        fail_if(err < 0, "Failed to add entry \"%s\"", buffer);
    }

    dict_remove(dict, key[3]);
    
    entry = dict_find(dict, key[3]);
    fail_if(NULL != entry, "\"%s\" was not properly removed", key[3]);
    
    for(i=0; i<32768; i+=2)
    {
        snprintf(buffer, 64, "%08x", i);
        dict_remove(dict, buffer);
    }

    for(i=0; i<32768; i+=2)
    {
        snprintf(buffer, 64, "%08x", i);
        entry = dict_find(dict, buffer);
        fail_if(NULL != entry, "\"%s\" was not properly removed", buffer);
    }
    
    for(i=1; i<32768; i+=2)
    {
        snprintf(buffer, 64, "%08x", i);
        entry = dict_find(dict, buffer);
        fail_if(NULL == entry, "\"%s\" not found", buffer);
    }
    
    for(i=32769; i<65536; i+=2)
    {
        dummy.foo[0] = i >> 24;
        dummy.foo[1] = i >> 16;
        dummy.foo[2] = i >>  8;
        dummy.bar    = i ;
        snprintf(buffer, 64, "%08x", i);
        err = dict_add(dict, buffer, &dummy);
        fail_if(err < 0, "Failed to add entry \"%s\"", buffer);
    }
    
    for(i=1; i<65535; i+=2)
    {
        snprintf(buffer, 64, "%08x", i);
        entry = dict_find(dict, buffer);
        fail_if(NULL == entry, "\"%s\" not found", buffer);
    }
    
    entry = dict_find(dict, key[0]);
    fail_if(NULL == entry, "\"%s\" not found", key[0]);
    
    dict_remove(dict, key[0]);
    entry = dict_find(dict, key[0]);
    fail_if(NULL != entry, "\"%s\" was not properly removed", key[0]);
    
    
    dict_delete(dict);
}
END_TEST

Suite* dict_suite()
{
    Suite *suite = suite_create("pceas");
    TCase *tcase = tcase_create("test_dict");
    tcase_add_test(tcase, test_dict_new);
    tcase_add_test(tcase, test_dict_add);
    tcase_add_test(tcase, test_dict_remove);
    suite_add_tcase(suite, tcase);
    return suite;
}

int main()
{
    int number_failed;
    Suite *suite = dict_suite();

    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return number_failed;
}
