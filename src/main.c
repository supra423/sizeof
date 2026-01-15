#include "lib.h"
#include <dirent.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

size_t total_bytes = 0;
bool truncate_flag = false;
bool silent_flag = false;
bool is_dir = true;

static inline void evaluate_options(char *argv[], int i) {
	// if argument is prefixed with '-' it might be an option
	if (*argv[i] == '-') {
		if (argv[i] == NULL || strcmp(argv[i], "--help") == 0 ||
			strcmp(argv[i], "-h") == 0) {
			HELP();
			exit(0);
		} else if (strcmp(argv[i], "--truncate-filename") == 0 ||
				   strcmp(argv[i], "-tf") == 0) {
			truncate_flag = 1;
		} else if ((strcmp(argv[i], "--silent") == 0 ||
					strcmp(argv[i], "-s") == 0) &&
				   is_dir == true) {
			silent_flag = 1;
		} else {
			printf("\"%s\" option doesn't exist, check for typos!\n", argv[i]);
			exit(1);
		}
	}
}

static inline int process_args(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++) {
		evaluate_options(argv, i);

		if (*argv[i] == '-')
			// skip arg if it is an option determined by the prefix '-'
			// this avoids treating the arg as a file
			continue;

		char *file_name = argv[i];

		// lstat to check first if it is a dir or not
		struct stat buf;
		lstat(file_name, &buf);

		if (S_ISDIR(buf.st_mode)) {
			// is_dir = 1;
			is_dir = true;
			remove_last_slash(file_name);
		} else {
			silent_flag = false;
			is_dir = false;
		}

		process_file(file_name, &buf);

		if (is_dir) {
			display_dir_output(argv[i], total_bytes);
			total_bytes = 0.0;
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		HELP();
		return 0;
	}
	int return_val = process_args(argc, argv);
	return return_val;
}
