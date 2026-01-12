#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BYTES_IN_KIB 1024
#define BYTES_IN_MIB 1048576
#define BYTES_IN_GIB 1073741824

#define HELP()                                                                 \
	{                                                                          \
		printf(                                                                \
			"\nChecks logical file size in Bytes, KiB, MiB, and GiB\n"         \
			"Amount of varying units depends on file size\n"                   \
			"If file size is greater than 1 MiB, it should output size in:\n"  \
			"B, KiB, and MiB\n"                                                \
			"but if file size is only lesser than 1 KiB, it should only\n"     \
			"output size in Bytes.\n"                                          \
			"This program can accept multiple files as arguments.\n\n"         \
			"Example usage: \n\n"                                              \
			"$ sizeof foo.h dir/bar.c\n\n"                                     \
			"OUTPUT:\n\n"                                                      \
			"FILE: foo.h\n"                                                    \
			"foo.h IN BYTES: 123B\n\n"                                         \
			"FILE: dir/bar.c\n"                                                \
			"dir/bar.c IN BYTES: 1234B\n"                                      \
			"dir/bar.c IN KiB: 1.23KiB\n\n"                                    \
			"OPTIONS:\n"                                                       \
			"\n\"--help / -h\" - to display this message\n"                    \
			"\n\"--truncate-filename / -tf\" - to truncate file name, e.g.,\n" \
			"\t\"dir/bar.c\" to \"bar.c\" only.\n\n"                           \
			"WARNING:\n\nMake sure that the arguments you enter that are\n"    \
			"intended to be used as a file name or path don't start with: "    \
			"'-'\n"                                                            \
			"the program will misinterpret it as an option. It's pretty "      \
			"unlikely\n"                                                       \
			"to happen though because who the hell names files/directories\n"  \
			"prefixed with a '-'.\n");                                         \
	}

#endif
