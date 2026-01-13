#include "constants.h"
#include "lib.h"
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int truncate_flag = 0;
double total_bytes = 0.0;

void evaluate_options(char *argv[], int i) {
	// if argument is prefixed with '-' it might be an option
	if (*argv[i] == '-') {
		if (argv[i] == NULL || strcmp(argv[i], "--help") == 0 ||
			strcmp(argv[i], "-h") == 0) {
			HELP();
			exit(0);
		} else if (strcmp(argv[i], "--truncate-filename") == 0 ||
				   strcmp(argv[i], "-tf") == 0) {
			truncate_flag = 1;
		} else {
			printf("\"%s\" option doesn't exist, check for typos!\n", argv[i]);
			exit(1);
		}
	}
}

void process_file(char *file_name, int truncate_flag, int is_dir) {
	// stat struct contains info about the file like size, file type, etc.
	struct stat buf;
	lstat(file_name, &buf);

	if (access(file_name, F_OK) != 0 && buf.st_size <= 0) {
		printf("FILE: \"%s\" doesn't exist, check for typos!\n", file_name);
		return;
	}

	if (evaluate_st_mode(buf, file_name) == 1)
		return;

	if (S_ISDIR(buf.st_mode) == 1) {
		DIR *dir = opendir(file_name);
		if (dir == NULL) {
			return;
		}
		struct dirent *dir_ent;
		while ((dir_ent = readdir(dir))) {
			if (strcmp(dir_ent->d_name, ".") != 0 &&
				strcmp(dir_ent->d_name, "..") != 0) {
				char *temp_concat = concat(file_name, dir_ent->d_name);
				struct stat inner_buf;
				lstat(temp_concat, &inner_buf);
				if (S_ISLNK(inner_buf.st_mode) == 1) {
					continue;
				}
				char *concat_dir_to_file_name = concat(file_name, "/");
				char *concat_path_name =
					concat(concat_dir_to_file_name, dir_ent->d_name);
				process_file(concat_path_name, truncate_flag, is_dir);
				free(concat_dir_to_file_name);
				free(concat_path_name);
				free(temp_concat);
			}
		}
		closedir(dir);
		return;
	}
	// st_size stores file size in bytes defined in sys/stat.h
	const double size = buf.st_size;
	total_bytes += size;

	if (size != 1) {
		process_output(file_name, size, truncate_flag, is_dir);
	}
	return;
}

void remove_last_slash(char *file_name) {
	// if command line arg looks like "dir/"
	int c = strlen(file_name);
	if (file_name[c - 1] == '/' && c > 1)
		file_name[c - 1] = '\0';
}

int process_args(int argc, char *argv[]) {
	int is_dir = 0;
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
			is_dir = 1;
			remove_last_slash(file_name);
		}

		process_file(file_name, truncate_flag, is_dir);
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
	return process_args(argc, argv);
}
