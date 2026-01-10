TARGET := sizeof
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

CC := gcc
CFLAGS := -Iinclude -Wall -Wextra -g
LDFLAGS := 
# CFLAGS := -Iinclude -march=native -Wall -Wextra -O2 -fomit-frame-pointer \
# 	   -fno-plt -pipe -mtune=native
# CFLAGS := -Iinclude -O2 -Wall -Wextra -pipe

# PKG_CFLAGS := $(shell pkg-config --cflags libfoo)
# PKG_LIBS := $(shell pkg-config --libs libfoo)

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	@echo "Build complete: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $<"

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned up build files."

run: all
	@echo "Running program..."
	@./$(BIN_DIR)/$(TARGET)
