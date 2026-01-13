I created a small utility program that is somehow an alternative to using the
classic Linux commands "du" and "stat" for checking file size. I somehow find it
annoying to use du/stat sometimes so I decided to create a simpler utility
program for myself.

The program can accept multiple files as arguments and displays varying outputs
depending on the size of the file, like this:

```bash
$ sizeof foo.h dir/bar.c
FILE: foo.h
foo.h IN BYTES: 123B

FILE: dir/bar.c
dir/bar.c IN BYTES: 1234B
dir/bar.c IN KiB: 1.23KiB
```

The program can also accept directories which just calculates the total bytes
for each file inside the directory, the output for each files are a lot shorter
though, this is similar to using:

```bash
$ du --si foo/
```

For more info, enter:
```bash 
$ sizeof --help
```
