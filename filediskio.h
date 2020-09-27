/* A low-level file disk interface required for FatFs */

#ifndef _FILEDISKIO
#define _FILEDISKIO

#include "fatfs/ff.h"
#include "fatfs/diskio.h"
#include <stdbool.h>

void file_disk_setup(const char *filename);
void file_disk_free();

DSTATUS file_disk_status();
DSTATUS file_disk_initialize();
DRESULT file_disk_read(BYTE *buff, LBA_t sector, UINT count);
DRESULT file_disk_write(const BYTE *buff, LBA_t sector, UINT count);
DRESULT file_disk_ioctl(BYTE cmd, void *buff);

#endif
