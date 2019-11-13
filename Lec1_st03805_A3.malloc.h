#ifndef RANDOM
#define RANDOM




#define magic 12345
#define memory 1024
//#define memory 1048576



typedef struct node_t
{
    int size;
    struct node_t *next;

} node_t;

extern node_t *head;
extern node_t *start_mmap;

int my_init();
void my_free(void *ptr);
void *my_malloc(int size);
void *my_calloc(int num, int size);
void *my_realloc(void *old_ptr, int new_size);
void my_showfreelist();
void my_coalesce();
void my_uninit();
 

#endif
