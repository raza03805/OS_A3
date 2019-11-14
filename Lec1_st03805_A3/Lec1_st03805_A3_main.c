#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "Lec1_st03805_A3_malloc.h"

int main()
{
    my_init();
    my_showfreelist();
    /*void *p = my_malloc(24);
    my_showfreelist();
    void *a = my_malloc(200);
    my_showfreelist();
    my_free(p);
    my_free(a);
    printf("\npointer pointing to:%d\n",*( (int *) p - 2 ) );
    my_showfreelist();
    void *c = my_malloc(500);
    my_showfreelist();
    void *d = my_malloc(50);
    my_showfreelist();
    void *e = my_malloc(80);
    my_showfreelist();
    void *f = my_malloc(150);

    my_showfreelist();
    my_free(a);
    my_free(c);
    my_free(d);
    my_free(e);
    my_free(f);
    my_showfreelist();
    void *g = my_malloc(108);
    my_showfreelist();
    my_free(g);
    my_showfreelist();
    void *h = my_calloc(10, 10);
    //void *h = my_malloc(100);
    printf("\npointer pointing to:%d\n",*( (int *) h - 2 ) );


    my_showfreelist();
    void *i = my_realloc(h, 100);
    //my_free(h);
    my_showfreelist();
    //my_init();
    //my_uninit();

    void *q = my_malloc(108);
    my_free(q);
    my_showfreelist();

    //my_coalesce();
    //my_showfreelist();*/
    void *a = my_malloc(100);
    my_showfreelist();
    
    //void *b = my_malloc(92);
    //my_showfreelist();
   
   
    my_free(a);
    //my_free(b);

    my_showfreelist();
    printf("%d\n",head->size);
    printf("%p\n",(char *)((node_t *)(head->next) + 1) + (head->next->size));
    printf("%p\n",(char *)(head->next) + (head->next->size) + sizeof(node_t));
    my_coalesce();
    my_showfreelist();
    my_uninit();

    


    /*    my_showfreelist();

        void *h = my_malloc(100);
        my_free(h);

    */
    return 0;
};


