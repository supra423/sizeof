#ifndef LIB_H
#define LIB_H
#include <arena.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

extern size_t total_bytes;
extern bool truncate_flag;
extern bool silent_flag;
extern bool is_dir;

#define HELP()                                                                 \
	printf("\nChecks logical file size in Bytes, KiB, MiB, and GiB\n"          \
		   "Amount of varying units depends on file size\n"                    \
		   "If file size is greater than 1 MiB, it should output size in:\n"   \
		   "B, KiB, and MiB\n"                                                 \
		   "but if file size is only lesser than 1 KiB, it should only\n"      \
		   "output size in Bytes.\n"                                           \
		   "This program can accept multiple files as arguments.\n\n"          \
		   "Example usage: \n\n"                                               \
		   "$ sizeof foo.h dir/bar.c\n\n"                                      \
		   "OUTPUT:\n\n"                                                       \
		   "FILE: foo.h\n"                                                     \
		   "foo.h IN BYTES: 123B\n\n"                                          \
		   "FILE: dir/bar.c\n"                                                 \
		   "dir/bar.c IN BYTES: 1234B\n"                                       \
		   "dir/bar.c IN KiB: 1.23KiB\n\n"                                     \
		   "OPTIONS:\n"                                                        \
		   "\n\"--help / -h\" - to display this message\n"                     \
		   "\n\"--truncate-filename / -tf\" - to truncate file name, e.g.,\n"  \
		   "\t\"dir/bar.c\" to \"bar.c\" only.\n"                              \
		   "\n\"--silent / -s\" - to only output total size of a directory\n"  \
		   "\nWARNING:\n\nMake sure that the arguments you enter that are\n"   \
		   "intended to be used as a file name or path don't start with: "     \
		   "'-'\n"                                                             \
		   "the program will misinterpret it as an option. It's pretty "       \
		   "unlikely\n"                                                        \
		   "to happen though because who the hell names files/directories\n"   \
		   "prefixed with a '-'.\n");

void process_output(char *file_name, size_t size, arena_t *arena);

// // displays varying output depending on file size, if file size
// // is lesser than a KiB, it should only display size in bytes, or if
// // the file size is greater than a GiB, it should display all sizes in
// // bytes, KiB, MiB, GiB
void display_dir_output(char *final_file_name, size_t size);
void process_file(char *file_name, struct stat *buf);

#endif
