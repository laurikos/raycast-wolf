# Default compiler
CC ?= gcc

# Compiler flags
CFLAGS ?= -Wall -Wextra -pedantic -g -std=c11

# CFLAGS += -fsanitize=address
# LDFLAGS += -fsanitize=address

LDFLAGS += -lSDL2 -lSDL2_image -lm

# Use lld if clang is the compiler
ifeq ($(CC),clang)
    LDFLAGS += -fuse-ld=lld
endif

# Define the binary target and directories
TARGET := bin/raycast
SRC_DIR := src
BIN_DIR := bin

# List of source files
SRCS := $(wildcard $(SRC_DIR)/*.c)

# List of header files (not needed for compilation rules but good for dependencies)
HEADERS := $(wildcard $(SRC_DIR)/*.h)

# Generate a list of object files by replacing the source directory with the binary directory
# and changing the file extension from .c to .o
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Rule to link the program
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule to compile the source files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
