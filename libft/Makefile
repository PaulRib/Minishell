NAME = libft.a

SRC = 	lst/ft_lstadd_back.c lst/ft_lstadd_front.c lst/ft_lstclear.c lst/ft_lstdelone.c lst/ft_lstlast.c\
		lst/ft_lstiter.c lst/ft_lstmap.c lst/ft_lstnew.c lst/ft_lstsize.c\
		mem/ft_bzero.c mem/ft_calloc.c mem/ft_memchr.c mem/ft_memcmp.c mem/ft_memcpy.c mem/ft_memmove.c\
		mem/ft_memset.c\
		nbr/ft_atoi.c nbr/ft_itoa.c nbr/ft_putnbr_fd.c\
		str/ft_putchar_fd.c str/ft_putendl_fd.c str/ft_putstr_fd.c str/ft_split.c str/ft_strchr.c\
		str/ft_strdup.c str/ft_striteri.c str/ft_strjoin.c str/ft_strlcat.c str/ft_strlcpy.c\
		str/ft_strlen.c str/ft_strmapi.c str/ft_strncmp.c str/ft_strnstr.c str/ft_strrchr.c\
		str/ft_strtrim.c str/ft_substr.c str/ft_tolower.c str/ft_toupper.c str/free_tab.c\
		other/ft_isalnum.c other/ft_isalpha.c other/ft_isascii.c other/ft_isdigit.c other/ft_isprint.c\
		gnl/get_next_line.c gnl/get_next_line_utils.c\
		printf/ft_printf.c printf/ft_printfutils.c printf/ft_printhexa.c\

OBJS = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror

CC = gcc

all: $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
