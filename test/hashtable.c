#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hashtable.h"

// [todo] unit test lib

int main()
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

    int index[] =
    {
        6, 3, 5, 0, 7, 2, 1, 4
    };

    struct htab_t *tab = htab_create(sizeof(uint64_t));
    for(int i=0; i<8; i++)
    {
        htab_add(tab, test_data[i].key, strlen(test_data[i].key), &test_data[i].value);
    }
    
    for(int i=0; i<8; i++)
    {
        int j = index[i];
        uint64_t *value = htab_get(tab, test_data[j].key, strlen(test_data[j].key));
        printf("%0lx %0lx\n", *value, test_data[j].value);
    }
    
    printf("%p\n", htab_get(tab, "rocket", 5));
    
    htab_destroy(tab);
    return EXIT_SUCCESS;
}
