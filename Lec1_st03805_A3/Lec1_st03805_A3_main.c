#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "Lec1_st03805_A3_malloc.h"

int main()
{
    my_init();
    void *a = my_calloc(5,10);
    my_free(a);
    my_free(a);
    my_showfreelist();
    return 0;
};












