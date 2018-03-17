#include "../includes/malloc.h"

struct ovrl_block *g_dma = NULL;

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
	}
}
