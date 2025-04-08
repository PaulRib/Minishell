NAME = minishell

SRC = src/minishell.c src/env.c parsing/parsing.c src/struct.c \
		parsing/token.c parsing/word.c builtin/history.c parsing/options.c parsing/quotes.c\
		parsing/quotes_utils.c

OBJS = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror -g

CC = cc

LIBFT = libft/libft.a

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline -lncurses > /dev/null
	@echo "\n$(CYAN)"
	@echo "███╗   ███╗██╗███╗  ██╗██╗ ██████╗██╗  ██╗███████╗██╗     ██╗     "
	@echo "████╗ ████║██║████╗ ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     "
	@echo "██╔████╔██║██║██╔██╗██║██║╚█████╗ ███████║█████╗  ██║     ██║     "
	@echo "██║╚██╔╝██║██║██║╚████║██║ ╚═══██╗██╔══██║██╔══╝  ██║     ██║     "
	@echo "██║ ╚═╝ ██║██║██║ ╚███║██║██████╔╝██║  ██║███████╗███████╗███████╗"
	@echo "╚═╝     ╚═╝╚═╝╚═╝  ╚══╝╚═╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
	@echo "																	 "
	@echo "██╗ ██████╗  ██████╗ ███████╗ █████╗ ██████╗ ██╗   ██╗  ██╗"
	@echo "██║██╔════╝  ██╔══██╗██╔════╝██╔══██╗██╔══██╗╚██╗ ██╔╝  ██║"
	@echo "██║╚█████╗   ██████╔╝█████╗  ███████║██║  ██║ ╚████╔╝   ██║"
	@echo "██║ ╚═══██╗  ██╔══██╗██╔══╝  ██╔══██║██║  ██║  ╚██╔╝    ╚═╝"
	@echo "██║██████╔╝  ██║  ██║███████╗██║  ██║██████╔╝   ██║     ██╗"
	@echo "╚═╝╚═════╝   ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝    ╚═╝     ╚═╝"
	@echo "$(RESET)\n"


$(LIBFT):
	@make all -C libft > /dev/null
	@echo "\n$(GREEN)Libft compiled successfully! 📚\n"

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@ > /dev/null
	@echo "\n$(MAGENTA)========================================"
	@echo "      Minishell is compiling... 🛠️		"
	@echo "========================================\n$(RESET)"

all: $(NAME)

clean:
	@rm -rf $(OBJS) > /dev/null
	@make clean -C libft > /dev/null
	@echo "\n$(YELLOW)Cleaning up... 🧹\n"

fclean: clean
	@rm -rf $(NAME) > /dev/null
	@make fclean -C libft > /dev/null
	@echo "\n$(RED)Deleting EVERYTHING! 🗑️\n"

re: fclean all

.PHONY: all bonus clean fclean re

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
MAGENTA = \033[1;35m
CYAN = \033[1;36m
