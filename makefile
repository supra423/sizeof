# source files
SRC_DIR := src

# change if you want a different binary name
FINAL_BIN_NAME := sizeof

# output dirs
OBJ_DIR := obj
BIN_DIR := bin

OBJ_DIR_DEBUG := obj/debug
OBJ_DIR_RELEASE := obj/release
OBJ_DIR_MAX := obj/max

BIN_DIR_DEBUG := bin/debug
BIN_DIR_RELEASE := bin/release
BIN_DIR_MAX := bin/max

# compiler gcc clang
CC := gcc

# flags
CFLAGS_DEBUG := -Iinclude -Wall -Wextra -Winline -g
CFLAGS_RELEASE := -Iinclude -O2 -pipe
CFLAGS_MAX := -Iinclude -O2 -march=native -mtune=native -fomit-frame-pointer \
   -fno-plt -pipe

LDFLAGS_DEBUG := 
LDFLAGS_RELEASE := 
LDFLAGS_MAX := 

# PKG_CFLAGS := $(shell pkg-config --cflags libfoo)
# PKG_LIBS := $(shell pkg-config --libs libfoo)

SRCS := $(wildcard $(SRC_DIR)/*.c)

OBJS_DEBUG := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR_DEBUG)/%.o, $(SRCS))
OBJS_RELEASE := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR_RELEASE)/%.o, $(SRCS))
OBJS_MAX := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR_MAX)/%.o, $(SRCS))

TARGET_DEBUG := $(BIN_DIR_DEBUG)/$(FINAL_BIN_NAME)
TARGET_RELEASE := $(BIN_DIR_RELEASE)/$(FINAL_BIN_NAME)
TARGET_MAX := $(BIN_DIR_MAX)/$(FINAL_BIN_NAME)

all: debug release max

debug: $(TARGET_DEBUG)
release: $(TARGET_RELEASE)
max: $(TARGET_MAX)

# keywords: link ld gcc ldflag
$(TARGET_DEBUG): $(OBJS_DEBUG) | $(BIN_DIR_DEBUG)
	$(CC) $(LDFLAGS_DEBUG) $(OBJS_DEBUG) -o $@
	@echo "Build complete: $@"
	@echo ""
$(TARGET_RELEASE): $(OBJS_RELEASE) | $(BIN_DIR_RELEASE)
	$(CC) $(LDFLAGS_RELEASE) $(OBJS_RELEASE) -o $@
	@echo "Build complete: $@"
	@echo ""
$(TARGET_MAX): $(OBJS_MAX) | $(BIN_DIR_MAX)
	$(CC) $(LDFLAGS_MAX) $(OBJS_MAX) -o $@
	@echo "Build complete: $@"
	@echo ""

# keywords: compile cc gcc cflag cflags clang obj object
$(OBJ_DIR_DEBUG)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR_DEBUG)
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@
	@echo "Compiled $<"
$(OBJ_DIR_RELEASE)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR_RELEASE)
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@
	@echo "Compiled $<"
$(OBJ_DIR_MAX)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR_MAX)
	$(CC) $(CFLAGS_MAX) -c $< -o $@
	@echo "Compiled $<"

# keywords: dirs mkdir directory
$(OBJ_DIR_DEBUG) $(BIN_DIR_DEBUG):
	mkdir -p $@
$(OBJ_DIR_RELEASE) $(BIN_DIR_RELEASE):
	mkdir -p $@
$(OBJ_DIR_MAX) $(BIN_DIR_MAX):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned up build files."
