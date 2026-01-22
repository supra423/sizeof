#include "arena.h"
#include <stdio.h>
#include <stdlib.h>

void arena_init(arena_t *arena, size_t initial_size) {
	arena->buf = malloc(initial_size);
	if (!arena->buf) {
		printf("There was a problem while allocating memory!\n");
		exit(1);
	}
	arena->max_size = initial_size;
	arena->used = 0;
}

char *arena_alloc(arena_t *arena, size_t bytes) {
	if (arena->used + bytes > arena->max_size) {
		size_t new_size = arena->max_size * 2;
		while (arena->used + bytes > new_size)
			new_size *= 2;
		char *new_buf = realloc(arena->buf, new_size);
		if (!new_buf) {
			free(arena->buf);
			exit(1);
		}
		arena->buf = new_buf;
		arena->max_size = new_size;
	}
	char *new_free_space = arena->buf + arena->used;
	arena->used += bytes;
	return new_free_space;
}

void arena_reset(arena_t *arena) { arena->used = 0; }

void arena_free(arena_t *arena) {
	free(arena->buf);
	arena->buf = NULL;
	arena->max_size = arena->used = 0;
}
