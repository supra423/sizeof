#include "constants.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char *truncate_file_name(char file_name[]) {
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

	char *new_str = malloc(new_str_size + 1); // + 1 for the '\0'
	for (int i = 0; i < new_str_size; i++) {
		new_str[i] = file_name[i + idx_of_last_slash + 1];
	}
	new_str[new_str_size] = '\0';
	return new_str;
}

int evaluate_st_mode(struct stat buf, char *file_name) {
	if (S_ISREG(buf.st_mode) != 0 && S_ISDIR(buf.st_mode) != 0 &&
		S_ISLNK(buf.st_mode) != 0) {
		printf("File: \"%s\" file type is not recognized!\n", file_name);
		return 1;
	}
	return 0;
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

void process_output(char *file_name, size_t size, int truncate_flag,
					int is_dir) {
	int is_mallocd = 0;
	char *final_file_name;
	if (truncate_flag) {
		final_file_name = truncate_file_name(file_name);
		is_mallocd = 1;
		if (final_file_name == NULL) {
			final_file_name = file_name;
			is_mallocd = 0;
		}
	} else
		final_file_name = file_name;

	if (is_dir) {
		display_file_output_lesser(final_file_name, size);
		if (is_mallocd)
			free(final_file_name);
		return;
	} else
		display_file_output_full(final_file_name, size);

	if (is_mallocd)
		free(final_file_name);
}

char *concat(const char *s1, const char *s2) {
	char *result = malloc(strlen(s1) + strlen(s2) + 1);
	if (result == NULL) {
		exit(1);
	}
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}
