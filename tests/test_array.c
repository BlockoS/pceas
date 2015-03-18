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

START_TEST(test_array_push_front)
{
    array_t *array, *tmp;
    uint8_t src[4][6] = 
    {
        { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab },
        { 0xcd, 0xef, 0xf9, 0x87, 0x65, 0x43 },
        { 0x21, 0x00, 0x0a, 0x1b, 0x2c, 0x3d },
        { 0x4e, 0x5f, 0x6a, 0x7b, 0x8c, 0x9d }
    };
    uint8_t *data;
    int i, j;
    
    array = array_new(sizeof(src[0]), 0);
    fail_if(NULL == array, "Could not allocate array");
    
    for(i=0; i<4; i++)
    {
        tmp = array_push_front(array, src[i]);
        fail_if(NULL  == tmp, "NULL pointer returned");
        fail_if((i+1) != array_size(tmp), "Invalid element count");
        
        data = array_front(tmp);
        fail_if(NULL == data, "Invalid data pointer");
        fail_if(memcmp(data, src[i], sizeof(src[i])), "Invalid data");
        
        for(j=1; j<=i; j++)
        {
            data = array_at(tmp, j);
            fail_if(NULL == data, "Invalid data pointer");
            fail_if(memcmp(data, src[i-j], sizeof(src[i-j])), "Invalid data");
        }
        
        array = tmp;
    }
    
    array_delete(array);
}
END_TEST

START_TEST(test_array_insert)
{
    array_t *array, *tmp;
    uint8_t src[4][6] = 
    {
        { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab },
        { 0xcd, 0xef, 0xf9, 0x87, 0x65, 0x43 },
        { 0x21, 0x00, 0x0a, 0x1b, 0x2c, 0x3d },
        { 0x4e, 0x5f, 0x6a, 0x7b, 0x8c, 0x9d }
    };
    size_t index[4] = 
    {
        0, 3, 2, 1
    };
    uint8_t *data;
    int i;
    
    array = array_new(sizeof(src[0]), 0);
    fail_if(NULL == array, "Could not allocate array");
    
    for(i=0; i<2; i++)
    {
        size_t j;

        array = array_push_back(array, src[i]);
        fail_if(NULL  == array, "NULL pointer returned");
    }
    
    for(; i<4; i++)
    {
        tmp = array_insert(array, 1, src[i]);
        fail_if(NULL  == tmp, "NULL pointer returned");
        fail_if((i+1) != array_size(tmp), "Invalid size");
        array = tmp;
    }
    
    for(i=0; i<4; i++)
    {
        fail_if(memcmp(array_at(array, i), src[index[i]], sizeof(src[index[i]])), "Invalid data at index %d", i);
    }
    
    array_delete(array);
}
END_TEST

START_TEST(test_array_pop_back)
{
    array_t *array, *tmp;
    uint8_t src[4][6] = 
    {
        { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab },
        { 0xcd, 0xef, 0xf9, 0x87, 0x65, 0x43 },
        { 0x21, 0x00, 0x0a, 0x1b, 0x2c, 0x3d },
        { 0x4e, 0x5f, 0x6a, 0x7b, 0x8c, 0x9d }
    };
    uint8_t *data;
    size_t i;
    
    array = array_new(sizeof(src[0]), 0);
    fail_if(NULL == array, "Could not allocate array");
    
    for(i=0; i<4; i++)
    {
        array = array_push_back(array, src[i]);
        fail_if(NULL  == array, "Push back failed");
    }
    
    i = array_size(array);
    do
    {
        array_pop_back(array);
        --i;
        fail_if(i != array_size(array), "Invalid size");
        data = array_back(array);
        if(array_size(array))
        {
            fail_if(NULL == data, "Empty array");
            fail_if(memcmp(data, src[i-1], sizeof(src[i-1])), "Invalid data");
        }
        else
        {
            fail_unless(NULL == data, "The array should be empty at this point");
        }
    }while(array_size(array));
    
    array_delete(array);
}
END_TEST

START_TEST(test_array_pop_front)
{
    array_t *array, *tmp;
    uint8_t src[4][6] = 
    {
        { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab },
        { 0xcd, 0xef, 0xf9, 0x87, 0x65, 0x43 },
        { 0x21, 0x00, 0x0a, 0x1b, 0x2c, 0x3d },
        { 0x4e, 0x5f, 0x6a, 0x7b, 0x8c, 0x9d }
    };
    uint8_t *data;
    size_t i, j;
    
    array = array_new(sizeof(src[0]), 0);
    fail_if(NULL == array, "Could not allocate array");
    
    for(i=0; i<4; i++)
    {
        array = array_push_back(array, src[i]);
        fail_if(NULL  == array, "Push back failed");
    }
    
    i = array_size(array);
    j = 0;
    do
    {
        array_pop_front(array);
        --i; ++j;
        fail_if(i != array_size(array), "Invalid size");
        data = array_front(array);
        if(array_size(array))
        {
            fail_if(NULL == data, "Empty array");
            fail_if(memcmp(data, src[j], sizeof(src[j])), "Invalid data");
        }
        else
        {
            fail_unless(NULL == data, "The array should be empty at this point");
        }
    }while(array_size(array));
    
    array_delete(array);
}
END_TEST

START_TEST(test_array_remove)
{
    array_t *array;
    uint8_t src[5][6] = 
    {
        { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab },
        { 0xcd, 0xef, 0xf9, 0x87, 0x65, 0x43 },
        { 0x21, 0x00, 0x0a, 0x1b, 0x2c, 0x3d },
        { 0x4e, 0x5f, 0x6a, 0x7b, 0x8c, 0x9d },
        { 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3 }
    };
    size_t index[3] = { 0, 2, 4 };
    uint8_t *data;
    size_t i;
    
    array = array_new(sizeof(src[0]), 0);
    fail_if(NULL == array, "Could not allocate array");
    
    for(i=0; i<5; i++)
    {
        array = array_push_back(array, src[i]);
        fail_if(NULL  == array, "Push back failed");
    }
    
    array_remove(array, 3);
    fail_if(4 != array_size(array), "Invalid array size");
    
    array_remove(array, 1);
    fail_if(3 != array_size(array), "Invalid array size");
    
    for(i=0; i<3; i++)
    {
        fail_if(memcmp(array_at(array, i), src[index[i]], sizeof(src[0])), "Invalid data at index %d", i);
    }
    
    array_delete(array);
}
END_TEST

Suite* array_suite()
{
    Suite *suite = suite_create("pceas");
    TCase *tcase = tcase_create("test_array");
    tcase_add_test(tcase, test_array_new);
    tcase_add_test(tcase, test_array_push_back);
    tcase_add_test(tcase, test_array_push_back_realloc);
    tcase_add_test(tcase, test_array_push_front);
    tcase_add_test(tcase, test_array_insert);
    tcase_add_test(tcase, test_array_pop_back);
    tcase_add_test(tcase, test_array_pop_front);
    tcase_add_test(tcase, test_array_remove);
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
