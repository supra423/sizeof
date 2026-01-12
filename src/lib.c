#include "constants.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char *truncate_file_name(char filename[]) {
	int c = 0;
	bool contains_dir = false;
	while (filename[c] != '\0') {
		c++;
		if (filename[c] == '/' && !contains_dir) {
			contains_dir = true;
		}
	} // after this, filename[c] should point at the last char of
	  // filename

	if (!contains_dir) {
		return NULL;
	}

	while (filename[c] != '/') {
		// find the last occurence of '/' by backtracking
		c--;
	}
	int idx_of_last_slash = c;
	c++; // increment to point after '/'
	int new_str_size = 0;

	while (filename[c] != '\0') { // determine size of new string
		c++;
		new_str_size++;
	}

	char *new_str = malloc(new_str_size + 1); // + 1 for the '\0'
	for (int i = 0; i < new_str_size; i++) {
		new_str[i] = filename[i + idx_of_last_slash + 1];
	}
	new_str[new_str_size] = '\0';
	return new_str;
}

void display_output(char *final_file_name, double size) {
	printf("\nFILE: %s\n", final_file_name);
	if (size <= BYTES_IN_KIB) {
		printf("%s IN BYTES: %.0fB\n", final_file_name, size);
	} else if (size <= BYTES_IN_MIB) {
		const double size_in_kib = size / BYTES_IN_KIB;

		printf("%s IN BYTES: %.0fB\n"
			   "%s IN KiB: %.2fKiB\n",
			   final_file_name, size, final_file_name, size_in_kib);
	} else if (size <= BYTES_IN_GIB) {
		const double size_in_kib = size / BYTES_IN_KIB;
		const double size_in_mib = size / BYTES_IN_MIB;

		printf("%s IN BYTES: %.0fB\n"
			   "%s IN KiB: %.2fKiB\n"
			   "%s IN MiB: %.2fMiB\n",
			   final_file_name, size, final_file_name, size_in_kib,
			   final_file_name, size_in_mib);
	} else {
		const double size_in_kib = size / BYTES_IN_KIB;
		const double size_in_mib = size / BYTES_IN_MIB;
		const double size_in_gib = size / BYTES_IN_GIB;

		printf("%s IN BYTES: %.0fB\n"
			   "%s IN KiB: %.2fKiB\n"
			   "%s IN MiB: %.2fMiB\n"
			   "%s IN GiB: %.2fGiB\n",
			   final_file_name, size, final_file_name, size_in_kib,
			   final_file_name, size_in_mib, final_file_name, size_in_gib);
	}
}
