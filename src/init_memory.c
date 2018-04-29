#include "../includes/malloc.h"
#include <stdio.h>
#include <stdlib.h>

struct ovrl_block *g_dma = NULL;

static void init_tiny(void)
{
    g_dma->tiny = (struct t_block *)(g_dma->large + 1);
    g_dma->tiny->blck_limit = g_dma->tiny + 1;
    g_dma->tiny->next = NULL;

    /*g_dma->tiny = (struct t_block *)(g_dma + 1);
    g_dma->tiny->blck_limit = g_dma->tiny + 2;
    g_dma->tiny_limit = (size_t)T_LIMIT;

    g_dma->tiny->next = NULL;
    g_dma->addr.tiny_start = (char*)g_dma->tiny;*/
}

static void init_small(void)
{
    g_dma->small = (struct s_block *)((char *)g_dma->tiny + TINY_ZONE);
    g_dma->small->blck_limit = g_dma->small + 1;
    g_dma->small->next = NULL;

    /*g_dma->small = (struct s_block *)((char *)g_dma->tiny + g_dma->tiny_limit);
    g_dma->small->blck_limit = g_dma->small + 1;
    g_dma->small_limit = (size_t)S_LIMIT;

    g_dma->small->next = NULL;
    g_dma->addr.small_start = (char*)g_dma->small;*/
}

static void init_large(void)
{
    g_dma->large = (struct l_block*)(g_dma + 1);
    g_dma->large->next = NULL;

    /*g_dma->large = (struct l_block*)(g_dma->tiny + 1);
    g_dma->large->blck_limit= g_dma->large + 1;
    g_dma->large->next = NULL;

    g_dma->addr.large_start = (char *)g_dma->large;*/
}

void init_memory(void)
{
  	if (!g_dma)
	{
	 /* if ((g_dma = mmap(NULL, HDR_SIZE
                              + PREALOC_SIZE_TINY
                              + PREALOC_SIZE_SMALL,
                              MMAP_FLAGS, OFFSET, 0)) == MAP_FAILED)
	    	exit(0);*/

        if ((g_dma = mmap(NULL, sizeof(struct ovrl_block) + TINY_ZONE + SMALL_ZONE, \
		PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
            exit(0);

        init_large();
	  	init_tiny();
		init_small();
        g_dma->large->blck_limit= (struct l_block *)((char *)g_dma->small + SMALL_ZONE);
		//g_dma->get_tiny = push_tiny_chunk;
        //g_dma->get_small = push_small_chunk;
        //g_dma->get_large = push_large_chunk;
        g_dma->get_block[TINY] = push_tiny_chunk;//g_dma->get_tiny;
        g_dma->get_block[SMALL] = push_small_chunk;//g_dma->get_small;
        g_dma->get_block[LARGE] = push_large_chunk;//g_dma->get_large;

        g_dma->print_block[TINY] = start_print_tiny;
        g_dma->print_block[SMALL] = start_print_small;
        g_dma->print_block[LARGE] = start_print_large;
    }
}

#include <string.h>
int main(void)
{
  /*char *ptr = mallok(20);
    show_alloc_mem();
  char *ptr2 = mallok(21);
  char *ptr3 = mallok(24);
    show_alloc_mem();
    char *ptr4 = mallok(23);
    mallok(25);
    char *test;
    free(ptr);
    free(ptr2);
    free(test);
    show_alloc_mem();*/


    char *ptr = mallok(2);

    strcpy(ptr, "aa");

    show_alloc_mem();


    ptr = realloc(ptr, 6890);

    strcpy(ptr, "hello my world");

    show_alloc_mem();

    return 0;

  return 0;
}
