#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int process_args(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i] == NULL || strcmp(argv[i], "--help") == 0 ||
            strcmp(argv[i], "-h") == 0 || argc == 1) {
            display_help();
            return 0;
        }
        char *file_name = {argv[i]};
        printf("\n");

        // stat struct contains info about the file like size, file type, etc.
        struct stat buf;
        stat(file_name, &buf);

        if (access(file_name, F_OK) != 0) {
            printf("File \"%s\" doesn't exist, check for typos!\n", file_name);
            continue;
        }

        if (S_ISDIR(buf.st_mode)) {
            printf("File type is a directory! Cannot calculate size\n");
            continue;
        }

        if (!S_ISREG(buf.st_mode)) {
            printf("File type not recognized!\n");
            continue;
        }

        // st_size stores file size in bytes defined in sys/stat.h
        const double size = buf.st_size;

        if (size != -1) {
            char *truncated_file_name = truncate_file_name(file_name);
            display_output(i, truncated_file_name, size);
            free(truncated_file_name);
        }
    }
    return 0; // not sure if this is necessary, I just wanna omit the warning
}

int main(int argc, char *argv[]) { return process_args(argc, argv); }
