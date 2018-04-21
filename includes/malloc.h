#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <errno.h>

#define T_SIZE 8
#define PREALOC_SIZE_TINY (getpagesize() * 100 * T_SIZE)
#define T_PAGE_SIZE (8 * getpagesize())
#define T_LIMIT (getpagesize() * 100 * T_SIZE)

#define S_SIZE 32
#define PREALOC_SIZE_SMALL (getpagesize() * 100 * S_SIZE)
#define S_PAGE_SIZE (32 * getpagesize())
#define S_LIMIT (getpagesize() * 100 * S_SIZE)

#define HDR_SIZE (sizeof(struct ovrl_block))
#define MMAP_FLAGS PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE
#define OFFSET -1

enum  CUNK_TYPE {TINY, SMALL};

#define ADD_CHUNK_TO_DMA(TYPE, NODE, TMP_NODE, CAST_TYPE) \
({\
    TMP_NODE = NODE->next; \
    NODE->next = NODE->blck_limit; \
    NODE->next->blck_limit = CAST_TYPE(char*)NODE->next + size; \
    NODE->next->next = TMP_NODE; \
    NODE->next->size = size; \
    NODE->next->inuse = 1; \
})

#define CHECK_MEM_OVERFLOW(NODE, HEAD, SIZE, MAX) \
({\
   ((char*)NODE->blck_limit + SIZE > (char*)HEAD + MAX) ? (1) : (0); \
})

#define FIND_FREED_BLOCK(NODE) \
({\
	while (NODE->next != NULL) \
	{ \
		if ((char *)NODE->next - (char *)NODE->blck_limit >= size) \
			break; \
		NODE = NODE->next;\
	} \
})

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

  	void *(*get_tiny)(struct t_block* , size_t, size_t);
	void *(*get_small)(struct s_block* , size_t, size_t);
  //void *(*get_large)(struct l_block* , size_t, size_t);
};

extern struct ovrl_block *g_dma;
extern pthread_mutex_t mutex;

void init_memory(void);
#endif
