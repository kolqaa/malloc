#ifndef MALLOC_H
#define MALLOC_H
#define T_SIZE 8
#define T_PAGE_SIZE (8 * getpagesize())
#define S_SIZE 32
#define S_PAGE_SIZE (32 * getpagesize());

struct t_block
{
	size_t size;
	size_t inuse;
	struct t_block *next;
};

struct s_block 
{
	size_t size;
	size_t inuse;
	struct s_block *next;
};


struct l_block
{
  	size_t size;
  	size_t inuse;
  	struct l_block *next;
};

struct block_addr
{
  	char   *t_s_addr;
  	char   *t_e_addr;
  	char   *s_s_addr;
  	char   *t_e_addr;
  	char   *l_s_addr;
};

struct ovrl_block
{
	/* memory addressed start - end */
  	struct block_addr *addr;
  	/* memory block */
	struct s_block *s;
  	struct t_block *t;
};

extern ovrl_block *g_dma;

#endi
