#include "../includes/malloc.h"

struct ovrl_block *g_dma = NULL;

void init_tiny(void)
{
  	g_dma->t = (char*)(g_dma + 1);

	g_dma->t_s_addr = (char*)(g_dma + 1);
	g_dma->t_e_addr = (char*)(g_dma + T_PAGE_SIZE + 1);

  	g_dma->t->size = 0;
  	g_dma->t->inuse = 0;
	g_dma->t->next = NULL;
}

void init_small(void)
{
  	g_dma->s = (char*)(g_dma->t_e_addr + 1);

	g_dma->s_s_addr = (char*)(g_dma->s + 1);
	g_dma->s_e_addr = (char*)(g_dma + S_PAGE_SIZE + 1);

	g_dma->s->size = 0;
	g_dma->s->inuse = 0;
	g_dma->s->next = NULL;
}

void init_memory(void)
{
  	if (!g_dma)
	{
	  g_dma = mmap(NULL, sizeof(struct ovrl_block)
		       + (T_PAGE_SIZE * 100)
		       + (S_PAGE_SIZE * 100),
		       PROT_READ | PROT_WRITE, MAP_ANON, -1, 0);
	  init_tiny();
	  init_small();
	  inti_large();
	}
}
