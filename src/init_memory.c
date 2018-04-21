#include "../includes/malloc.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct ovrl_block *g_dma = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

enum CHUNK_TYPE {TINY, SMALL};



#define ADD_CHUNK_TO_DMA(TYPE, TINY_NODE, TMP) ({\
    TMP = TINY_NODE->next; \
    TINY_NODE->next = TINY_NODE->blck_limit; \
    TINY_NODE->next->blck_limit = \
    (struct t_block*)((char*)tiny_node->next + size); \
    TINY_NODE->next->next = tmp; \
    TINY_NODE->next->size = size; \
})

void *add_tiny(struct t_block *tiny_head, size_t size, size_t max)
{
    struct  t_block *tiny_node;
    struct  t_block *tmp;
    size += sizeof(struct t_block);

    tiny_node = tiny_head;
    while (tiny_node->next != NULL)
    {
        if ((char *)tiny_node->next - (char *)tiny_node->blck_limit >= size)
	    break;
        tiny_node = tiny_node->next;
    }

    if ((char*)tiny_node->blck_limit + size > (char*)tiny_head + max)
    {
        errno = ENOMEM;
        return (NULL);
    }
    ADD_CHUNK_TO_DMA(TINY, tiny_node, tmp);
    return (tiny_node->next + 1);
}


static void init_tiny(void)
{
    g_dma->tiny = (struct t_block *)(g_dma + 1);
    g_dma->tiny->blck_limit = g_dma->tiny + 2;
    g_dma->tiny_limit = (size_t)T_LIMIT;

    g_dma->tiny->next = NULL;
    g_dma->tiny->size = 0;
    g_dma->tiny->inuse = 0;

    g_dma->addr.tiny_start = (char*)g_dma->tiny;
}

static void init_small(void)
{
    g_dma->small = (struct s_block *)((char *)g_dma->tiny + g_dma->tiny_limit);
    g_dma->small->blck_limit = g_dma->small + 1;
    g_dma->small_limit = (size_t)S_LIMIT;

    g_dma->small->next = NULL;
    g_dma->small->size = 0;
    g_dma->small->inuse = 0;

    g_dma->addr.small_start = (char*)g_dma->small;
}

long print_alloc(struct t_block *node)
{
  long size;

  printf("%lx", (long)(node + 1));
  printf(" - ");
  printf("%lx", (long)(node->blck_limit));
  printf(" : ");
  size = node->size;//(char*)node->blck_limit - (char*)node - sizeof(struct t_block);
  printf("%ld ", size);
  puts(" octets\n");
  return (size);
}

long print_malloc(struct t_block *node)
{
  long total;

  total = 0;
  while (node->next != NULL)
    {
      node = node->next;
      total += print_alloc(node);
    }
    return (total);
}

void show_alloc_mem(void)
{
    printf("TINY: %p\n", g_dma->tiny);
    print_malloc(g_dma->tiny);
    printf("SMALL: %p\n", g_dma->small);
}

/*static void init_large(void)
{
  	g_dma->l = NULL;
	g_dma->l->size = 0;
	g_dma->l->inuse = 0;
	g_dma->l->next = NULL;
    }*/

void init_memory(void)
{
  	if (!g_dma)
	{
	  if ((g_dma = mmap(NULL, HDR_SIZE
                              + PREALOC_SIZE_TINY
                              + PREALOC_SIZE_SMALL,
                              MMAP_FLAGS, OFFSET, 0)) == MAP_FAILED)
	    	exit(0);
	  	init_tiny();
		init_small();
		g_dma->get_tiny = add_tiny;
        //g_dma->get_large = add_large;
       // g_dma->get_small = get_small;
	} 
	  //init_large();
}

void *mallok(size_t size)
{
    pthread_mutex_lock(&mutex);

    if (g_dma == NULL)
        init_memory();

    if (size <= T_PAGE_SIZE)
        return (g_dma->get_tiny(g_dma->tiny, size, g_dma->tiny_limit));
  /*else if (size < S_LIMIT)
    return (g_dma->get_small(g_dma->small, size, g_dma->small_limit));
  else
  return (g_dma->get_large(g_dma->large, size));*/
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void)
{
  //   init_memory();
  char *ptr = mallok(20);
  mallok(21);
  mallok(24);
  mallok(23);
  show_alloc_mem();
  return 0;
}
