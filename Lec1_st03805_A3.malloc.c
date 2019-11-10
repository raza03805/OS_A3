#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define magic 12345

typedef struct node_t
{
    int size;
    struct node_t *next;

} node_t;

node_t *head = NULL; //points to the start of free list.


int my_init()
{
    //1048576
    if (head != NULL)
    {
	printf("Request rejected!, my_init alreay initiallized!");
        return 0;
    };
    head = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    if (head == MAP_FAILED)
    {
	printf("Request rejected!, mmap call unsuccessful!!");
        return 0;
    };
   
    head->size = 1024 - sizeof(node_t);
    head->next = NULL;
    return 1;

};



void my_free(void *ptr)
{
    if (*((int *)ptr - 1) != magic)
    {
	printf("\nInvalid pointer given!\n");
    }
    else
    {
	    node_t *temp = head;
	    while (temp->next != NULL)
	    {
		temp = temp->next;

	    };

	    int ptr_size = *((int *)ptr - 2);
	    int *tptr = ptr - 2;
	    temp->next = (node_t *) tptr;
	    temp->next->size = ptr_size;
	    temp->next->next = NULL;
    };
};

void *my_malloc(int size)
{
    //The following code does not check if the requested size is very big that there would be no space for free-list.

    if (head == NULL)
    {
	printf("my_malloc unsuccessful, my_init isnt initiallized!");
	return NULL;	

    };

      
    if (head->next == NULL)
    {
        int old_head_size = head->size;
        int actual_free_space = sizeof(node_t) + old_head_size;
        int needed_space = (2*sizeof(int)) + size;

        if (actual_free_space >= needed_space)
        {
            int* ptr = (int*) head;
            *ptr = size;
            *(ptr+1) = 12345; //Magic number.
            printf("\nMemory of size %d allocated!\n", size);

            if ((actual_free_space - needed_space) >= sizeof(node_t))
            {
                /*char *t = (char *) head;
                t = t + needed_space;
                (char *)head = t;*/

                head = (node_t *)((char *) head + needed_space);


                head->size = actual_free_space - needed_space - sizeof(node_t);
                head->next = NULL;
                return ptr + 2;
            };

        }
        else
        {
            printf("\nMemory of size %d unavailable!\n",size);
            return NULL;
        };
    };

    if (head->next != NULL)
    {
        node_t* temp = head;
        node_t* prev = head;

        while(temp != NULL)
        {
            int old_temp_size = temp->size;
            int actual_free_space = sizeof(node_t) + old_temp_size;
            int needed_space = (2*sizeof(int)) + size;

            if (actual_free_space == needed_space)
            {

                int* ptr = (int*) temp;
                *ptr = size;
                *(ptr+1) = 12345; //Magic number.
                printf("\nMemory of size %d allocated!\n", size);

                if (temp == head)
                {
                    head = head->next;
                };

                if (temp != head)
                {
                    prev->next = temp->next;
                };

                return ptr + 2;


            };

            if (actual_free_space > needed_space)
            {
                //printf("entered");
                node_t *temp_next = temp->next;
                int* ptr = (int*) temp;
                *ptr = size;
                *(ptr+1) = 12345; //Magic number.
                printf("\nMemory of size %d allocated!\n", size);

                if ((actual_free_space - needed_space) >= sizeof(node_t))
                {
                    /*char *t = (char *) head;
                    t = t + needed_space;
                    (char *)head = t;*/

                    if (head == temp)

                    {
                        temp = (node_t *)((char *) temp + needed_space);
                        int free_size = actual_free_space - needed_space - sizeof(node_t);
                        temp->size = actual_free_space - needed_space - sizeof(node_t);
                        //printf("%d",free_size);
                        temp->next = temp_next;
                        head = temp;
                        return ptr + 2;
                    }
                    else
                    {
                        temp = (node_t *)((char *) temp + needed_space);
                        int free_size = actual_free_space - needed_space - sizeof(node_t);
                        temp->size = actual_free_space - needed_space - sizeof(node_t);
                        //printf("%d",free_size);
                        temp->next = temp_next;
                        prev->next = temp;
                        return ptr + 2;
                    }


                };

                if ((actual_free_space - needed_space) < sizeof(node_t))
                {
                    if (temp != head)
                    {
			prev->next = temp->next;
			return ptr + 2;
		    }
		    else
                    {
			head = head->next;
                        return ptr + 2;
                       
                    }
                };
            };

            prev = temp;
            temp = temp->next;


        };
    };

    printf("\nMemory of size %d unavailable!\n",size);
    return NULL;
};

void *my_calloc(int num, int size)
{

    if (head == NULL)
    {
	printf("my_realloc unsuccessful, my_init isnt initiallized!");
	return NULL;	

    };

    int total_size = num*size;
    printf("%d",total_size);
    void *p = my_malloc(total_size);
    printf("\n%d\n",*((int *)p - 1));
    if (p == NULL)
    { 
	printf("calloc isnt successful!");

    }
    else
    {
	char *ptr = (char *) p;
	int counter = 0;
	while (counter != total_size)
 	{
	    *(ptr + counter) = "A";
            counter++;
	};
    
    };

    return p;

};

void *my_realloc(void *old_ptr, int new_size)
{

    if (head == NULL)
    {
	printf("my_realloc unsuccessful, my_init isnt initiallized!");
	return NULL;	

    };

    int old_size = *(((int *) old_ptr) - 2);
    printf("%d", old_size);
    int needed_size = old_size + new_size;

    void *new_ptr = my_malloc(needed_size);

    if (new_ptr == NULL)
    {
	printf("Reqest of realloc rejected due to unavailability of space!\n");
	return NULL;
    };
    
    int counter = 0;

    while (counter != old_size)
    {
	*((char *) new_ptr + counter) = *((char *) old_ptr + counter);
	counter++; 

    };
    
    printf("Reqest of realloc accepted!\n");
    my_free(old_ptr);
    return new_ptr;
};




void my_showfreelist()
{
    if (head != NULL)
    {
    int node_no = 0;
    node_t *temp = head;
    printf("\nStart of Freelist\n");
    while (temp != NULL)
    {
        printf("%d:%d:%p\n", node_no,temp->size,temp); //Not printing address.
        node_no++;
        temp = temp->next;
    };
    printf("End of Freelist\n");
    }; 

    if (head == NULL)
    {
	printf("my_showfreelist unsuccessful, my_init isnt initiallized!");	

    };
};


int main()
{
    my_init();
    my_showfreelist();
    void *p = my_malloc(24);
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
    my_init();

    return 0;
};







