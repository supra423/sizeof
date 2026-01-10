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
    } // after this, filename[c] should point at the last char of filename

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

void display_help() {
    printf(
        "\nChecks logical file size in Bytes, KiB, MiB, and GiB\n"
        "Amount of varying units depends on file size\n"
        "If file size is greater than 1 MiB, it should output size in:\n"
        "B, KiB, and MiB\n"
        "but if file size is only lesser than 1 KiB, it should only\n"
        "output size in Bytes.\n"
        "This program can accept multiple files as arguments.\n\n"
        "Example usage: \n\n"
        "$ sizeof foo.h dir/bar.c\n\n"
        "OUTPUT:\n\n"
        "FILE: foo.h\n"
        "foo.h IN BYTES: 123B\n\n"
        "FILE: dir/bar.c\n"
        "dir/bar.c IN BYTES: 1234B\n"
        "dir/bar.c IN KiB: 1.23KiB\n\n"
        "OPTIONS:\n"
        "\n\"--help / -h\" - to display this message\n"
        "\n\"--truncate-filename / -tf\" - to truncate file name, e.g.,\n"
        "\t\"dir/bar.c\" to \"bar.c\" only.\n\n"
        "WARNING:\n\nMake sure that the arguments you enter that are\n"
        "intended to be used as a file name or path don't start with: '-'\n"
        "the program will misinterpret it as an option. It's pretty unlikely\n"
        "to happen though because who the hell names files/directories\n"
        "prefixed with a '-'.\n");
}

void display_output(char *truncated_file_name, double size) {
    printf("\nFILE: %s\n", truncated_file_name);
    if (size <= BYTES_IN_KIB) {
        printf("%s IN BYTES: %.0fB\n", truncated_file_name, size);
    } else if (size <= BYTES_IN_MIB) {
        const double size_in_kib = size / BYTES_IN_KIB;

        printf("%s IN BYTES: %.0fB\n", truncated_file_name, size);
        printf("%s IN KiB: %.2fKiB\n", truncated_file_name, size_in_kib);
    } else if (size <= BYTES_IN_GIB) {
        const double size_in_kib = size / BYTES_IN_KIB;
        const double size_in_mib = size / BYTES_IN_MIB;

        printf("%s IN BYTES: %.0fB\n", truncated_file_name, size);
        printf("%s IN KiB: %.2fKiB\n", truncated_file_name, size_in_kib);
        printf("%s IN MiB: %.2fMiB\n", truncated_file_name, size_in_mib);
    } else {
        const double size_in_kib = size / BYTES_IN_KIB;
        const double size_in_mib = size / BYTES_IN_MIB;
        const double size_in_gib = size / BYTES_IN_GIB;

        printf("%s IN BYTES: %.0fB\n", truncated_file_name, size);
        printf("%s IN KiB: %.2fKiB\n", truncated_file_name, size_in_kib);
        printf("%s IN MiB: %.2fMiB\n", truncated_file_name, size_in_mib);
        printf("%s IN GiB: %.2fGiB\n", truncated_file_name, size_in_gib);
    }
}
