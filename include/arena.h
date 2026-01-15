#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>

typedef struct {
	char *buf;
	size_t max_size;
	size_t used;
} arena_t;
void arena_init(arena_t *arena, size_t initial_size);
char *arena_alloc(arena_t *arena, size_t bytes);
void arena_reset(arena_t *arena);
void arena_free(arena_t *arena);

#endif
