/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimonov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/27 16:26:04 by nsimonov          #+#    #+#             */
/*   Updated: 2018/05/27 17:23:52 by nsimonov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <sys/mman.h>
# include <pthread.h>
# include <errno.h>

# define TINY_MAX (size_t)(getpagesize() / 4)
# define TINY_ZONE 100 * TINY_MAX
# define SMALL_MAX (size_t)(getpagesize() * 16)
# define SMALL_ZONE 100 * SMALL_MAX

# define HDR_SIZE (sizeof(struct s_block))
# define MMAP_FLAGS PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE
# define OFFSET -1
# define HDR_OFFSET 1

# define IN_PAGE(NODE, S) (((char *)NODE->next - (char *)NODE->blck_limit) < S)

# define BLOCK_TOTAL 3

enum					e_block_type
{
	TINY,
	SMALL,
	LARGE
};

struct					s_t_block
{
	size_t				size;
	struct s_t_block	*next;
	struct s_t_block	*blck_limit;
};

struct					s_s_block
{
	size_t				size;
	struct s_s_block	*next;
	struct s_s_block	*blck_limit;
};

struct					s_l_block
{
	size_t				size;
	struct s_l_block	*next;
	struct s_l_block	*blck_limit;
};

struct					s_block_addr
{
	char				*tiny_start;
	char				*small_start;
	char				*large_start;
};

struct					s_block
{
	struct s_t_block	*tiny;
	size_t				tiny_limit;
	struct s_s_block	*small;
	size_t				small_limit;
	struct s_l_block	*large;
	struct s_block_addr	addr;
	void				*(*get_block[BLOCK_TOTAL])();
	long				(*print_block[BLOCK_TOTAL])();
};

extern struct s_block	*g_dma;
extern pthread_mutex_t	g_mutex;

void					init_memory(void);

void					*malloc(size_t size);
void					*push_tiny_chunk(struct s_t_block *tiny_head,
							size_t size, size_t limit);
void					*push_small_chunk(struct s_s_block *small_head,
							size_t size, size_t limit);
void					*push_large_chunk(size_t size);

void					show_alloc_mem(void);
long					start_print_tiny(struct s_t_block *node);
long					start_print_small(struct s_s_block *node);
long					start_print_large(struct s_l_block *node);

void					*realloc(void *ptr, size_t size);
void					*realloc_tiny(struct s_t_block *tiny_head,
							void *ptr, size_t size);
void					*realloc_small(struct s_s_block *small_head,
							void *ptr, size_t size);
void					*realloc_large(struct s_l_block *large_head,
							void *ptr, size_t size);
void					free(void *ptr);
void					check_tiny_size(size_t *size,
							struct s_t_block *tiny_node);
void					check_small_size(size_t *size,
							struct s_s_block *small_node);
void					check_large_size(size_t *size,
							struct s_l_block *large_node);
void					free_large(void *ptr);
int						try_free_tiny(void *ptr);
int						try_free_small(void *ptr);
void					print_hex(long hex, int prefix);
void					ft_putnbr(int n);
void					ft_putchar(char c);
long					tiny_allocation_print(struct s_t_block *node);
long					small_allocation_print(struct s_s_block *node);

#endif
