# === EXECUTABLE NAMES ===
NAME        := philo
TESTER_NAME := philo_tester

# === CONFIGURATION ===
CC          := cc
RM          := rm -f
MKDIR       := mkdir -p

# === DIRECTORIES ===
OBJ_DIR     := obj
INC_DIR     := includes
# Directories for the main program source files
SRC_DIRS    := srcs srcs/init srcs/utils
# Directory for the test source files
TEST_DIRS   := tests

# === FLAGS ===
CFLAGS      := -Wall -Wextra -Werror -g -pthread
CPPFLAGS    := -I$(INC_DIR)

# === SOURCE FILES ===
# CRITICAL FIX: Tell 'make' to look for .c files in BOTH source and test directories.
vpath %.c $(SRC_DIRS) $(TEST_DIRS)

# Files shared by both the main program and the tester
COMMON_SRCS := init.c utils.c ft_atoi.c cleanup.c

# Source files specific to each build
MAIN_SRCS   := main.c
TESTER_SRCS := test_main.c test_init.c

# === OBJECT FILES ===
COMMON_OBJS := $(addprefix $(OBJ_DIR)/, $(COMMON_SRCS:.c=.o))
MAIN_OBJS   := $(addprefix $(OBJ_DIR)/, $(MAIN_SRCS:.c=.o))
TESTER_OBJS := $(addprefix $(OBJ_DIR)/, $(TESTER_SRCS:.c=.o))

# --- COLORS (Omitted for brevity) ---

# === MAIN RULES ===
.DEFAULT_GOAL := all
all: $(NAME)

# --- TARGETS ---
$(NAME): $(COMMON_OBJS) $(MAIN_OBJS)
	@printf "\033[1;34mLinking executable:\033[0m $(NAME)\n"
	@$(CC) $(COMMON_OBJS) $(MAIN_OBJS) -o $(NAME) $(CFLAGS)
	@printf "\033[0;32m✓ Project '$(NAME)' created successfully!\033[0m\n"

test: $(COMMON_OBJS) $(TESTER_OBJS)
	@printf "\033[1;34mLinking TEST executable:\033[0m $(TESTER_NAME)\n"
	@$(CC) $(COMMON_OBJS) $(TESTER_OBJS) -o $(TESTER_NAME) $(CFLAGS)
	@printf "\033[0;32m✓ Tester '$(TESTER_NAME)' created successfully!\033[0m\n"
	@printf "\033[0;33m--- Running tests ---\033[0m\n"
	@./$(TESTER_NAME)

# --- COMPILATION RULE ---
$(OBJ_DIR)/%.o: %.c
	@$(MKDIR) -p $(OBJ_DIR)
	@printf "\033[0;33mCompiling:\033[0m $<\n"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# --- CLEAN & OTHER RULES ---
clean:
	@printf "\033[0;33mCleaning object files...\033[0m\n"
	@$(RM) -rf $(OBJ_DIR)

fclean: clean
	@printf "\033[0;32mCleaning executables...\033[0m\n"
	@$(RM) -f $(NAME) $(TESTER_NAME)

re:
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) all --no-print-directory

norm:
	@printf "\033[1;34mChecking Norm compliance...\033[0m\n"
	@norminette $(INC_DIR) $(SRC_DIRS) $(TEST_DIRS)

.PHONY: all test clean fclean re norm