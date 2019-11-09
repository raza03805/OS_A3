#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>



typedef struct node_t
{
    int size;
    struct node_t *next;
    
} node_t;

node_t *head = NULL; //points to the start of free list.

int my_init()
{
    head = mmap(NULL, 1048576, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    head->size = 1048576 - sizeof(node_t);
    head->next = NULL;
    return 11;

};

void *my_malloc(int size)
{
    int head_size = head->size;
    int actual_size = size + 8;
    if (head->next == NULL)
    {
	
	if (head->size >= actual_size)
	{
	    int *ptr = (int *) head;
	    *ptr = size;
        *(ptr + 1) = 12345; //Overwriting on struct space, which is of 16 bytes.
	    printf("Allocated %d\n",size);
	    
	    head_size -= actual_size;
	    head = head + size; //Do i need to cast it into char to move head pointer?. 
	    head->size = head_size;
	    return (ptr + 2);

	};
    };

    if (head->next !=NULL)
    {
        node_t* temp = head;
        node_t* prev = head;

        while(temp != NULL)
        {
            if (temp->size == actual_size)
            {
                if(temp->next == NULL)
                {
                    prev->next = NULL;
                };
            

                if(temp->next != NULL)
                {
                    prev->next = temp->next;
                };

	        int *ptr = (int *) temp;
	        *ptr = size;
            *(ptr + 1) = 12345; //Overwriting on struct space, which is of 16 bytes.
	        printf("Allocated %d\n",size);
			return ptr + 2;

            };

            if (temp->size > actual_size)
            {
				int temp_size = temp->size;
				node_t *temp_next = temp->next;
				int *ptr = (int *) temp;
				*ptr = size;
				*(ptr + 1) = 12345; //Overwriting on struct space, which is of 16 bytes.
				printf("Allocated %d\n",size);
				
				
				temp_size -= actual_size;
				prev->next = ptr + size; //Do i need to cast it into char to move head pointer?. 
				prev->size = head_size;


				return ptr + 2;
                
            };

        prev = temp;
	    temp = temp->next;
        };

    printf("No space of size %d available.",size);
    return NULL;

	
    };


    /*size += 8;
    node_t* temp = head;
    node_t* prev = head;

    while(temp != NULL && temp->size < size)
    {
	prev = temp;
	temp = temp->next;
    };
   

    if(temp->next == NULL)
    {
        prev->next = NULL;
    };

    if(temp->next != NULL)
    {
        prev->next = temp->next;
    };
    
    
    if(temp == NULL)
    {
	printf("No space of size %d available.",size);
	return NULL;
    };


    int *ptr = (int *) temp;
    
    *ptr = size;
    *(ptr + 1) = 12345;
	
    size -= 8;
    if(temp != NULL)
    {
	
	printf("Allocated %d\n",size);
    };


    return ptr + 2;*/

};

void my_showfreelist()
{
    int node_no = 0;
    node_t *temp = head;
    while (temp != NULL)
    {
        printf("%d:%d:%d\n", node_no,temp->size,100);
        node_no++;
        temp = temp->next;
    };

};


int main()
{
    my_init();
    my_showfreelist();
    my_malloc(100);
    my_showfreelist();
    my_malloc(200);
    my_showfreelist();
	
    
    return 0;
}







