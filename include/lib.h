#ifndef LIB_H
#define LIB_H
#include <sys/stat.h>

// slices a file name if the file name contains directories of its path,
// e.g., "src/main.c", the function should return "main.c"
char *truncate_file_name(char filename[]);
char *concat(const char *s1, const char *s2);
void process_output(char *file_name, double size, int truncate_flag,
					int is_dir);
int evaluate_st_mode(struct stat buf, char *file_name);

// displays varying output depending on file size, if file size
// is lesser than a KiB, it should only display size in bytes, or if
// the file size is greater than a GiB, it should display all sizes in
// bytes, KiB, MiB, GiB
void display_file_output_full(char *final_file_name, double size);
void display_file_output_lesser(char *final_file_name, double size);
void display_dir_output(char *final_file_name, double size);

void process_directory(char *dir_name);
void process_file(char *file_name, int truncate_flag, int is_dir);

#endif
