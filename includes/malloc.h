#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>

#define TINY_MAX (size_t)(getpagesize() / 4)
#define TINY_ZONE 100 * TINY_MAX

#define SMALL_MAX (size_t)(getpagesize() * 16)
#define SMALL_ZONE 100 * SMALL_MAX

#define HDR_SIZE (sizeof(struct ovrl_block))
#define MMAP_FLAGS PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE
#define OFFSET -1
#define HDR_OFFSET 1

#define IN_PAGE(NODE, S) (((char *)NODE->next - (char *)NODE->blck_limit) < S)

#define BLOCK_TOTAL 3

enum BLOCK_TYPE
{
    TINY,
    SMALL,
    LARGE
};

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

    void *(*get_block[BLOCK_TOTAL])();
    long (*print_block[BLOCK_TOTAL])();
};

extern struct ovrl_block *g_dma;
extern pthread_mutex_t mutex;

void init_memory(void);

/* allocation function */
void *mallok(size_t size);
void *push_tiny_chunk(struct t_block *tiny_head, size_t size, size_t limit);
void *push_small_chunk(struct s_block *small_head, size_t size, size_t limit);
void *push_large_chunk(size_t size);

/* memory printing function */
void show_alloc_mem(void);
long start_print_tiny(struct t_block *node);
long start_print_small(struct s_block *node);
long start_print_large(struct l_block *node);

/* block reallocation function */
void    *realloc(void *ptr, size_t size);
void	*realloc_tiny(struct t_block *tiny_head, void *ptr, size_t size);
void	*realloc_small(struct s_block *small_head, void *ptr, size_t size);
void	*realloc_large(struct l_block *large_head, void *ptr, size_t size);
void free(void *ptr);

#endif
