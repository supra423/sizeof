#include "constants.h"
#include "lib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static bool truncate_flag = false;

void evaluate_options(char *argv[], int i) {
	if (*argv[i] == '-') {
		// if argument is prefixed with '-' it might be an option
		if (argv[i] == NULL || strcmp(argv[i], "--help") == 0 ||
			strcmp(argv[i], "-h") == 0) {
			HELP();
			exit(0);
		} else if (strcmp(argv[i], "--truncate-filename") == 0 ||
				   strcmp(argv[i], "-tf") == 0) {
			truncate_flag = true;
		} else {
			printf("\"%s\" option doesn't exist, check for typos!\n", argv[i]);
			exit(1);
		}
	}
}

int evaluate_st_mode(struct stat buf, char *file_name) {
	if (S_ISDIR(buf.st_mode)) {
		printf("\"%s\" is a directory! Cannot calculate size\n", file_name);
		return -1;
	}

	if (!S_ISREG(buf.st_mode)) {
		printf("File: \"%s\" file type is not recognized!\n", file_name);
		return -1;
	}
	return 0;
}

void process_output(char *file_name, double size) {
	bool is_mallocd = false;
	char *final_file_name;
	if (truncate_flag) {
		final_file_name = truncate_file_name(file_name);
		is_mallocd = true;
		if (final_file_name == NULL) {
			final_file_name = file_name;
			is_mallocd = false;
		}
	} else
		final_file_name = file_name;

	display_output(final_file_name, size);

	if (is_mallocd)
		free(final_file_name);
}

int process_args(int argc, char *argv[]) {
	if (argc == 1) {
		HELP();
		return 0;
	}
	for (int i = 1; i < argc; i++) {
		evaluate_options(argv, i);
		char *file_name = argv[i];

		if (*file_name == '-')
			// skip arg if it is an option determined by the prefix '-'
			// this avoids treating the arg as a file
			continue;

		if (access(file_name, F_OK) != 0) {
			printf("File \"%s\" doesn't exist, check for typos!\n", file_name);
			continue;
		}

		// stat struct contains info about the file like size, file type, etc.
		struct stat buf;
		stat(file_name, &buf);

		if (evaluate_st_mode(buf, file_name) == -1)
			continue;

		// st_size stores file size in bytes defined in sys/stat.h
		const double size = buf.st_size;

		if (size != -1) {
			process_output(file_name, size);
		}
	}
	return 0;
}

int main(int argc, char *argv[]) { return process_args(argc, argv); }
