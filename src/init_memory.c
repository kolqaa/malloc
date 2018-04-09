#include "../includes/malloc.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct ovrl_block *g_dma = NULL;

static void init_tiny(void)
{
  	g_dma->t = (struct t_block *)(g_dma + 1);

	g_dma->addr = (struct block_addr*)(g_dma + 1);
	g_dma->addr->t_s_addr = (char *)(g_dma + 1);
	g_dma->addr->t_e_addr = (char *)(g_dma + T_PAGE_SIZE + 1);

  	g_dma->t->size = 0;
  	g_dma->t->inuse = 0;
	g_dma->t->next = NULL;
}

static void init_small(void)
{
  	g_dma->s = (struct s_block*)(g_dma->addr->t_e_addr + 1);

	g_dma->addr->s_s_addr = (char*)(g_dma->s + 1);
	g_dma->addr->s_e_addr = (char*)(g_dma + S_PAGE_SIZE + 1);

	g_dma->s->size = 0;
	g_dma->s->inuse = 0;
	g_dma->s->next = NULL;
}

static void init_large(void)
{
  	g_dma->l = NULL;
	g_dma->l->size = 0;
	g_dma->l->inuse = 0;
	g_dma->l->next = NULL;
}

void init_memory(void)
{
	const size_t SIZE = (sizeof(struct ovrl_block) + T_PAGE_SIZE + S_PAGE_SIZE) * 200;
  	if (!g_dma)
	{
		if ((g_dma = mmap(NULL, SIZE,
						  PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		{
			exit(0);
		}
		init_tiny();
	    //init_small();
	  }
	  //init_large();
}
