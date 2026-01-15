# Introduction

I created a small utility program that is somehow an alternative to using the
classic Linux commands "du" and "stat" for checking file size. I somehow find it
annoying to use du/stat sometimes so I decided to create a simpler utility
program for myself.

The program can accept multiple files as arguments and displays varying outputs
depending on the size of the file, like this:

# Usage

```bash
sizeof foo.h dir/bar.c
FILE: foo.h
foo.h IN BYTES: 123B

FILE: dir/bar.c
dir/bar.c IN BYTES: 1234B
dir/bar.c IN KiB: 1.23KiB
```

The program can also accept directories which just calculates the total bytes
for each file inside the directory, the output for each files are a lot shorter
though:
```bash
sizeof src/
src/arena.c IN BYTES: 915B
src/main.c IN KiB: 1.08KiB
src/lib.c IN KiB: 6.93KiB

DIRECTORY: src
src IN BYTES: 9119B
src IN KiB: 8.91KiB
```

For more info, enter:
```bash 
sizeof --help
```
# Installation

Clone the repo:
```bash
git clone https://github.com/supra423/sizeof.git
```
cd into the directory:
```bash
cd sizeof
```
compile the program using make, there are different options, you can use either:
```bash
make debug # compile with debug flags
make release # compile with moderate opimization (O2, etc)
make max # compile with more aggressive opimization (02, march=native, mtune=native, etc)
make # to all compile 3 different binaries
```
choose the binary to put into /usr/bin/ directory to turn it into a command, if you want the max optimization binary, you can do:
```bash
sudo cp bin/max/sizeof -r /usr/bin/
```
you are all set!
