#include <stdio.h>
#include <stdint.h>
#include <check.h>

#include "list.h"

struct dummy_t
{
    int  foo;
    char bar[6];
};

static struct dummy_t input[4] =
{
    { 0x004182c3, "test0" },
    { 0x145596d7, "test1" },
    { 0x2869aaeb, "test2" },
    { 0x3c7dbeff, "test3" },
};


START_TEST(test_list_new)
{
    list_t *list;
    
    list = list_new(7);
    fail_if(NULL == list, "List allocation failed");
    list_delete(list);
    
    list = list_new(2000);
    fail_if(NULL == list, "List allocation failed");
    list_delete(list);
}
END_TEST

START_TEST(test_list_push_back)
{
    list_t *list;
    list_iterator_t *iterator;
    int i, ret;
    
    list = list_new(sizeof(struct dummy_t));
    fail_if(NULL == list, "List allocation failed");
    
    for(i=0, ret=0; (ret>=0) && (i<4); i++)
    {
        ret = list_push_back(list, &input[i]);
        fail_if(ret < 0, "Failed to push back element %d", i);
    }
    
    for(iterator=list_front(list), i=0; (i<4) && (NULL!=iterator); iterator=list_iterator_next(iterator), i++)
    {
        fail_if(memcmp(list_iterator_data(iterator), &input[i], sizeof(struct dummy_t)), "Invalid data (forward) %d", i);
    }
    fail_if(4!=i, "Missing elements!");
    fail_if(NULL!=iterator, "Incorrect iterator");
    
    for(iterator=list_back(list), i=3; (i>=0) && (NULL!=iterator); iterator=list_iterator_previous(iterator), i--)
    {
        fail_if(memcmp(list_iterator_data(iterator), &input[i], sizeof(struct dummy_t)), "Invalid data (backward) %d", i);
    }
    fail_if(-1!=i, "Missing elements!");
    fail_if(NULL!=iterator, "Incorrect iterator");
    
    list_delete(list);
}
END_TEST

START_TEST(test_list_push_front)
{
    list_t *list;
    list_iterator_t *iterator;
    int i, ret;
    
    list = list_new(sizeof(struct dummy_t));
    fail_if(NULL == list, "List allocation failed");
    
    for(i=0, ret=0; (ret>=0) && (i<4); i++)
    {
        ret = list_push_front(list, &input[i]);
        fail_if(ret < 0, "Failed to push front element %d", i);
    }
    
    for(iterator=list_front(list), i=3; (i>=0) && (NULL!=iterator); iterator=list_iterator_next(iterator), i--)
    {
        fail_if(memcmp(list_iterator_data(iterator), &input[i], sizeof(struct dummy_t)), "Invalid data (forward) %d", i);
    }
    fail_if(-1!=i, "Missing elements!");
    fail_if(NULL!=iterator, "Incorrect iterator");
    
    for(iterator=list_back(list), i=0; (i<4) && (NULL!=iterator); iterator=list_iterator_previous(iterator), i++)
    {
        fail_if(memcmp(list_iterator_data(iterator), &input[i], sizeof(struct dummy_t)), "Invalid data (backward) %d", i);
    }
    fail_if(4!=i, "Missing elements!");
    fail_if(NULL!=iterator, "Incorrect iterator");
    
    list_delete(list);
}
END_TEST

