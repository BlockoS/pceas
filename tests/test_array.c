#include <stdio.h>
#include <stdint.h>
#include <check.h>

#include "array.h"

START_TEST(test_array_new)
{
    array_t *array;
    
    array = array_new(0, 0);
    fail_if(NULL != array, "Allocated empty sized array");

    array = array_new(0, 0xcafe);
    fail_if(NULL != array, "Allocated array with 0 sized element");

    array = array_new(0xbeef, 0);
    fail_if(NULL == array, "Could not allocate empty array");
    fail_if(1 != array_capacity(array), "Incorrect array capacity");

    array_delete(array);

    array = array_new(sizeof(char[10]), 7);
    fail_if(NULL == array, "Could not allocate array");
    fail_if(7 != array_capacity(array), "Incorrect array capacity");
    fail_if(7 != array_size(array), "Invalid number of elements.");

    array_delete(array);
}
END_TEST

START_TEST(test_array_push_back)
{
    array_t *array, *tmp;
    uint8_t src[6] = { 0x5c, 0xbe, 0x79, 0xef, 0x00, 0xff };
    uint8_t *data;
    
    array = array_new(sizeof(src), 0);
    fail_if(NULL == array, "Could not allocate array");
    
    tmp = array_push_back(array, src);
    fail_if(NULL  == tmp, "NULL pointer returned");
    fail_if(array != tmp, "Invalid pointer");
    fail_if(1 != array_size(array), "Invalid element count");
    
    data = array_back(array);
    fail_if(NULL == data, "Invalid data pointer");
    fail_if(memcmp(data, src, sizeof(src)), "Invalid data");

    array_delete(array);
}
END_TEST

START_TEST(test_array_push_back_realloc)
{
    array_t *array, *tmp;
    uint8_t src[6] = { 0xab, 0xcd, 0xef, 0x01, 0x23, 0x45 };
    uint8_t other[6] = { 0x67, 0x78, 0x90, 0x12, 0x34, 0x56 };
    uint8_t *data;
    
    array = array_new(sizeof(src), 3);
    fail_if(NULL == array, "Could not allocate array");
    
    tmp = array_push_back(array, src);
    fail_if(NULL  == tmp, "NULL pointer returned");
    fail_if(4 != array_size(tmp), "Invalid element count");
    fail_if(6 != array_capacity(tmp), "Invalid capacity");
    
    data = array_back(tmp);
    fail_if(NULL == data, "Invalid data pointer");
    fail_if(memcmp(data, src, sizeof(src)), "Invalid data");

    array = tmp;
    tmp = array_push_back(array, other);
    fail_if(NULL  == tmp, "NULL pointer returned");
    fail_if(5 != array_size(tmp), "Invalid element count");
    fail_if(6 != array_capacity(tmp), "Invalid capacity");
    
    data = array_back(tmp);
    fail_if(NULL == data, "Invalid data pointer");
    fail_if(memcmp(data, other, sizeof(src)), "Invalid data");

    data = array_at(tmp, array_size(tmp)-1);
    fail_if(NULL == data, "Invalid data pointer");
    fail_if(memcmp(data, other, sizeof(src)), "Invalid data");

    array_delete(tmp);
}
END_TEST

Suite* array_suite()
{
    Suite *suite = suite_create("pceas_array");
    TCase *tcase = tcase_create("test_array");
    tcase_add_test(tcase, test_array_new);
    tcase_add_test(tcase, test_array_push_back);
    tcase_add_test(tcase, test_array_push_back_realloc);
    suite_add_tcase(suite, tcase);
    return suite;
}

int main()
{
    int number_failed;
    Suite *suite = array_suite();

    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return number_failed;
}
