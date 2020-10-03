# Flopgen
Flopgen is a tool for automatic creation of FAT-formatted floppy disk images with user-supplied files.

This program should be especially useful for people who need to transfer files frequently between their main machines and emulated/virtualised legacy systems with no or unreliable CD support (e.g. DOS and old versions of Windows).

## Supported functions
* Creating FAT12-formatted .img floppy disk images in a raw format with capacity either 360, 720, 1200, 1440 or 2880 KB
* Making as many images as required automatically if supplied files do not fit in one disk
* Organising provided files so that the largest ones are put in a floppy disk image first
* Reading the list of files to be added to images from either the command line, the standard input or a text file
* Processing both files and directories supplied by the user (directories are always processed recursively)

## Installation
### Pre-built binaries
You can download pre-built binaries from the releases page.

### From source code
1. Make sure that a compiler supporting the C++17 standard is installed on your machine.
2. Clone the repository: `git clone https://github.com/maksgraczyk/Flopgen`.
3. Navigate to the directory where you have cloned the repository and run `make`.
4. Done! You can run Flopgen by executing e.g. `./flopgen` on Linux/Unix.

## How to use
For the first use, running `flopgen --help` is recommended. This will display a help message explaining how to use the program along with all available flags and options.

Apart from running help, you **must** always specify the `-o`/`--output` option when using Flopgen. This option tells the program how to name resultant floppy disk images (there might be more than one image produced). For example, executing Flopgen with `-o test` will produce images of form `test1.img`, `test2.img`, `test3.img` etc.

By default, Flopgen creates FAT12-formatted 1440 KB floppy disk images using code page 437. Long file name support is always enabled.

If a directory or a file takes too much disk space (i.e. takes more disk space than the specified floppy disk capacity), Flopgen will not add it to any image. A directory is counted as one inseparable element. **Please note that the disk space occupied by a file is not necessarily the same as the size of the file due to how FAT is structured.**

When Flopgen runs successfully, you will see `Successfully created X floppy disk images.`, where X is the number of resultant images. These are produced in the same directory Flopgen is run in (because of that, do not run the program with `-o` set to some path like `test/disk`, this will not work).

### Examples
#### Making a 1440 KB floppy disk image with files test1.dat, test2.dat and test3.dat
`flopgen -o (name) test1.dat test2.dat test3.dat`

#### Making a 720 KB floppy disk image with directory Floppy
`flopgen -o (name) -s 720 Floppy`

#### Making a 1440 KB floppy disk image with files listed by the standard input
*Filenames must be separated by a new line in the list.*

`flopgen -o (name) -i -`

#### Making a 2880 KB floppy disk image with files provided by an external list called list.txt
*Filenames must be separated by a new line in the list.*

`flopgen -o (name) -s 2880 -i list.txt`

#### Making a 1440 KB floppy disk image with directory Disk and using code page 852
`flopgen -o (name) -c 852 Disk`

#### Making a 1440 KB floppy disk image with all files from directory Folder (Linux/Unix)
*The command is Linux/Unix-specific. In this case, the resultant images will contain all files stored inside directory Folder, but without the directory entry Folder itself.*

`flopgen -o (name) Folder/*`

## Found a bug?
If so, don't hesitate to report it in the issues page.

## Want to contribute?
Great! All contributions are welcome.

## Third-party code used
* **FatFs:** Flopgen uses FatFs created by ChaN and partially modified by myself. Its code is stored in the `fatfs` directory and is licensed under the terms outlined in `fatfs/LICENSE.txt`. The exception is `fatfs/fattime.c` which is a file written by myself and released under the 2-clause BSD license.
* **CLI11:** Flopgen uses CLI11 for command line input parsing. Its code is stored in the `cli` directory and is licensed under the terms outlined in `cli/LICENSE`.

## License
Apart from the third-party code indicated in the previous section and `fatfs/fattime.c`, the project is released under the GNU General Public License v3.0 (see `LICENSE`). If in doubt, check the starting comment of a source file for licensing information applicable to the file.
