#include "lib.h"
#include "arena.h"
#include "constants.h"
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static inline char *concat(const char *s1, const char *s2, arena_t *arena) {
	if (arena == NULL) {
		arena_free(arena);
		exit(1);
	}
	char *new_string = arena_alloc(arena, strlen(s1) + strlen(s2) + 1);
	strcpy(new_string, s1);
	strcat(new_string, s2);
	return new_string;
}

static inline void handle_dir(char *file_name, struct stat *buf,
							  arena_t *arena) {
	DIR *dir = opendir(file_name);
	if (dir == NULL) {
		return;
	}
	struct dirent *dir_ent;
	while ((dir_ent = readdir(dir))) {
		if (strcmp(dir_ent->d_name, ".") != 0 &&
			strcmp(dir_ent->d_name, "..") != 0) {
			char *concat_file_name_slash = concat(file_name, "/", arena);
			char *concat_path =
				concat(concat_file_name_slash, dir_ent->d_name, arena);
			process_file(concat_path, buf);
			arena_reset(arena);
		}
	}
	closedir(dir);
}

static inline int file_is_valid(char *file_name, size_t size, size_t blocks) {
	if (access(file_name, F_OK) != 0) {
		if (silent_flag == false) {
			printf("FILE/DIRECTORY: \"%s\" doesn't exist, check for typos!\n",
				   file_name);
		}
		return 1;
	}
	if (size <= 0) {
		if (silent_flag == false) {
			printf("FILE/DIRECTORY: \"%s\" size in bytes not detected!\n"
				   "This usually happens when you are reading files from "
				   "directories like  /proc or others in the / directory\n",
				   file_name);
		}
		return 1;
	}
	if (blocks <= 0) {
		if (silent_flag == false) {
			printf("FILE/DIRECTORY: \"%s\" amount of blocks not detected!\n"
				   "This usually happens when you are reading files from "
				   "directories like  /proc or others in the / directory\n",
				   file_name);
		}
		return 1;
	}
	return 0;
}

static inline int evaluate_st_mode(struct stat *buf, char *file_name) {
	if (S_ISREG(buf->st_mode) != 0 && S_ISDIR(buf->st_mode) != 0 &&
		S_ISLNK(buf->st_mode) != 0) {
		printf("File: \"%s\" file type is not recognized!\n", file_name);
		return 1;
	}
	return 0;
}

char *truncate_file_name(char file_name[], arena_t *arena) {
	int c = 0;
	int contains_dir = 0;
	while (file_name[c] != '\0') {
		c++;
		if (file_name[c] == '/' && !contains_dir) {
			contains_dir = 1;
		}
	} // after this, file_name[c] should point at the last char of
	  // file_name

	if (!contains_dir) {
		return NULL;
	}

	while (file_name[c] != '/') {
		// find the last occurence of '/' by backtracking
		c--;
	}
	int idx_of_last_slash = c;
	c++; // increment to point after '/'
	int new_str_size = 0;

	while (file_name[c] != '\0') { // determine size of new string
		c++;
		new_str_size++;
	}

	char *new_str = arena_alloc(arena, new_str_size + 1); // + 1 for the '\0'
	for (int i = 0; i < new_str_size; i++) {
		new_str[i] = file_name[i + idx_of_last_slash + 1];
	}
	new_str[new_str_size] = '\0';
	return new_str;
}

void display_file_output_full(char *final_file_name, size_t size) {
	printf("\nFILE: %s\n", final_file_name);
	if (size <= BYTES_IN_KIB) {
		printf("%s IN BYTES: %zuB\n", final_file_name, size);
	} else if (size <= BYTES_IN_MIB) {
		const double size_in_kib = size / BYTES_IN_KIB;

		printf("%s IN BYTES: %zuB\n"
			   "%s IN KiB: %.2fKiB\n",
			   final_file_name, size, final_file_name, size_in_kib);
	} else if (size <= BYTES_IN_GIB) {
		const double size_in_kib = size / BYTES_IN_KIB;
		const double size_in_mib = size / BYTES_IN_MIB;

		printf("%s IN BYTES: %zuB\n"
			   "%s IN KiB: %.2fKiB\n"
			   "%s IN MiB: %.2fMiB\n",
			   final_file_name, size, final_file_name, size_in_kib,
			   final_file_name, size_in_mib);
	} else {
		const double size_in_kib = size / BYTES_IN_KIB;
		const double size_in_mib = size / BYTES_IN_MIB;
		const double size_in_gib = size / BYTES_IN_GIB;

		printf("%s IN BYTES: %zuB\n"
			   "%s IN KiB: %.2fKiB\n"
			   "%s IN MiB: %.2fMiB\n"
			   "%s IN GiB: %.2fGiB\n",
			   final_file_name, size, final_file_name, size_in_kib,
			   final_file_name, size_in_mib, final_file_name, size_in_gib);
	}
}

