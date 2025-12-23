# ---- Makefile for the shell project ----

# Compiler and flags
CC      := gcc
CFLAGS  := -g -std=c99 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 \
           -Wall -Wextra -Werror -Wno-unused-parameter

# Source and build directories
SRC_DIR := src
OBJ_DIR := obj
BIN     := shell.out

# List of source files
SRC := main.c handle_fg_bg.c globals.c log_function.c reveal_func.c \
       hop_function.c check_foreground.c input_parser.c file_redirection.c \
       split_pipe.c redirect_stdin.c prompt_username.c
SRCS := $(addprefix $(SRC_DIR)/,$(SRC))

# Object files will live in obj/
OBJS := $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

# Default target
all: $(BIN)

# Link the final executable
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile each .c into obj/.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create obj directory if missing
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Convenience targets
clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
