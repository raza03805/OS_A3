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
    //1048576
    head = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
    head->size = 1024 - sizeof(node_t);
    head->next = NULL;
    return 1;

};



void my_free(void *ptr)
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

void *my_malloc(int size)
{
    //The following code does not check if the requested size is very big that there would be no space for free-list.
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
		
		head = (char *) head + needed_space;


		head->size = actual_free_space - needed_space - sizeof(node_t);
		head->next = NULL;
		return ptr + 2;
	    };
	    
	}
	else
	{
	    printf("\nMemory unavailable!\n");
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
                printf("entered");
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
                    temp = (char *) temp + needed_space;	
                    int free_size = actual_free_space - needed_space - sizeof(node_t); 
		    temp->size = actual_free_space - needed_space - sizeof(node_t);
                    printf("%d",free_size);  
		    temp->next = temp_next;
                    head = temp;
		    return ptr + 2;
	            }
                    else
		    {
                    temp = (char *) temp + needed_space;	
                    int free_size = actual_free_space - needed_space - sizeof(node_t); 
		    temp->size = actual_free_space - needed_space - sizeof(node_t);
                    printf("%d",free_size);  
		    temp->next = temp_next;
                    prev->next = temp;
		    return ptr + 2;
	            }


	    	};

	    	if ((actual_free_space - needed_space) < sizeof(node_t))
	    	{
		    prev->next = temp->next;
		    
	    	};
	    };

	prev = temp;
	temp = temp->next;


        };	
    };

    printf("\nMemory unavailable!\n");
    return NULL;




/*    int head_size = head->size;
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
*/

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
    printf("\nStart of Freelist\n");
    while (temp != NULL)
    {
        printf("%d:%d:%d\n", node_no,temp->size,100); //Not printing address.
        node_no++;
        temp = temp->next;
    };
    printf("End of Freelist\n");

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
    
	
    
    return 0;
}