START_TEST(test_list_copy)
{
    list_t *list[2];
    list_iterator_t *iterator[2];
    int i, ret;
    
    list[0] = list_new(sizeof(struct dummy_t));
    fail_if(NULL == list[0], "List allocation failed");
    
    for(i=0, ret=0; (ret>=0) && (i<4); i++)
    {
        ret = list_push_back(list[0], &input[i]);
        fail_if(ret < 0, "Failed to push back element %d", i);
    }
    
    list[1] = list_copy(list[0]);
    fail_if(NULL == list[1], "List copy failed");
    
    for(i=0; i<2; i++)
    {
        iterator[i] = list_front(list[i]);
    }
    while((NULL != iterator[0]) && (NULL != iterator[1]))
    {
        fail_if(memcmp(list_iterator_data(iterator[0]), list_iterator_data(iterator[1]), sizeof(struct dummy_t)), "Invalid data in forward scan");
        for(i=0; i<2; i++)
        {
            iterator[i] = list_iterator_next(iterator[i]);
        }
    }
    fail_if((NULL != iterator[0]) || (NULL != iterator[1]), "List mismatch");

    for(i=0; i<2; i++)
    {
        iterator[i] = list_back(list[i]);
    }
    while((NULL != iterator[0]) && (NULL != iterator[1]))
    {
        fail_if(memcmp(list_iterator_data(iterator[0]), list_iterator_data(iterator[1]), sizeof(struct dummy_t)), "Invalid data in backward scan");
        for(i=0; i<2; i++)
        {
            iterator[i] = list_iterator_previous(iterator[i]);
        }
    }
    fail_if((NULL != iterator[0]) || (NULL != iterator[1]), "List mismatch");

    for(i=0; i<2; i++)
    {
        list_delete(list[i]);
    }
}
END_TEST

START_TEST(test_list_insert)
{
    static struct dummy_t extras[3] =
    {
        {0x10005000, "____0" },
        {0x02040608, "____1" },
        {0x00300070, "____2" },
    };
    
    list_t *list;
    list_iterator_t *iterator;
    int i, ret;
    
    list = list_new(sizeof(struct dummy_t));
    fail_if(NULL == list, "List allocation failed");
    
    for(i=0; i<4; i++)
    {
        ret = list_push_back(list, &input[i]);
        fail_if(ret < 0, "Failed to push back element %d", i);
    }
    
    iterator = list_front(list);
    ret = list_insert(list, iterator, &extras[0]);
    fail_if(ret < 0, "Failed to insert extra element 0");
    fail_if(memcmp(list_iterator_data(list_front(list)), &extras[0], sizeof(struct dummy_t)), "Invalid data for element 0");
    fail_if(iterator == list_front(list), "Invalid head pointer");
    
    iterator = list_back(list);
    ret = list_insert(list, iterator, &extras[1]);
    fail_if(ret < 0, "Failed to insert extra element 1");
    fail_if(list_back(list) != iterator, "List tail should not have changed");
    
    iterator = list_iterator_previous(iterator);
    fail_if(NULL == iterator, "Invalid pointer");
    fail_if(memcmp(list_iterator_data(iterator), &extras[1], sizeof(struct dummy_t)), "Invalid data for element 1");

    iterator = list_front(list);
    for(i=0; (i<2) && (NULL != iterator); i++)
    {
        iterator = list_iterator_next(iterator);
    }
    fail_if(NULL == iterator, "Invalid list");

    ret = list_insert(list, iterator, &extras[2]);
    fail_if(ret < 0, "Failed to insert extra element 2");

    iterator = list_front(list);
    for(i=0; (i<2) && (NULL != iterator); i++)
    {
        iterator = list_iterator_next(iterator);
    }
    fail_if(NULL == iterator, "Invalid list");
    fail_if(memcmp(list_iterator_data(iterator), &extras[2], sizeof(struct dummy_t)), "Invalid data for element 2");

    list_delete(list);
}
END_TEST

Suite* list_suite()
{
    Suite *suite = suite_create("pceas");
    TCase *tcase = tcase_create("test_list");
    tcase_add_test(tcase, test_list_new);
    tcase_add_test(tcase, test_list_push_back);
    tcase_add_test(tcase, test_list_push_front);
    tcase_add_test(tcase, test_list_copy);
    tcase_add_test(tcase, test_list_insert);
    suite_add_tcase(suite, tcase);
    return suite;
}

int main()
{
    int number_failed;
    Suite *suite = list_suite();

    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return number_failed;
}
