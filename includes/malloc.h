#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>

#define T_SIZE 2
#define PREALOC_SIZE_TINY (getpagesize() * 100 * T_SIZE)
#define T_PAGE_SIZE (2 * getpagesize()) // 32768
#define T_LIMIT (getpagesize() * 100 * T_SIZE)

#define S_SIZE 16
#define PREALOC_SIZE_SMALL (getpagesize() * 100 * S_SIZE)
#define S_PAGE_SIZE (16 * getpagesize()) //131 078
#define S_LIMIT (getpagesize() * 100 * S_SIZE)


#define HDR_SIZE (sizeof(struct ovrl_block))
#define MMAP_FLAGS PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE
#define OFFSET -1
#define HDR_OFFSET 1

#define IN_PAGE(NODE, S) ((char *)NODE->next - (char *)NODE->blck_limit) < S

/**
 * tiny_block
 */
struct t_block
{
    size_t size;
	struct t_block *next;
  	struct t_block *blck_limit;
};


/**
 * small block
 */
struct s_block
{
    size_t size;
	struct s_block *next;
  	struct s_block *blck_limit;
};


/**
 * large block
 */
struct l_block
{
    size_t size;
  	struct l_block *next;
  	struct l_block *blck_limit;
};

/**
 * @block_addr: stroes info about zones addresses for tiny,
 *              small and large. It include pointer to start
 *              used for alloc_mem_show()
 */
struct block_addr
{
  	char   *tiny_start;
  	char   *small_start;
  	char   *large_start;
};

/**
 * @ovrl_block: stores info about memory zone
 *
 * addr: stores start/end addreses of every zone
 * tiny: tiny based on dbl linked list
 * small: small based on dbl linked list
 * large: large based on dbl linked list
 */
struct ovrl_block
{
  	struct t_block *tiny;
  	size_t tiny_limit;
	struct s_block *small;
  	size_t small_limit;
  	struct l_block *large;
  	struct block_addr addr;

  	void *(*get_tiny)(struct t_block* , size_t, size_t);
	void *(*get_small)(struct s_block* , size_t, size_t);
    void *(*get_large)(size_t);
};




extern struct ovrl_block *g_dma;
extern pthread_mutex_t mutex;

void *mallok(size_t size);
void init_memory(void);
void *push_tiny_chunk(struct t_block *tiny_head, size_t size, size_t limit);
void *push_small_chunk(struct s_block *small_head, size_t size, size_t limit);
void *push_large_chunk(size_t size);
void show_alloc_mem(void);
long print_malloc(struct t_block *node);
long print_alloc(struct t_block *node);

#endif
