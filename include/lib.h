#ifndef LIB_H
#define LIB_H

// slices a file name if the file name contains directories of its path,
// e.g., "src/main.c", the function should return "main.c"
char *truncate_file_name(char filename[]);

void display_help();

// displays varying output depending on file size, if file size
// is lesser than a KiB, it should only display size in bytes, or if
// the file size is greater than a GiB, it should display all sizes in
// bytes, KiB, MiB, GiB
void display_output(char *truncated_file_name, double size);

#endif
