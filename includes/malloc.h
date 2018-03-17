#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

#define T_SIZE 8
#define T_PAGE_SIZE (8 * getpagesize())
#define S_SIZE 32
#define S_PAGE_SIZE (32 * getpagesize())

extern pthread_mutex_t mutex;

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

/**
 * @block_addr: stroes info about zones addresses for tiny,
 *              small and large. It include pointer to start
 *              (x_s_addr) and end (x_e_addr) addres
 */
struct block_addr
{
  	char   *t_s_addr;
  	char   *t_e_addr;
  	char   *s_s_addr;
  	char   *s_e_addr;
  	char   *l_s_addr;
};

/**
 * @ovrl_block: stores info about memory zone
 * addr: stores start/end addreses of every zone
 * t: tiny
 * s: small
 * l: large
 */
struct ovrl_block
{
  	struct block_addr *addr;
	struct s_block *s;
  	struct t_block *t;
  	struct l_block *l;
};

extern struct ovrl_block *g_dma;

void init_memory(void);
#endif
