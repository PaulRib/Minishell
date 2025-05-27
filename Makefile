NAME = minishell

SRC = src/minishell.c src/env.c parsing/parser.c src/builtin.c src/signals.c src/signals2.c src/sigint_status.c\
		parsing/token.c parsing/parser_utils.c builtin/history.c parsing/options.c parsing/quotes.c\
		parsing/quotes_utils.c parsing/expand.c parsing/expand_utils.c parsing/syntax.c parsing/join_quotes.c\
		exec/exec_fd.c exec/exec_hub.c exec/exec_utils.c exec/exec.c exec/heredoc_process.c exec/heredoc_utils.c\
		exec/heredoc.c exec/pipe_utils.c exec/pipeline_executor.c exec/heredoc_warning_msg.c\
		builtin/cd.c builtin/cd2.c builtin/pwd.c builtin/echo.c builtin/unset.c builtin/export.c builtin/export2.c builtin/export_utils.c builtin/ft_exit.c\
		utils/ft_add_env_var.c utils/ft_find_env_var.c utils/ft_get_env.c utils/ft_update_env_var.c utils/free_all.c utils/msg_errors.c\

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