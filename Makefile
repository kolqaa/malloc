
# Check env
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# Library
NAME 	= 	libft_malloc_$(HOSTTYPE).so
LINK	=	libft_malloc.so

# Compilation
CC	= gcc
CFLAGS	=	-Wall -Wextra -Werror

# Directories
SRCDIR	=	src
OBJDIR	= 	objs
INCDIR	=	includes

# Files && Objs

FILES 	=	free			\
		init_memory		\
		malloc			\
		realloc			\
		print_mem	\
		chunck_manage \
		block_reallocation

OBJ	:= 	$(addsuffix .o, $(FILES))

# Paths foreach
OBJP 	=	$(addprefix $(OBJDIR)/, $(OBJ))

# Main rules
all			: 	$(OBJDIR) $(NAME)

re			: 	fclean all

# Compilation rules
$(OBJDIR)		:
					@mkdir -p $@ 2>&-

$(NAME)			: 	$(OBJP)
				@ar rc $@ $^
				@ranlib $@
				@$(CC) $(CFLAGS) -shared -o $@ $^
				@ln -s $(NAME) $(LINK)

$(OBJDIR)/%.o	:	$(SRCDIR)/%.c
					@$(CC) $(CFLAGS) -c $< -o $@ -I$(INCDIR)

# Clean rules
clean			:
					@rm -rf $(OBJDIR)

fclean			: 	clean
					@rm -f $(NAME)
					@rm -f $(LINK)
