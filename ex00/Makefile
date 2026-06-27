NAME=btc
OBJS=src/main.o src/BitcoinExchange.o
INCLUDE=include/BitcoinExchange.hpp
CXX=c++
CXXFLAGS=-Wall -Wextra -Werror -std=c++98 -Iinclude
GREEN = \033[32m
YELLOW = \033[33m
RESET = \033[0m
RED = \033[31m

all: $(NAME)
	@echo Execute o Programa: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@printf "$(GREEN)[✓] Compilado: $(NAME)$(RESET)\n"

$(OBJS): $(INCLUDE)

clean:
	rm -f $(OBJS)
	rm -rf objs
	@printf "$(RED)Objectos eliminados$(RESET)\n"

fclean: clean
	@rm -rf $(NAME)

re: fclean  all

.PHONY: all clean fclean re