void display_dir_output(char *final_file_name, size_t size) {
	printf("\nDIRECTORY: %s\n", final_file_name);
	if (size <= BYTES_IN_KIB) {
		printf("%s IN BYTES: %zuB\n", final_file_name, size);
	} else if (size <= BYTES_IN_MIB) {
		const double size_in_kib = size / BYTES_IN_KIB;

		printf("%s IN BYTES: %zuB\n"
			   "%s IN KiB: %.2fKiB\n",
			   final_file_name, size, final_file_name, size_in_kib);
	} else if (size <= BYTES_IN_GIB) {
		const double size_in_kib = size / BYTES_IN_KIB;
		const double size_in_mib = size / BYTES_IN_MIB;

		printf("%s IN BYTES: %zuB\n"
			   "%s IN KiB: %.2fKiB\n"
			   "%s IN MiB: %.2fMiB\n",
			   final_file_name, size, final_file_name, size_in_kib,
			   final_file_name, size_in_mib);
	} else {
		const double size_in_kib = size / BYTES_IN_KIB;
		const double size_in_mib = size / BYTES_IN_MIB;
		const double size_in_gib = size / BYTES_IN_GIB;

		printf("%s IN BYTES: %zuB\n"
			   "%s IN KiB: %.2fKiB\n"
			   "%s IN MiB: %.2fMiB\n"
			   "%s IN GiB: %.2fGiB\n",
			   final_file_name, size, final_file_name, size_in_kib,
			   final_file_name, size_in_mib, final_file_name, size_in_gib);
	}
}

void display_file_output_lesser(char *final_file_name, size_t size) {
	if (size <= BYTES_IN_KIB) {
		printf("%s IN BYTES: %zuB\n", final_file_name, size);
	} else if (size <= BYTES_IN_MIB) {
		const double size_in_kib = size / BYTES_IN_KIB;

		printf("%s IN KiB: %.2fKiB\n", final_file_name, size_in_kib);
	} else if (size <= BYTES_IN_GIB) {
		const double size_in_mib = size / BYTES_IN_MIB;

		printf("%s IN MiB: %.2fMiB\n", final_file_name, size_in_mib);
	} else {
		const double size_in_gib = size / BYTES_IN_GIB;

		printf("%s IN GiB: %.2fGiB\n", final_file_name, size_in_gib);
	}
}

void process_output(char *file_name, size_t size, arena_t *arena) {
	char *final_file_name;
	if (truncate_flag) {
		final_file_name = truncate_file_name(file_name, arena);
		if (final_file_name == NULL) {
			final_file_name = file_name;
		}
	} else
		final_file_name = file_name;

	if (is_dir) {
		display_file_output_lesser(final_file_name, size);
		return;
	} else
		display_file_output_full(final_file_name, size);
}

void remove_last_slash(char *file_name) {
	// if command line arg looks like "dir/"
	int c = strlen(file_name);
	if (file_name[c - 1] == '/' && c > 1)
		file_name[c - 1] = '\0';
}
void process_file(char *file_name, struct stat *buf) {
	arena_t arena;
	arena_init(&arena, 2048);
	lstat(file_name, buf);

	if (file_is_valid(file_name, buf->st_size, buf->st_blocks) == 1) {
		arena_free(&arena);
		return;
	}

	if (evaluate_st_mode(buf, file_name) == 1) {
		arena_free(&arena);
		return;
	}

	if (S_ISDIR(buf->st_mode) == 1) {
		handle_dir(file_name, buf, &arena);
		arena_free(&arena);
		return;
	}
	// st_size stores file size in bytes defined in sys/stat.h
	const size_t size = buf->st_size;
	total_bytes += size;

	if (size >= 1 && silent_flag == false) {
		process_output(file_name, size, &arena);
	}
	arena_free(&arena);
	return;
}
