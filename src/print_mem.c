#include "../includes/malloc.h"

void ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
{
	if (n >= 0)
		n = -n;
	else
		ft_putchar('-');
	if (n < -9)
		ft_putnbr(-(n / 10));
	ft_putchar(-(n % 10) + 48);
}
static void	print_hex(long hex, int prefix)
{
	int	i;

	if (prefix)
		write(1, "0x", 2);
	if (hex >= 16)
		print_hex(hex / 16, 0);
	i = hex % 16;
	if (i >= 10)
		ft_putchar(i - 10 + 'a');
	else
		ft_putchar(i + '0');
}

long small_allocation_print(struct s_block *node)
{
	long size;

	if (node == NULL)
		return 0;

	print_hex((long)(node + 1), 1);
	write(1, " - ", 3);
	print_hex((long)(node->blck_limit), 1);
	write(1, " : ", 3);
	size = node->size;
	ft_putnbr(size);
	write(1, " octets\n", 8);

	return (size);
}

long start_print_small(struct s_block *node)
{
	long total;

	total = 0;

	while (node->next)
	{
		total += small_allocation_print(node->next);
		node = node->next;
	}
	return (total);
}


long large_allocation_print(struct l_block *node)
{
	long size;

	if (node == NULL)
		return 0;

	print_hex((long)(node + 1), 1);
	write(1, " - ", 3);
	print_hex((long)(node->blck_limit), 1);
	write(1, " : ", 3);
	size = node->size;
	ft_putnbr(size);
	write(1, " octets\n", 8);

	return (size);
}

long start_print_large(struct l_block *node)
{
	long total;

	total = 0;
	while (node->next)
	{
		total += large_allocation_print(node->next);
		node = node->next;
	}
	return (total);
}

long tiny_allocation_print(struct t_block *node)
{
	long size;

	if (node == NULL)
		return 0;

	print_hex((long)(node + 1), 1);
	write(1, " - ", 3);
	print_hex((long)(node->blck_limit), 1);
	write(1, " : ", 3);
	size = node->size;
	ft_putnbr(size);
	write(1, " octets\n", 8);

	return (size);
}

long start_print_tiny(struct t_block *node)
{
	long total;

	total = 0;

	while (node->next)
	{
		total += tiny_allocation_print(node->next);
		node = node->next;
	}
	return (total);
}

void show_alloc_mem(void)
{
	pthread_mutex_lock(&mutex);
	if (!g_dma)
	{
		write(1, "show mem error: Not initialized\n", 32);
		return;
	}
	write(1, "TINY : ", 7);
	print_hex((long)g_dma->tiny, 1);
	write(1, "\n", 1);
	g_dma->print_block[TINY](g_dma->tiny);

	write(1, "SMALL : ", 8);
	print_hex((long)g_dma->small, 1);
	write(1, "\n", 1);
	g_dma->print_block[SMALL](g_dma->small);

	write(1, "LARGE : ", 8);
	print_hex((long)g_dma->large, 1);
	write(1, "\n", 1);
	g_dma->print_block[LARGE](g_dma->large);
	pthread_mutex_unlock(&mutex);
}
