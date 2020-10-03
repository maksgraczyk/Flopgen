/*
  Flopgen: a tool for automatic creation of FAT-formatted floppy disk images
  Copyright (C) 2020 Maksymilian Graczyk.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/* A low-level file disk interface required for FatFs */

#ifndef _FILEDISKIO_H
#define _FILEDISKIO_H

#include "fatfs/ff.h"
#include "fatfs/diskio.h"
#include <stdbool.h>

void file_disk_setup(const char *filename, int bytes);
void file_disk_free();

DSTATUS file_disk_status();
DSTATUS file_disk_initialize();
DRESULT file_disk_read(BYTE *buff, LBA_t sector, UINT count);
DRESULT file_disk_write(const BYTE *buff, LBA_t sector, UINT count);
DRESULT file_disk_ioctl(BYTE cmd, void *buff);

#endif
