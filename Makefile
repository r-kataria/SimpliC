# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -pthread -g

# Directories
SRC_DIR = src
OBJ_DIR = build
INCLUDE_DIR = include
CONFIG_DIR = config

# Source Files
SRC = $(wildcard $(SRC_DIR)/*.c)

# Object Files
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Executable
EXEC = $(OBJ_DIR)/simpliC

# Default Target
all: $(EXEC)

# Create build directory if it doesn't exist
$(EXEC): $(OBJ) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean Build Artifacts
clean:
	rm -rf $(OBJ_DIR)

# Phony Targets
.PHONY: all clean
