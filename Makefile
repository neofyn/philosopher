# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fyudris <fyudris@student.42berlin.de>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/28 09:08:16 by fyudris           #+#    #+#              #
#    Updated: 2025/11/28 11:49:38 by fyudris          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# === EXECUTABLE NAME ===
NAME            := philo

# === CONFIGURATION ===
CC              := cc
RM              := rm -rf
MKDIR           := mkdir -p

# === DIRECTORIES ===
OBJ_DIR         := obj
SRC_DIR         := srcs
INC_DIR         := includes

# === FLAGS ===
CFLAGS          := -Wall -Wextra -Werror -g -pthread
CPPFLAGS        := -I$(INC_DIR)

# === SOURCE & OBJECT FILES ===
SRCS            := \
                   srcs/main.c \
                   srcs/init.c \
                   srcs/utils.c \
				   srcs/time.c \
				   srcs/routine.c \
				   srcs/monitor.c \
				   srcs/cleanup.c

OBJS            := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# === COLORS ===
GREEN           := \033[0;32m
YELLOW          := \033[0;33m
BLUE_BOLD       := \033[1;34m
RESET           := \033[0m

# === MAIN RULES ===
.DEFAULT_GOAL   := all

all: $(NAME)

$(NAME): $(OBJS)
	@printf "\n$(BLUE_BOLD)Linking -> $(RESET)$(NAME)\n"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@printf "$(GREEN)✓ Philosophers created successfully!$(RESET)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(@D)
	@printf "$(YELLOW)Compiling:$(RESET) %-30s\r" $<
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(YELLOW)Object files cleaned.$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(GREEN)Full clean complete.$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re