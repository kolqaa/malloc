#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

#define T_SIZE 8
#define T_PAGE_SIZE (8 * getpagesize())
#define T_LIMIT (getpagesize() * 100 * T_SIZE)

#define S_SIZE 32
#define S_PAGE_SIZE (32 * getpagesize())
#define S_LIMIT (getpagesize() * 100 * S_SIZE)


#define MMAP_FLAGS PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE
#define OFFSET -1

extern pthread_mutex_t mutex;

struct t_block
{
	size_t size;
	size_t inuse;
	struct t_block *next;
    struct t_block *blck_limit;
};

struct s_block
{
	size_t size;
	size_t inuse;
	struct s_block *next;
    struct s_block *blck_limit;
};


struct l_block
{
  	size_t size;
  	size_t inuse;
  	struct l_block *next;
    struct l_block *blck_limit;
};

/**
 * @block_addr: stroes info about zones addresses for tiny,
 *              small and large. It include pointer to start
 *              (x_s_addr) and end (x_e_addr) addres
 */
struct block_addr
{
  	char   *tiny_start;
  	char   *small_start;
  	char   *large_start;
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
  	struct t_block *tiny;
    size_t tiny_limit;
	struct s_block *small;
    size_t small_limit;
  	struct l_block *large;
  	struct block_addr addr;
};

extern struct ovrl_block *g_dma;

void init_memory(void);
#endif
