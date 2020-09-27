#include "filediskio.h"
#include "classes.hpp"

static File *file_disk;

bool file_disk_setup(const char *filename) {
  file_disk = new File(std::string(filename));
  return file_disk != NULL;
}

void file_disk_free() {
  delete file_disk;
}

DSTATUS file_disk_status() {
  /* TODO */
}

DSTATUS file_disk_initialize() {
  /* TODO */
}

DRESULT file_disk_read(BYTE *buff, LBA_t sector, UINT count) {
  /* TODO */
}

DRESULT file_disk_write(const BYTE *buff, LBA_t sector, UINT count) {
  /* TODO */
}

DRESULT file_disk_ioctl(BYTE cmd, void *buff) {
  /* TODO */
}
